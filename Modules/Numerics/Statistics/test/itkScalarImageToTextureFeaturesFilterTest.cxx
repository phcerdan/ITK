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

// Insight classes
#include "itkImageRegionIterator.h"

#include "itkScalarImageToTextureFeaturesFilter.h"
#include "itkDenseFrequencyContainer2.h"

int
itkScalarImageToTextureFeaturesFilterTest(int, char *[])
{

  // Data definitions
  constexpr unsigned int IMGWIDTH = 5;
  constexpr unsigned int IMGHEIGHT = 5;
  constexpr unsigned int NDIMENSION = 2;


  //------------------------------------------------------
  // Create a simple test images
  //------------------------------------------------------
  using InputImageType = itk::Image<unsigned char, NDIMENSION>;
  using MaskImageType = itk::Image<bool, NDIMENSION>;

  using InputImageIterator = itk::ImageRegionIterator<InputImageType>;
  using MaskImageIterator = itk::ImageRegionIterator<MaskImageType>;


  auto image = InputImageType::New();

  constexpr InputImageType::SizeType inputImageSize = { { IMGWIDTH, IMGHEIGHT } };

  constexpr InputImageType::IndexType index{};
  InputImageType::RegionType          region;

  region.SetSize(inputImageSize);
  region.SetIndex(index);

  //--------------------------------------------------------------------------
  // Set up the image first. It looks like:
  //  1 2 1 2 1
  //  1 2 1 2 1
  //  1 2 1 2 1
  //  1 2 1 2 1
  //  1 2 1 2 1
  //--------------------------------------------------------------------------

  image->SetRegions(region);
  image->Allocate();

  // setup the iterator
  InputImageIterator imageIt(image, image->GetBufferedRegion());

  for (int i = 0; i < 5; ++i)
    for (int j = 0; j < 5; j++, ++imageIt)
    {
      imageIt.Set(j % 2 + 1);
    }

  //--------------------------------------------------------------------------
  // Set up the mask next. It looks like:
  //  1 1 1 1 1
  //  1 1 1 1 1
  //  1 1 1 1 1
  //  1 1 1 1 1
  //  1 1 1 1 1
  //--------------------------------------------------------------------------

  auto mask = MaskImageType::New();
  mask->SetRegions(region);
  mask->Allocate();

  // setup the iterator
  MaskImageIterator maskIt(mask, mask->GetBufferedRegion());
  maskIt.GoToBegin();
  for (int i = 0; i < 5; ++i)
    for (int j = 0; j < 5; j++, ++maskIt)
    {
      maskIt.Set(true);
    }

  //--------------------------------------------------------------------------
  // Test the texFilter
  //--------------------------------------------------------------------------
  try
  {

    using TextureFilterType = itk::Statistics::
      ScalarImageToTextureFeaturesFilter<InputImageType, itk::Statistics::DenseFrequencyContainer2, MaskImageType>;

    // First test: just use the defaults.
    auto texFilter = TextureFilterType::New();
    bool passed = true;
    // Invoke update before adding an input. An exception should be
    // thrown.
    try
    {
      texFilter->Update();
      passed = false;
      std::cerr << "Failed to throw expected exception due to nullptr input: " << std::endl;
      return EXIT_FAILURE;
    }
    catch (const itk::ExceptionObject & excp)
    {
      std::cout << "Expected exception caught: " << excp << std::endl;
    }

    texFilter->ResetPipeline();

    if (texFilter->GetInput() != nullptr)
    {
      std::cerr << "GetInput() should return nullptr since the input is not set yet " << std::endl;
      passed = false;
    }

    if (texFilter->GetMaskImage() != nullptr)
    {
      std::cerr << "GetMaskImage() should return nullptr since the mask image is not set yet " << std::endl;
      passed = false;
    }

    // Invoke update with a nullptr input. An exception should be
    // thrown.
    texFilter->SetInput(nullptr);
    try
    {
      texFilter->Update();
      passed = false;
      std::cerr << "Failed to throw expected exception due to nullptr input: " << std::endl;
      return EXIT_FAILURE;
    }
    catch (const itk::ExceptionObject & excp)
    {
      std::cout << "Expected exception caught: " << excp << std::endl;
    }

    texFilter->ResetPipeline();

    if (texFilter->GetInput() != nullptr)
    {
      passed = false;
    }

    // Test the Use_PixelContainer boolean
    texFilter->SetFastCalculations(false);
    if (texFilter->GetFastCalculations() != false)
    {
      std::cerr << "Error in Set/Get FastCalculations methods" << std::endl;
      return EXIT_FAILURE;
    }

    texFilter->FastCalculationsOn();
    if (texFilter->GetFastCalculations() != true)
    {
      std::cerr << "Error in Set/Get FastCalculationsOn method" << std::endl;
      return EXIT_FAILURE;
    }

    texFilter->FastCalculationsOff();
    texFilter->SetInput(image);
    texFilter->SetMaskImage(mask);
    texFilter->Update();

    texFilter->Print(std::cout);

    // Test GetInput
    if (texFilter->GetInput() != image)
    {
      std::cerr << "Error in GetInput() method " << std::endl;
      passed = false;
    }

    // Test GetMaskImage
    if (texFilter->GetMaskImage() != mask)
    {
      std::cerr << "Error in GetMaskImage() method " << std::endl;
      passed = false;
    }

    TextureFilterType::FeatureValueVectorPointer means = texFilter->GetFeatureMeans();
    TextureFilterType::FeatureValueVectorPointer stds = texFilter->GetFeatureStandardDeviations();

    constexpr double expectedMeans[6] = { 0.505, 0.992738, 0.625, 0.75, 0.0959999, 0.2688 };
    constexpr double expectedDeviations[6] = { 0.00866027, 0.0125788, 0.216506351, 0.433012702, 0.166277, 0.465575 };


    {
      TextureFilterType::FeatureValueVector::ConstIterator mIt = means->Begin();
      for (int counter = 0; mIt != means->End(); ++mIt, counter++)
      {
        if (itk::Math::abs(expectedMeans[counter] - mIt.Value()) > 0.0001)
        {
          std::cerr << "Error. Mean for feature " << counter << " is " << mIt.Value() << ", expected "
                    << expectedMeans[counter] << '.' << std::endl;
          passed = false;
        }
      }
    }

    {
      TextureFilterType::FeatureValueVector::ConstIterator sIt = stds->Begin();
      for (int counter = 0; sIt != stds->End(); ++sIt, counter++)
      {
        if (itk::Math::abs(expectedDeviations[counter] - sIt.Value()) > 0.0001)
        {
          std::cerr << "Error. Deviation for feature " << counter << " is " << sIt.Value() << ", expected "
                    << expectedDeviations[counter] << '.' << std::endl;
          passed = false;
        }
      }
    }

    // Rerun the feature generation with fast calculations on
    texFilter->FastCalculationsOn();
    texFilter->Update();
    means = texFilter->GetFeatureMeans();
    stds = texFilter->GetFeatureStandardDeviations();

    constexpr double expectedMeans2[6] = { 0.5, 1.0, 0.5, 1.0, 0.0, 0.0 };
    constexpr double expectedDeviations2[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

    {
      TextureFilterType::FeatureValueVector::ConstIterator mIt = means->Begin();
      for (int counter = 0; mIt != means->End(); ++mIt, counter++)
      {
        if (itk::Math::abs(expectedMeans2[counter] - mIt.Value()) > 0.0001)
        {
          std::cerr << "Error. Mean for feature " << counter << " is " << mIt.Value() << ", expected "
                    << expectedMeans2[counter] << '.' << std::endl;
          passed = false;
        }
      }
    }

    {
      TextureFilterType::FeatureValueVector::ConstIterator sIt = stds->Begin();
      for (int counter = 0; sIt != stds->End(); ++sIt, counter++)
      {
        if (itk::Math::abs(expectedDeviations2[counter] - sIt.Value()) > 0.0001)
        {
          std::cerr << "Error. Deviation for feature " << counter << " is " << sIt.Value() << ", expected "
                    << expectedDeviations2[counter] << '.' << std::endl;
          passed = false;
        }
      }
    }

    // Rerun the feature generation setting an offset
    TextureFilterType::OffsetType offset;
    offset[0] = -1;
    offset[1] = -1;

    TextureFilterType::OffsetVectorPointer offsets;

    offsets = TextureFilterType::OffsetVector::New();

    offsets->push_back(offset);
    texFilter->SetOffsets(offsets);

    const TextureFilterType::OffsetVector * offsets2 = texFilter->GetOffsets();

    for (TextureFilterType::OffsetVector::ConstIterator vIt = offsets->Begin(), vIt2 = offsets2->Begin();
         vIt != offsets->End();
         ++vIt, ++vIt2)
    {
      if (vIt.Value() != vIt2.Value())
      {
        std::cerr << "Offsets not properly set" << std::endl;
        passed = false;
      }
    }

    texFilter->Update();

    means = texFilter->GetFeatureMeans();
    stds = texFilter->GetFeatureStandardDeviations();

    constexpr double expectedMeans3[6] = { 0.5, 1.0, 0.5, 1.0, 0.0, 0.0 };
    constexpr double expectedDeviations3[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

    {
      TextureFilterType::FeatureValueVector::ConstIterator mIt = means->Begin();
      for (int counter = 0; mIt != means->End(); ++mIt, counter++)
      {
        if (itk::Math::abs(expectedMeans3[counter] - mIt.Value()) > 0.0001)
        {
          std::cerr << "Error. Mean for feature " << counter << " is " << mIt.Value() << ", expected "
                    << expectedMeans3[counter] << '.' << std::endl;
          passed = false;
        }
      }
    }

    {
      TextureFilterType::FeatureValueVector::ConstIterator sIt = stds->Begin();
      for (int counter = 0; sIt != stds->End(); ++sIt, counter++)
      {
        if (itk::Math::abs(expectedDeviations3[counter] - sIt.Value()) > 0.0001)
        {
          std::cerr << "Error. Deviation for feature " << counter << " is " << sIt.Value() << ", expected "
                    << expectedDeviations3[counter] << '.' << std::endl;
          passed = false;
        }
      }
    }

    // Test Set/Get Requested features
    const TextureFilterType::FeatureNameVectorPointer requestedFeatures = TextureFilterType::FeatureNameVector::New();

    requestedFeatures->push_back(
      static_cast<uint8_t>(itk::Statistics::HistogramToTextureFeaturesFilterEnums::TextureFeature::Inertia));
    requestedFeatures->push_back(
      static_cast<uint8_t>(itk::Statistics::HistogramToTextureFeaturesFilterEnums::TextureFeature::ClusterShade));
    texFilter->SetRequestedFeatures(requestedFeatures);

    const TextureFilterType::FeatureNameVector * requestedFeatures2 = texFilter->GetRequestedFeatures();

    TextureFilterType::FeatureNameVector::ConstIterator fIt;

    fIt = requestedFeatures2->Begin();
    if (fIt.Value() !=
        static_cast<uint8_t>(itk::Statistics::HistogramToTextureFeaturesFilterEnums::TextureFeature::Inertia))
    {
      std::cerr << "Requested feature name not correctly set" << std::endl;
      passed = false;
    }
    ++fIt;

    if (fIt.Value() !=
        static_cast<uint8_t>(itk::Statistics::HistogramToTextureFeaturesFilterEnums::TextureFeature::ClusterShade))
    {
      std::cerr << "Requested feature name not correctly set" << std::endl;
      passed = false;
    }

    if (!passed)
    {
      std::cerr << "Test failed" << std::endl;
      return EXIT_FAILURE;
    }

    std::cerr << "Test succeeded" << std::endl;
    return EXIT_SUCCESS;
  }
  catch (const itk::ExceptionObject & err)
  {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    std::cerr << "Test failed" << std::endl;
    return EXIT_FAILURE;
  }
}
