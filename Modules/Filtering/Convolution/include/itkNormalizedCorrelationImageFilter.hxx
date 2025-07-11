/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkNormalizedCorrelationImageFilter_hxx
#define itkNormalizedCorrelationImageFilter_hxx

#include "itkNeighborhoodAlgorithm.h"
#include "itkImageRegionIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkTotalProgressReporter.h"

namespace itk
{
template <typename TInputImage, typename TMaskImage, typename TOutputImage, typename TOperatorValueType>
void
NormalizedCorrelationImageFilter<TInputImage, TMaskImage, TOutputImage, TOperatorValueType>::SetMaskImage(
  const TMaskImage * mask)
{
  this->ProcessObject::SetNthInput(1, const_cast<TMaskImage *>(mask));
  this->DynamicMultiThreadingOn();
  this->ThreaderUpdateProgressOff();
}

template <typename TInputImage, typename TMaskImage, typename TOutputImage, typename TOperatorValueType>
const TMaskImage *
NormalizedCorrelationImageFilter<TInputImage, TMaskImage, TOutputImage, TOperatorValueType>::GetMaskImage() const
{
  return itkDynamicCastInDebugMode<MaskImageType *>(const_cast<DataObject *>(this->ProcessObject::GetInput(1)));
}

template <typename TInputImage, typename TMaskImage, typename TOutputImage, typename TOperatorValueType>
void
NormalizedCorrelationImageFilter<TInputImage, TMaskImage, TOutputImage, TOperatorValueType>::
  GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // Superclass handled the input image, now we just need to handle
  // the mask image is any.
  const InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
  const MaskImagePointer  maskPtr = const_cast<TMaskImage *>(this->GetMaskImage());

  if (!inputPtr || !maskPtr)
  {
    return;
  }

  // get a copy of the input requested region which was set up by the
  // superclass (NeighborhoodOperatorImageFilter)
  const typename TInputImage::RegionType inputRequestedRegion = inputPtr->GetRequestedRegion();

  // set the mask requested region to match the input requested region
  if (maskPtr->GetLargestPossibleRegion().IsInside(inputRequestedRegion))
  {
    maskPtr->SetRequestedRegion(inputRequestedRegion);
  }
  else
  {
    // Couldn't make the masked region match the input region.
    // Throw an exception.

    // store what we tried to request
    maskPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of the mask image.");
    e.SetDataObject(maskPtr);
    throw e;
  }
}

