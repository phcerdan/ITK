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
#ifndef itkAccumulateImageFilter_hxx
#define itkAccumulateImageFilter_hxx

#include "itkImageRegionIterator.h"

namespace itk
{

template <typename TInputImage, typename TOutputImage>
AccumulateImageFilter<TInputImage, TOutputImage>::AccumulateImageFilter()
  : m_AccumulateDimension(InputImageDimension - 1)
{}

template <typename TInputImage, typename TOutputImage>
void
AccumulateImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
{
  itkDebugMacro("GenerateOutputInformation Start");

  typename TInputImage::IndexType      inputIndex;
  typename TInputImage::SizeType       inputSize;
  typename TOutputImage::SizeType      outputSize;
  typename TOutputImage::IndexType     outputIndex;
  typename TInputImage::SpacingType    inSpacing;
  typename TInputImage::PointType      inOrigin;
  typename TOutputImage::SpacingType   outSpacing;
  typename TOutputImage::DirectionType outDirection;
  typename TOutputImage::PointType     outOrigin;

  // Get pointers to the input and output
  const typename Superclass::OutputImagePointer output = this->GetOutput();
  const typename Superclass::InputImagePointer  input = const_cast<TInputImage *>(this->GetInput());

  if (!input || !output)
  {
    return;
  }

  inputIndex = input->GetLargestPossibleRegion().GetIndex();
  inputSize = input->GetLargestPossibleRegion().GetSize();
  inSpacing = input->GetSpacing();
  outDirection = input->GetDirection();
  inOrigin = input->GetOrigin();

  // Set the LargestPossibleRegion of the output.
  // Reduce the size of the accumulated dimension.
  for (unsigned int i = 0; i < InputImageDimension; ++i)
  {
    if (i != m_AccumulateDimension)
    {
      outputSize[i] = inputSize[i];
      outputIndex[i] = inputIndex[i];
      outSpacing[i] = inSpacing[i];
      outOrigin[i] = inOrigin[i];
    }
    else
    {
      outputSize[i] = 1;
      outputIndex[i] = 0;
      outSpacing[i] = inSpacing[i] * inputSize[i];
      outOrigin[i] = inOrigin[i] + (i - 1) * inSpacing[i] / 2;
    }
  }

  const typename TOutputImage::RegionType outputRegion(outputIndex, outputSize);
  output->SetOrigin(outOrigin);
  output->SetSpacing(outSpacing);
  output->SetDirection(outDirection);
  output->SetLargestPossibleRegion(outputRegion);

  itkDebugMacro("GenerateOutputInformation End");
}

template <typename TInputImage, typename TOutputImage>
void
AccumulateImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion()
{
  itkDebugMacro("GenerateInputRequestedRegion Start");
  Superclass::GenerateInputRequestedRegion();

  if (this->GetInput())
  {
    typename TInputImage::SizeType   inputSize;
    typename TInputImage::IndexType  inputIndex;
    typename TInputImage::SizeType   inputLargSize;
    typename TInputImage::IndexType  inputLargIndex;
    typename TOutputImage::SizeType  outputSize;
    typename TOutputImage::IndexType outputIndex = this->GetOutput()->GetRequestedRegion().GetIndex();
    outputSize = this->GetOutput()->GetRequestedRegion().GetSize();
    inputLargSize = this->GetInput()->GetLargestPossibleRegion().GetSize();
    inputLargIndex = this->GetInput()->GetLargestPossibleRegion().GetIndex();

    for (unsigned int i = 0; i < TInputImage::ImageDimension; ++i)
    {
      if (i != m_AccumulateDimension)
      {
        inputSize[i] = outputSize[i];
        inputIndex[i] = outputIndex[i];
      }
      else
      {
        inputSize[i] = inputLargSize[i];
        inputIndex[i] = inputLargIndex[i];
      }
    }

    const typename TInputImage::RegionType RequestedRegion(inputIndex, inputSize);
    const InputImagePointer                input = const_cast<TInputImage *>(this->GetInput());
    input->SetRequestedRegion(RequestedRegion);
  }

  itkDebugMacro("GenerateInputRequestedRegion End");
}

template <typename TInputImage, typename TOutputImage>
void
AccumulateImageFilter<TInputImage, TOutputImage>::GenerateData()
{
  if (m_AccumulateDimension >= TInputImage::ImageDimension)
  {
    itkExceptionMacro(
      "AccumulateImageFilter: invalid dimension to accumulate. AccumulateDimension = " << m_AccumulateDimension);
  }

  using OutputPixelType = typename TOutputImage::PixelType;
  using AccumulateType = typename NumericTraits<OutputPixelType>::AccumulateType;

  const typename Superclass::InputImageConstPointer inputImage = this->GetInput();
  const typename TOutputImage::Pointer              outputImage = this->GetOutput();
  outputImage->SetBufferedRegion(outputImage->GetRequestedRegion());
  outputImage->Allocate();

  // Accumulate over the Nth dimension ( = m_AccumulateDimension)
  // and divide by the size of the accumulated dimension.
  using outputIterType = ImageRegionIterator<TOutputImage>;
  outputIterType outputIter(outputImage, outputImage->GetBufferedRegion());
  using inputIterType = ImageRegionConstIterator<TInputImage>;

  typename TInputImage::SizeType  AccumulatedSize = inputImage->GetLargestPossibleRegion().GetSize();
  typename TInputImage::IndexType AccumulatedIndex = inputImage->GetLargestPossibleRegion().GetIndex();

  const SizeValueType SizeAccumulateDimension = AccumulatedSize[m_AccumulateDimension];
  const auto          sizeAccumulateDimensionDouble = static_cast<double>(SizeAccumulateDimension);
  const typename TInputImage::IndexValueType IndexAccumulateDimension = AccumulatedIndex[m_AccumulateDimension];
  for (unsigned int i = 0; i < InputImageDimension; ++i)
  {
    if (i != m_AccumulateDimension)
    {
      AccumulatedSize[i] = 1;
    }
  }
  while (!outputIter.IsAtEnd())
  {
    typename TOutputImage::IndexType OutputIndex = outputIter.GetIndex();
    for (unsigned int i = 0; i < InputImageDimension; ++i)
    {
      if (i != m_AccumulateDimension)
      {
        AccumulatedIndex[i] = OutputIndex[i];
      }
      else
      {
        AccumulatedIndex[i] = IndexAccumulateDimension;
      }
    }
    const typename TInputImage::RegionType AccumulatedRegion(AccumulatedIndex, AccumulatedSize);
    inputIterType                          inputIter(inputImage, AccumulatedRegion);
    inputIter.GoToBegin();
    AccumulateType Value{};
    while (!inputIter.IsAtEnd())
    {
      Value += static_cast<AccumulateType>(inputIter.Get());
      ++inputIter;
    }
    if (m_Average)
    {
      outputIter.Set(static_cast<OutputPixelType>(Value / sizeAccumulateDimensionDouble));
    }
    else
    {
      outputIter.Set(static_cast<OutputPixelType>(Value));
    }
    ++outputIter;
  }
}

template <typename TInputImage, typename TOutputImage>
void
AccumulateImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "AccumulateDimension: " << m_AccumulateDimension << std::endl;
  itkPrintSelfBooleanMacro(Average);
}
} // end namespace itk

#endif
