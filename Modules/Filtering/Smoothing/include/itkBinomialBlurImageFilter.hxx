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
#ifndef itkBinomialBlurImageFilter_hxx
#define itkBinomialBlurImageFilter_hxx

#include "vnl/vnl_vector_fixed.h"
#include "itkProgressReporter.h"
#include "itkImageRegion.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionReverseIterator.h"
#include <algorithm> // For min and max.

namespace itk
{
template <typename TInputImage, typename TOutputImage>
BinomialBlurImageFilter<TInputImage, TOutputImage>::BinomialBlurImageFilter()
  // The default is to just do one repetition
  : m_Repetitions(1)
{
  itkDebugMacro("BinomialBlurImageFilter::BinomialBlurImageFilter() called");
}

template <typename TInputImage, typename TOutputImage>
void
BinomialBlurImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion()
{
  itkDebugMacro("BinomialBlurImageFilter::GenerateInputRequestedRegion() called");

  Superclass::GenerateInputRequestedRegion();

  const InputImagePointer  inputPtr = const_cast<TInputImage *>(this->GetInput(0));
  const OutputImagePointer outputPtr = this->GetOutput(0);

  if (!inputPtr || !outputPtr)
  {
    return;
  }

  typename TOutputImage::RegionType           outputRegion;
  typename TInputImage::RegionType            inputRegion;
  typename TInputImage::RegionType::SizeType  inputSize;
  typename TInputImage::RegionType::IndexType inputIndex;

  outputRegion = outputPtr->GetRequestedRegion();

  // This filter needs a m_Repetitions pixel border about the output
  // (clamped of course at the true boundaries of the input image)
  inputRegion = outputRegion;

  inputSize = inputRegion.GetSize();
  inputIndex = inputRegion.GetIndex();

  typename TInputImage::RegionType::IndexType inputLargestPossibleRegionIndex =
    inputPtr->GetLargestPossibleRegion().GetIndex();
  typename TInputImage::RegionType::SizeType inputLargestPossibleRegionSize =
    inputPtr->GetLargestPossibleRegion().GetSize();

  for (unsigned int i = 0; i < inputPtr->GetImageDimension(); ++i)
  {
    inputIndex[i] -= m_Repetitions;
    inputIndex[i] = std::max(inputIndex[i], inputLargestPossibleRegionIndex[i]);

    inputSize[i] += m_Repetitions;
    inputSize[i] = std::min(inputSize[i], inputLargestPossibleRegionSize[i]);
  }

  inputRegion.SetIndex(inputIndex);
  inputRegion.SetSize(inputSize);

  inputPtr->SetRequestedRegion(inputRegion);
}

template <typename TInputImage, typename TOutputImage>
void
BinomialBlurImageFilter<TInputImage, TOutputImage>::GenerateData()
{
  itkDebugMacro("BinomialBlurImageFilter::GenerateData() called");

  // Get the input and output pointers
  const InputImageConstPointer inputPtr = this->GetInput(0);
  const OutputImagePointer     outputPtr = this->GetOutput(0);

  // Allocate the output
  outputPtr->SetBufferedRegion(outputPtr->GetRequestedRegion());
  outputPtr->Allocate();

  // Create a temporary image used while processing the image
  // Processing with doubles eliminates possible rounding artifacts which may
  // accumulate over repeated integer division
  using TTempImage = Image<double, NDimensions>;
  auto tempPtr = TTempImage::New();

  const typename TTempImage::RegionType tempRegion = inputPtr->GetRequestedRegion();

  tempPtr->SetRegions(tempRegion);
  tempPtr->Allocate();

  // How big is the input image?
  typename TInputImage::SizeType  size = inputPtr->GetRequestedRegion().GetSize();
  typename TInputImage::IndexType startIndex = inputPtr->GetRequestedRegion().GetIndex();

  // Iterator Typedefs for this routine
  using TempIterator = ImageRegionIterator<TTempImage>;
  using TempReverseIterator = ImageRegionReverseIterator<TTempImage>;
  using InputIterator = ImageRegionConstIterator<TInputImage>;
  using OutputIterator = ImageRegionIterator<TOutputImage>;

  // Create a progress reporter
  ProgressReporter progress(
    this, 0, (outputPtr->GetRequestedRegion().GetNumberOfPixels()) * m_Repetitions * 2 * NDimensions);

  // Copy the input image to the temporary image
  TempIterator  tempIt(tempPtr, tempPtr->GetRequestedRegion());
  InputIterator inputIt(inputPtr, inputPtr->GetRequestedRegion());

  for (inputIt.GoToBegin(), tempIt.GoToBegin(); !tempIt.IsAtEnd(); ++tempIt, ++inputIt)
  {
    tempIt.Set(static_cast<double>(inputIt.Get()));
  }

  // Define a few indices that will be used to translate from an input pixel
  // to an output pixel
  typename TTempImage::IndexType index;
  typename TTempImage::IndexType indexShift;

  // walk the output image forwards and compute blur
  for (unsigned int rep = 0; rep < m_Repetitions; ++rep)
  {
    itkDebugMacro("Repetition #" << rep);

    // blur each dimension
    for (unsigned int dim = 0; dim < NDimensions; ++dim)
    {
      TempIterator tempItDir(tempPtr, tempPtr->GetRequestedRegion());
      tempItDir.GoToBegin();
      while (!tempItDir.IsAtEnd())
      {
        // determine the index of the output pixel
        index = tempItDir.GetIndex();

        if (index[dim] < (startIndex[dim] + static_cast<typename TTempImage::OffsetValueType>(size[dim]) - 1))
        {
          // Figure out the location of the "neighbor" pixel
          for (unsigned int i = 0; i < NDimensions; ++i)
          {
            if (i == dim)
            {
              indexShift.m_InternalArray[i] = index.m_InternalArray[i] + 1;
            }
            else
            {
              indexShift.m_InternalArray[i] = index.m_InternalArray[i];
            }
          }

          // Average the pixel of interest and shifted pixel
          double       pixelA = tempPtr->GetPixel(index);
          const double pixelB = tempPtr->GetPixel(indexShift);

          pixelA += pixelB;
          pixelA = pixelA / 2.0;

          tempPtr->SetPixel(index, pixelA);
          progress.CompletedPixel();
        }

        ++tempItDir;
      } // end walk the image forwards

      itkDebugMacro("End processing forward dimension " << dim);

      //----------------------Reverse pass----------------------
      TempReverseIterator tempReverseIt(tempPtr, tempPtr->GetRequestedRegion());

      tempReverseIt.GoToBegin();

      while (!tempReverseIt.IsAtEnd())
      {
        // determine the index of the output pixel
        index = tempReverseIt.GetIndex();

        if (index[dim] > startIndex[dim])
        {
          // Figure out the location of the "neighbor" pixel
          for (unsigned int i = 0; i < NDimensions; ++i)
          {
            if (i == dim)
            {
              indexShift.m_InternalArray[i] = index.m_InternalArray[i] - 1;
            }
            else
            {
              indexShift.m_InternalArray[i] = index.m_InternalArray[i];
            }
          }

          // Average the pixel of interest and shifted pixel
          double       pixelA = tempPtr->GetPixel(index);
          const double pixelB = tempPtr->GetPixel(indexShift);

          pixelA += pixelB;
          pixelA = pixelA / 2;

          tempPtr->SetPixel(index, pixelA);
          progress.CompletedPixel();
        }

        ++tempReverseIt;
      } // end walk the image backwards
    } // end dimension loop
  } // end number of repetitions loop

  // Now, copy the temporary image to the output image. Note that the temp
  // buffer iterator walks a region defined by the output
  using OutputIterator = ImageRegionIterator<TOutputImage>;

  OutputIterator outIt(outputPtr, outputPtr->GetRequestedRegion());
  TempIterator   tempIt2(tempPtr, outputPtr->GetRequestedRegion());

  for (outIt.GoToBegin(), tempIt2.GoToBegin(); !outIt.IsAtEnd(); ++outIt, ++tempIt2)
  {
    outIt.Set(static_cast<PixelType>(tempIt2.Get()));
  }
}

template <typename TInputImage, typename TOutputImage>
void
BinomialBlurImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Number of repetitions: " << m_Repetitions << std::endl;
}
} // namespace itk

#endif