template <typename TInputImage, typename TMaskImage, typename TOutputImage, typename TOperatorValueType>
void
NormalizedCorrelationImageFilter<TInputImage, TMaskImage, TOutputImage, TOperatorValueType>::
  DynamicThreadedGenerateData(const OutputImageRegionType & outputRegionForThread)
{
  // Normalize the template in a local variable. This will simplify
  // the calculations later.
  using NormalizedTemplateType = Neighborhood<typename NumericTraits<OperatorValueType>::RealType, ImageDimension>;
  using OutputPixelRealType = typename NumericTraits<OutputPixelType>::RealType;

  NormalizedTemplateType normalizedTemplate;
  normalizedTemplate.SetRadius(this->GetOperator().GetRadius());

  OutputPixelRealType sum = 0.0;
  OutputPixelRealType sumOfSquares = 0.0;
  {
    for (typename OutputNeighborhoodType::ConstIterator tIt = this->GetOperator().Begin();
         tIt < this->GetOperator().End();
         ++tIt)
    {
      sum += (*tIt);
      sumOfSquares += ((*tIt) * (*tIt));
    }
  }
  auto                      num = static_cast<OutputPixelRealType>(this->GetOperator().Size());
  const OutputPixelRealType mean = sum / num;
  const OutputPixelRealType var = (sumOfSquares - (sum * sum / num)) / (num - 1.0);
  const OutputPixelRealType std = std::sqrt(var);

  // convert std to a scaling factor k such that
  //
  //        || (coeff - mean) / k || = 1.0
  //
  const double k = std * std::sqrt(num - 1.0);

  // normalize the template
  {
    typename OutputNeighborhoodType::ConstIterator tIt = this->GetOperator().Begin();
    for (typename NormalizedTemplateType::Iterator ntIt = normalizedTemplate.Begin(); ntIt < normalizedTemplate.End();
         ++ntIt)
    {
      *ntIt = (*tIt - mean) / k;
      ++tIt;
    }
  }
  // get output/inputs
  OutputImageType *      output = this->GetOutput();
  const InputImageType * input = this->GetInput();
  const MaskImageType *  mask = this->GetMaskImage();

  // Break the input into a series of regions.  The first region is free
  // of boundary conditions, the rest with boundary conditions. Note,
  // we pass in the input image and the OUTPUT requested region. We are
  // only concerned with centering the neighborhood operator at the
  // pixels that correspond to output pixels.
  using BFC = NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>;

  using FaceListType = typename BFC::FaceListType;
  BFC                faceCalculator;
  const FaceListType faceList = faceCalculator(input, outputRegionForThread, this->GetOperator().GetRadius());

  TotalProgressReporter progress(this, output->GetRequestedRegion().GetNumberOfPixels());

  // Process non-boundary region and each of the boundary faces.
  // These are N-d regions which border the edge of the buffer.

  const unsigned int        templateSize = normalizedTemplate.Size();
  const OutputPixelRealType zero = OutputPixelType{};

  auto realTemplateSize = static_cast<OutputPixelRealType>(templateSize);
  for (const auto & face : faceList)
  {
    ConstNeighborhoodIterator<InputImageType> bit =
      ConstNeighborhoodIterator<InputImageType>(normalizedTemplate.GetRadius(), input, face);
    bit.OverrideBoundaryCondition(this->GetBoundaryCondition());
    bit.GoToBegin();

    ImageRegionIterator<OutputImageType> it = ImageRegionIterator<OutputImageType>(output, face);

    if (!mask)
    {
      // No mask is defined
      while (!bit.IsAtEnd())
      {
        // Compute the normalized correlation at this pixel.  The
        // template has already been normalized to mean zero and norm 1.
        // This simplifies the calculation to being just the correlation
        // of the image neighborhood with the template, normalized by a
        // function of the image neighborhood.
        sum = 0.0;
        sumOfSquares = 0.0;
        OutputPixelRealType numerator = 0.0;
        for (unsigned int i = 0; i < templateSize; ++i)
        {
          auto value = static_cast<OutputPixelRealType>(bit.GetPixel(i));

          numerator += (value * normalizedTemplate[i]);

          // tally values for normalizing the result
          sum += value;
          sumOfSquares += (value * value);
        }
        OutputPixelRealType denominator = std::sqrt(sumOfSquares - (sum * sum / realTemplateSize));

        it.Value() = numerator / denominator;

        ++bit;
        ++it;
        progress.CompletedPixel();
      }
    }
    else
    {
      // Mask is defined, use the same calculation as above but only
      // perform it under the mask
      ImageRegionConstIterator<MaskImageType> mit = ImageRegionConstIterator<MaskImageType>(mask, face);
      while (!bit.IsAtEnd())
      {
        if (mit.Get())
        {
          // Compute the normalized correlation at this pixel.  The
          // template has already been normalized to mean zero and norm 1.
          // This simplifies the calculation to being just the correlation
          // of the image neighborhood with the template, normalized by a
          // function of the image neighborhood.
          sum = 0.0;
          sumOfSquares = 0.0;
          OutputPixelRealType numerator = 0.0;
          for (unsigned int i = 0; i < templateSize; ++i)
          {
            auto value = static_cast<OutputPixelRealType>(bit.GetPixel(i));

            numerator += (value * normalizedTemplate[i]);

            // tally values for normalizing the result
            sum += value;
            sumOfSquares += (value * value);
          }
          OutputPixelRealType denominator = std::sqrt(sumOfSquares - (sum * sum / realTemplateSize));

          it.Value() = numerator / denominator;
        }
        else
        {
          // Not under the mask.  Set the normalized correlation to zero.
          it.Value() = zero;
        }
        ++bit;
        ++it;
        ++mit;
        progress.CompletedPixel();
      }
    }
  }
}
} // end namespace itk

#endif
