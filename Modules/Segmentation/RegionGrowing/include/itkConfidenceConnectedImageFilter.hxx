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
#ifndef itkConfidenceConnectedImageFilter_hxx
#define itkConfidenceConnectedImageFilter_hxx

#include "itkMath.h"
#include "itkMacro.h"
#include "itkImageRegionIterator.h"
#include "itkImageNeighborhoodOffsets.h"
#include "itkShapedImageNeighborhoodRange.h"
#include "itkBinaryThresholdImageFunction.h"
#include "itkFloodFilledImageFunctionConditionalIterator.h"
#include "itkProgressReporter.h"
#include "itkPrintHelper.h"
#include <algorithm> // For min and max.

namespace itk
{

template <typename TInputImage, typename TOutputImage>
ConfidenceConnectedImageFilter<TInputImage, TOutputImage>::ConfidenceConnectedImageFilter()
  : m_Multiplier(2.5)
  , m_NumberOfIterations(4)
  , m_ReplaceValue(NumericTraits<OutputImagePixelType>::OneValue())
  , m_InitialNeighborhoodRadius(1)
  , m_Mean(InputRealType{})
  , m_Variance(InputRealType{})
{
  m_Seeds.clear();
}

template <typename TInputImage, typename TOutputImage>
void
ConfidenceConnectedImageFilter<TInputImage, TOutputImage>::SetSeed(const IndexType & seed)
{
  this->m_Seeds.clear();
  this->AddSeed(seed);
}

template <typename TInputImage, typename TOutputImage>
void
ConfidenceConnectedImageFilter<TInputImage, TOutputImage>::ClearSeeds()
{
  if (this->m_Seeds.size() > 0)
  {
    this->m_Seeds.clear();
    this->Modified();
  }
}

template <typename TInputImage, typename TOutputImage>
void
ConfidenceConnectedImageFilter<TInputImage, TOutputImage>::AddSeed(const IndexType & seed)
{
  this->m_Seeds.push_back(seed);
  this->Modified();
}

template <typename TInputImage, typename TOutputImage>
auto
ConfidenceConnectedImageFilter<TInputImage, TOutputImage>::GetSeeds() const -> const SeedsContainerType &
{
  itkDebugMacro("returning Seeds");
  return this->m_Seeds;
}

template <typename TInputImage, typename TOutputImage>
void
ConfidenceConnectedImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream & os, Indent indent) const
{
  using namespace print_helper;

  Superclass::PrintSelf(os, indent);

  os << indent << "Seeds: " << m_Seeds << std::endl;
  os << indent << "Multiplier: " << m_Multiplier << std::endl;
  os << indent << "NumberOfIterations: " << m_NumberOfIterations << std::endl;
  os << indent
     << "ReplaceValue: " << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_ReplaceValue)
     << std::endl;
  os << indent << "InitialNeighborhoodRadius: " << m_InitialNeighborhoodRadius << std::endl;
  os << indent << "Mean: " << static_cast<typename NumericTraits<InputRealType>::PrintType>(m_Mean) << std::endl;
  os << indent << "Variance: " << static_cast<typename NumericTraits<InputRealType>::PrintType>(m_Variance)
     << std::endl;
}

template <typename TInputImage, typename TOutputImage>
void
ConfidenceConnectedImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
  if (this->GetInput())
  {
    const InputImagePointer input = const_cast<TInputImage *>(this->GetInput());
    input->SetRequestedRegionToLargestPossibleRegion();
  }
}

template <typename TInputImage, typename TOutputImage>
void
ConfidenceConnectedImageFilter<TInputImage, TOutputImage>::EnlargeOutputRequestedRegion(DataObject * output)
{
  Superclass::EnlargeOutputRequestedRegion(output);
  output->SetRequestedRegionToLargestPossibleRegion();
}

