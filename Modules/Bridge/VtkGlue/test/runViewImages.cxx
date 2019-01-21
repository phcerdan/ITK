/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include <string>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkViewImages.h"

/**
 * App to run ViewImage in an image file.
 *
 * This file is added to the TestDriver but it is not a regression test (no add_test).
 * It can be used for developers to fast visualize images with:
 *
 * ITK-build/bin/ITKVtkGlueTestDriver runViewImage /path/to/image.xxx \
 * [title] [win_width] [win_height]
 *
 * \sa itk::ViewImage
 */
int
runViewImages(int argc, char* argv[])
{
  if ( argc < 2 )
    {
    std::cerr << "Usage: " << argv[0] << " inputImage [title] [win_size_x win_size_y] " << std::endl;
    return EXIT_FAILURE;
    }
  // Defaults
  std::string winTitle = "itkViewImages";
  size_t winWidth = 1200;
  size_t winHeight = 800;
  std::vector<std::string> inputImages;
  inputImages.push_back(argv[1]);
  if ( argc >= 3 )
    for (int i = 2; i < argc; ++i)
      inputImages.push_back(argv[i]);

  constexpr unsigned int dimension = 3;
  using PixelType = float;
  using ImageType = itk::Image< PixelType, dimension >;
  using ReaderType = itk::ImageFileReader< ImageType >;
  itk::ViewImages<float> viewer;
  for (unsigned int i = 0; i < inputImages.size(); ++i)
    {
    auto reader = ReaderType::New();
    reader->SetFileName(inputImages[i]);
    reader->Update();

    viewer.Add(reader->GetOutput(), inputImages[i]);
    }
  viewer.View(winTitle, winWidth, winHeight);

  return EXIT_SUCCESS;
}
