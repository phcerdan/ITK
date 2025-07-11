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

#include <iostream>
#include "itkFrameDifferenceVideoFilter.h"
#include "itkVideoFileReader.h"
#include "itkVideoFileWriter.h"
#include "itkFileListVideoIOFactory.h"
#include "itkTestingMacros.h"


// Set up type alias for test
constexpr unsigned int Dimension = 2;
using InputPixelType = unsigned char;
using InputFrameType = itk::Image<InputPixelType, Dimension>;
using InputVideoType = itk::VideoStream<InputFrameType>;
using OutputPixelType = unsigned char;
using OutputFrameType = itk::Image<OutputPixelType, Dimension>;
using OutputVideoType = itk::VideoStream<OutputFrameType>;
using SizeValueType = itk::SizeValueType;


/**
 * Helper function
 */

namespace itk::FrameDifferenceVideoFilterTest
{

/**
 * Create a new frame and fill it with the indicated value
 */
InputFrameType::Pointer
CreateInputFrame(InputPixelType val)
{
  auto out = InputFrameType::New();

  InputFrameType::RegionType          largestRegion;
  InputFrameType::SizeType            sizeLR;
  constexpr InputFrameType::IndexType startLR{};
  sizeLR[0] = 50;
  sizeLR[1] = 40;
  largestRegion.SetSize(sizeLR);
  largestRegion.SetIndex(startLR);
  out->SetRegions(largestRegion);

  out->Allocate();

  // Fill with the desired value
  itk::ImageRegionIterator<InputFrameType> iter(out, largestRegion);
  while (!iter.IsAtEnd())
  {
    iter.Set(val);
    ++iter;
  }

  return out;
}

} // namespace itk::FrameDifferenceVideoFilterTest


/**
 * Main test
 */
int
itkFrameDifferenceVideoFilterTest(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  // Instantiate the filter
  using FilterType = itk::FrameDifferenceVideoFilter<InputVideoType, OutputVideoType>;
  auto filter = FilterType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(filter, FrameDifferenceVideoFilter, VideoToVideoFilter);


  // Set up an input VideoStream
  auto                    inputVideo = InputVideoType::New();
  constexpr SizeValueType numInputFrames = 50;
  inputVideo->SetNumberOfBuffers(numInputFrames);
  itk::TemporalRegion inputTempRegion;
  inputTempRegion.SetFrameStart(0);
  inputTempRegion.SetFrameDuration(numInputFrames);
  inputVideo->SetLargestPossibleTemporalRegion(inputTempRegion);
  inputVideo->SetRequestedTemporalRegion(inputTempRegion);
  inputVideo->SetBufferedTemporalRegion(inputTempRegion);
  for (SizeValueType i = 0; i < numInputFrames; ++i)
  {
    inputVideo->SetFrame(i, itk::FrameDifferenceVideoFilterTest::CreateInputFrame(i));
  }
  filter->SetInput(inputVideo);


  //////
  // Test filter with difference of adjacent frames (the default)
  //////

  filter->UpdateOutputInformation();

  // Make sure output largest possible temporal region is correct
  itk::TemporalRegion outputLargestTempRegion = filter->GetOutput()->GetLargestPossibleTemporalRegion();
  SizeValueType       outputStart = outputLargestTempRegion.GetFrameStart();
  SizeValueType       outputDuration = outputLargestTempRegion.GetFrameDuration();
  if (outputStart != 0)
  {
    std::cerr << "output's LargestPossibleTemporalRegion incorrect start. Got: " << outputStart << " Expected: 0"
              << std::endl;
    return EXIT_FAILURE;
  }
  if (outputDuration != numInputFrames - 1)
  {
    std::cerr << "output's LargestPossibleTemporalRegion incorrect duration. Got: " << outputDuration
              << " Expected: " << numInputFrames - 1 << std::endl;
    return EXIT_FAILURE;
  }

  // Go one frame at a time and check results
  OutputFrameType::IndexType checkPx;
  checkPx[0] = inputVideo->GetFrame(0)->GetLargestPossibleRegion().GetSize()[0] - 1;
  checkPx[1] = inputVideo->GetFrame(0)->GetLargestPossibleRegion().GetSize()[1] - 1;
  for (unsigned int i = outputStart; i < outputStart + outputDuration; ++i)
  {
    itk::TemporalRegion reqTempRegion;
    reqTempRegion.SetFrameStart(i);
    reqTempRegion.SetFrameDuration(1);
    filter->GetOutput()->SetRequestedTemporalRegion(reqTempRegion);
    filter->Update();

    // Check the results
    constexpr OutputPixelType expectedVal = 1;
    const OutputPixelType     actualVal = filter->GetOutput()->GetFrame(i)->GetPixel(checkPx);
    if (expectedVal != actualVal)
    {
      std::cerr << "Filter failed to compute frame " << i << " correctly for adjacent frames." << std::endl;
      std::cerr << "Expected Pixel Val: " << expectedVal << std::endl;
      std::cerr << "Actual Pixel Val: " << actualVal << std::endl;
      return EXIT_FAILURE;
    }
  }

  //////
  // Test filter with offset of 2
  //////

  // Reset the filter
  filter = FilterType::New();
  filter->SetInput(inputVideo);
  filter->SetFrameOffset(2);
  filter->UpdateOutputInformation();

  // Make sure output largest possible temporal region is correct
  outputLargestTempRegion = filter->GetOutput()->GetLargestPossibleTemporalRegion();
  outputStart = outputLargestTempRegion.GetFrameStart();
  outputDuration = outputLargestTempRegion.GetFrameDuration();
  if (outputStart != 0)
  {
    std::cerr << "output's LargestPossibleTemporalRegion incorrect start. Got: " << outputStart << " Expected: 0"
              << std::endl;
    return EXIT_FAILURE;
  }
  if (outputDuration != numInputFrames - 2)
  {
    std::cerr << "output's LargestPossibleTemporalRegion incorrect duration. Got: " << outputDuration
              << " Expected: " << numInputFrames - 2 << std::endl;
    return EXIT_FAILURE;
  }

  // Get all the frames at once and check results
  filter->Update();
  for (unsigned int i = outputStart; i < outputStart + outputDuration; ++i)
  {
    constexpr OutputPixelType expectedVal = 4; // Difference of 2 squared
    const OutputPixelType     actualVal = filter->GetOutput()->GetFrame(i)->GetPixel(checkPx);
    if (expectedVal != actualVal)
    {
      std::cerr << "Filter failed to compute frame " << i << " correctly with offset of 2." << std::endl;
      std::cerr << "Expected Pixel Val: " << expectedVal << std::endl;
      std::cerr << "Actual Pixel Val: " << actualVal << std::endl;
      return EXIT_FAILURE;
    }
  }

  //////
  // Return successfully
  //////
  return EXIT_SUCCESS;
}