template <typename TInputImage, typename TOutputImage>
void
ConfidenceConnectedImageFilter<TInputImage, TOutputImage>::GenerateData()
{
  using FunctionType = BinaryThresholdImageFunction<InputImageType, double>;
  using SecondFunctionType = BinaryThresholdImageFunction<OutputImageType, double>;

  using IteratorType = FloodFilledImageFunctionConditionalIterator<OutputImageType, FunctionType>;
  using SecondIteratorType = FloodFilledImageFunctionConditionalConstIterator<InputImageType, SecondFunctionType>;

  const typename Superclass::InputImageConstPointer inputImage = this->GetInput();
  const typename Superclass::OutputImagePointer     outputImage = this->GetOutput();

  // Zero the output
  const OutputImageRegionType region = outputImage->GetRequestedRegion();
  outputImage->SetBufferedRegion(region);
  outputImage->AllocateInitialized();

  // Compute the statistics of the seed point

  // Set up the image function used for connectivity
  auto function = FunctionType::New();
  function->SetInputImage(inputImage);
  m_Mean = InputRealType{};
  m_Variance = InputRealType{};

  if (m_InitialNeighborhoodRadius > 0)
  {
    const auto neighborhoodOffsets =
      GenerateRectangularImageNeighborhoodOffsets(SizeType::Filled(m_InitialNeighborhoodRadius));
    auto neighborhoodRange =
      ShapedImageNeighborhoodRange<const InputImageType>(*inputImage, IndexType(), neighborhoodOffsets);

    InputRealType sumOfSquares{};

    auto          si = m_Seeds.begin();
    const auto    li = m_Seeds.end();
    SizeValueType num = 0;
    while (si != li)
    {
      if (region.IsInside(*si))
      {
        neighborhoodRange.SetLocation(*si);

        InputRealType neighborhoodSum{ 0.0 };
        InputRealType neighborhoodSumOfSquares{ 0.0 };

        for (const InputImagePixelType pixelValue : neighborhoodRange)
        {
          const auto realValue = static_cast<InputRealType>(pixelValue);

          neighborhoodSum += realValue;
          neighborhoodSumOfSquares += (realValue * realValue);
        }
        m_Mean += neighborhoodSum / neighborhoodRange.size();
        sumOfSquares += neighborhoodSumOfSquares;
        ++num;
      }
      ++si;
    }

    if (num == 0)
    {
      this->UpdateProgress(1.0);
      // no seeds result in zero image
      return;
    }

    const double totalNum = num * neighborhoodRange.size();
    m_Mean /= num;
    m_Variance = (sumOfSquares - (m_Mean * m_Mean * totalNum)) / (totalNum - 1.0);
  }
  else
  {
    InputRealType sum{};
    InputRealType sumOfSquares{};

    auto          si = m_Seeds.begin();
    const auto    li = m_Seeds.end();
    SizeValueType num = 0;
    while (si != li)
    {
      if (region.IsInside(*si))
      {
        const auto value = static_cast<InputRealType>(inputImage->GetPixel(*si));

        sum += value;
        sumOfSquares += value * value;
        ++num;
      }
      ++si;
    }

    if (num == 0)
    {
      this->UpdateProgress(1.0);
      // no seeds result in zero image
      return;
    }
    m_Mean = sum / static_cast<double>(num);
    m_Variance = (sumOfSquares - (sum * sum / static_cast<double>(num))) / (static_cast<double>(num) - 1.0);
  }

  InputRealType lower = m_Mean - m_Multiplier * std::sqrt(m_Variance);
  InputRealType upper = m_Mean + m_Multiplier * std::sqrt(m_Variance);

  // Find the highest and lowest seed intensity.
  InputRealType lowestSeedIntensity = itk::NumericTraits<InputImagePixelType>::max();
  InputRealType highestSeedIntensity = itk::NumericTraits<InputImagePixelType>::NonpositiveMin();
  auto          si = m_Seeds.begin();
  const auto    li = m_Seeds.end();
  while (si != li)
  {
    if (region.IsInside(*si))
    {
      const auto seedIntensity = static_cast<InputRealType>(inputImage->GetPixel(*si));

      if (lowestSeedIntensity > seedIntensity)
      {
        lowestSeedIntensity = seedIntensity;
      }
      if (highestSeedIntensity < seedIntensity)
      {
        highestSeedIntensity = seedIntensity;
      }
    }
    ++si;
  }

  // Adjust lower and upper to always contain the seed's intensity, otherwise,
  // no pixels will be
  // returned by the iterator and a zero variance will result
  if (lower > lowestSeedIntensity)
  {
    lower = lowestSeedIntensity;
  }
  if (upper < highestSeedIntensity)
  {
    upper = highestSeedIntensity;
  }

  // Make sure the lower and upper limit are not outside the valid range of the
  // input
  lower = std::max(lower, static_cast<InputRealType>(NumericTraits<InputImagePixelType>::NonpositiveMin()));
  upper = std::min(upper, static_cast<InputRealType>(NumericTraits<InputImagePixelType>::max()));

  function->ThresholdBetween(static_cast<InputImagePixelType>(lower), static_cast<InputImagePixelType>(upper));

  itkDebugMacro("\nLower intensity = " << lower << ", Upper intensity = " << upper << "\nmean = " << m_Mean
                                       << " , std::sqrt(variance) = " << std::sqrt(m_Variance));

  // Segment the image, the iterator walks the output image (so Set()
  // writes into the output image), starting at the seed point.  As
  // the iterator walks, if the corresponding pixel in the input image
  // (accessed via the "function" assigned to the iterator) is within
  // the [lower, upper] bounds prescribed, the pixel is added to the
  // output segmentation and its neighbors become candidates for the
  // iterator to walk.
  IteratorType it(outputImage, function, m_Seeds);
  it.GoToBegin();
  while (!it.IsAtEnd())
  {
    it.Set(m_ReplaceValue);
    ++it;
  }

  ProgressReporter progress(this, 0, region.GetNumberOfPixels() * m_NumberOfIterations);

  for (unsigned int loop = 0; loop < m_NumberOfIterations; ++loop)
  {
    // Now that we have an initial segmentation, let's recalculate the
    // statistics.  Since we have already labelled the output, we visit
    // pixels in the input image that have been set in the output image.
    // Essentially, we flip the iterator around, so we walk the input
    // image (so Get() will get pixel values from the input) and constrain
    // iterator such it only visits pixels that were set in the output.
    auto secondFunction = SecondFunctionType::New();
    secondFunction->SetInputImage(outputImage);
    secondFunction->ThresholdBetween(m_ReplaceValue, m_ReplaceValue);

    InputRealType                        sum{};
    InputRealType                        sumOfSquares{};
    typename TOutputImage::SizeValueType numberOfSamples = 0;

    SecondIteratorType sit(inputImage, secondFunction, m_Seeds);
    sit.GoToBegin();
    while (!sit.IsAtEnd())
    {
      const auto value = static_cast<InputRealType>(sit.Get());
      sum += value;
      sumOfSquares += value * value;
      ++numberOfSamples;
      ++sit;
    }
    m_Mean = sum / static_cast<double>(numberOfSamples);
    m_Variance = (sumOfSquares - (sum * sum / static_cast<double>(numberOfSamples))) /
                 (static_cast<double>(numberOfSamples) - 1.0);
    // if the variance is zero, there is no point in continuing
    if (Math::AlmostEquals(m_Variance, 0.0))
    {
      itkDebugMacro("\nLower intensity = " << lower << ", Upper intensity = " << upper << "\nmean = " << m_Mean
                                           << ", variance = " << m_Variance
                                           << " , std::sqrt(variance) = " << std::sqrt(m_Variance));
      itkDebugMacro("\nsum = " << sum << ", sumOfSquares = " << sumOfSquares
                               << "\nnumberOfSamples = " << numberOfSamples);
      break;
    }
    lower = m_Mean - m_Multiplier * std::sqrt(m_Variance);
    upper = m_Mean + m_Multiplier * std::sqrt(m_Variance);

    // Adjust lower and upper to always contain the seed's intensity, otherwise,
    // no pixels will be
    // returned by the iterator and a zero variance will result
    if (lower > lowestSeedIntensity)
    {
      lower = lowestSeedIntensity;
    }
    if (upper < highestSeedIntensity)
    {
      upper = highestSeedIntensity;
    }
    // Make sure the lower and upper limit are not outside the valid range of
    // the input
    lower = std::max(lower, static_cast<InputRealType>(NumericTraits<InputImagePixelType>::NonpositiveMin()));
    upper = std::min(upper, static_cast<InputRealType>(NumericTraits<InputImagePixelType>::max()));

    function->ThresholdBetween(static_cast<InputImagePixelType>(lower), static_cast<InputImagePixelType>(upper));

    itkDebugMacro("\nLower intensity = " << lower << ", Upper intensity = " << upper << "\nmean = " << m_Mean
                                         << ", variance = " << m_Variance
                                         << " , std::sqrt(variance) = " << std::sqrt(m_Variance));
    itkDebugMacro("\nsum = " << sum << ", sumOfSquares = " << sumOfSquares << "\nnum = " << numberOfSamples);

    // Rerun the segmentation, the iterator walks the output image,
    // starting at the seed point.  As the iterator walks, if the
    // corresponding pixel in the input image (accessed via the
    // "function" assigned to the iterator) is within the [lower,
    // upper] bounds prescribed, the pixel is added to the output
    // segmentation and its neighbors become candidates for the
    // iterator to walk.
    outputImage->FillBuffer(OutputImagePixelType{});
    IteratorType thirdIt(outputImage, function, m_Seeds);
    thirdIt.GoToBegin();
    try
    {
      while (!thirdIt.IsAtEnd())
      {
        thirdIt.Set(m_ReplaceValue);
        ++thirdIt;
        progress.CompletedPixel(); // potential exception thrown here
      }
    }
    catch (const ProcessAborted &)
    {
      break; // interrupt the iterations loop
    }
  } // end iteration loop

  if (this->GetAbortGenerateData())
  {
    ProcessAborted e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Process aborted.");
    throw ProcessAborted(__FILE__, __LINE__);
  }
}
} // end namespace itk

#endif
