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
#include "itkPNGImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTestingMacros.h"

// Specific ImageIO test

int
itkPNGImageIOTest(int argc, char * argv[])
{
  // Test the reading of an image as RGB image and writing of RGB image

  if (argc < 5)
  {
    std::cerr << "Usage: " << itkNameOfTestExecutableMacro(argv) << " input"
              << " output"
              << " useCompression"
              << " compressionLevel"
              << " expandRGBPalette" << std::endl;
    return EXIT_FAILURE;
  }

  constexpr unsigned long Dimension = 2;
  using PixelType = unsigned char;

  // We are converting read data into RGB pixel image
  using RGBPixelType = itk::RGBPixel<PixelType>;
  using RGBImageType = itk::Image<RGBPixelType, Dimension>;


  // Read in the image
  const itk::PNGImageIO::Pointer io = itk::PNGImageIO::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(io, PNGImageIO, ImageIOBase);


  // Exercise exception cases
  const size_t sizeOfActualIORegion =
    io->GetIORegion().GetNumberOfPixels() * (io->GetComponentSize() * io->GetNumberOfComponents());
  auto * loadBuffer = new char[sizeOfActualIORegion];

  ITK_TRY_EXPECT_EXCEPTION(io->Read(loadBuffer));


  auto useCompression = static_cast<bool>(std::stoi(argv[3]));
  ITK_TEST_SET_GET_BOOLEAN(io, UseCompression, useCompression);

  const int compressionLevel = std::stoi(argv[4]);
  io->SetCompressionLevel(compressionLevel);
  ITK_TEST_SET_GET_VALUE(compressionLevel, io->GetCompressionLevel());

  auto expandRGBPalette = static_cast<bool>(argv[5]);
  ITK_TEST_SET_GET_BOOLEAN(io, ExpandRGBPalette, expandRGBPalette);

  if (io->CanReadFile(""))
  {
    std::cerr << "Test failed!" << std::endl;
    std::cout << "No filename specified." << std::endl;
    std::cout << "CanReadFile: "
              << "Expected false but got true" << std::endl;
    return EXIT_FAILURE;
  }

  if (!io->SupportsDimension(Dimension))
  {
    std::cerr << "Test failed!" << std::endl;
    std::cerr << "itk::PNGImageIO does not support dimension: " << Dimension << std::endl;
    return EXIT_FAILURE;
  }

  if (io->CanStreamRead())
  {
    std::cout << "itk::PNGImageIO can stream read" << std::endl;
  }
  else
  {
    std::cout << "itk::PNGImageIO cannot stream read" << std::endl;
  }


  if (!io->CanReadFile(argv[1]))
  {
    std::cerr << "Test failed!" << std::endl;
    std::cout << "itk::PNGImageIO cannot read file " << io->GetFileName() << std::endl;
    return EXIT_FAILURE;
  }
  const itk::ImageFileReader<RGBImageType>::Pointer reader = itk::ImageFileReader<RGBImageType>::New();
  reader->SetFileName(argv[1]);
  reader->SetImageIO(io);
  ITK_TRY_EXPECT_NO_EXCEPTION(reader->Update());

  if (io->GetExpandRGBPalette())
  {
    std::cout << "If palette image, expanding to RGB. " << std::endl;
  }
  else
  {
    std::cout << "If palette image, trying to read as scalar. " << std::endl;
  }

  if (!io->GetExpandRGBPalette() && io->GetIsReadAsScalarPlusPalette())
  {
    std::cout << "Image read as Scalar." << std::endl;
    itk::PNGImageIO::PaletteType palette = io->GetColorPalette();
    std::cout << "Palette: " << std::endl;
    for (unsigned int i = 0; i < palette.size(); ++i)
    {
      std::cout << '[' << i << "]:" << palette[i] << std::endl;
    }
  }
  else
  {
    std::cout << "Image read as RGB." << std::endl;
  }

  // Try writing
  const itk::ImageFileWriter<RGBImageType>::Pointer writer = itk::ImageFileWriter<RGBImageType>::New();
  writer->SetInput(reader->GetOutput());
  writer->SetFileName(argv[2]);
  writer->SetImageIO(io);

  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Write());


  // Release memory
  delete[] loadBuffer;

  // Exercise other methods
  const itk::ImageIOBase::SizeType pixelStride = io->GetPixelStride();
  std::cout << "PixelStride: " << static_cast<itk::NumericTraits<itk::ImageIOBase::SizeType>::PrintType>(pixelStride)
            << std::endl;

  //
  // Try writing out several kinds of images using png.
  // The images to test are as follows:
  // - 3D non-degenerate volume: this covers all images greater than or
  // equal to 3D. The writer should write out the first slice.
  // - 3D degenerate volume: The writer should write out the first
  // slice.
  // - 2D image: The writer should write it out correctly.
  // - 2D degenerate image: The writer should write out the image.
  // - 1D image: The writer should write it out as a 2D image.
  //

  using ImageType3D = itk::Image<unsigned short, 3>;
  using ImageType2D = itk::Image<unsigned short, 2>;
  using ImageType1D = itk::Image<unsigned short, 1>;

  //
  // 3D non-degenerate volume
  //

  auto volume = ImageType3D::New();

  auto                             size3D = ImageType3D::SizeType::Filled(10);
  constexpr ImageType3D::IndexType start3D{};
  ImageType3D::RegionType          region3D{ start3D, size3D };

  volume->SetRegions(region3D);
  volume->Allocate();
  volume->FillBuffer(0);

  using WriterType3D = itk::ImageFileWriter<ImageType3D>;
  auto writer3D = WriterType3D::New();
  writer3D->SetFileName(argv[2]);
  writer3D->SetImageIO(io);

  writer3D->SetInput(volume);

  ITK_TRY_EXPECT_NO_EXCEPTION(writer3D->Update());


  //
  // 3D degenerate volume
  //

  auto degenerateVolume = ImageType3D::New();
  // Collapse the first dimension.
  size3D[0] = 1;
  region3D.SetSize(size3D);
  degenerateVolume->SetRegions(region3D);
  degenerateVolume->Allocate();
  degenerateVolume->FillBuffer(0);

  writer3D->SetFileName(argv[2]);
  writer3D->SetImageIO(io);
  writer3D->SetInput(degenerateVolume);

  ITK_TRY_EXPECT_NO_EXCEPTION(writer3D->Update());


  //
  // 2D non-degenerate volume
  //
  auto image = ImageType2D::New();

  auto                             size2D = ImageType2D::SizeType::Filled(10);
  constexpr ImageType2D::IndexType start2D{};
  ImageType2D::RegionType          region2D{ start2D, size2D };

  image->SetRegions(region2D);
  image->Allocate();
  image->FillBuffer(0);

  using WriterType2D = itk::ImageFileWriter<ImageType2D>;
  auto writer2D = WriterType2D::New();

  writer2D->SetFileName(argv[2]);
  writer2D->SetImageIO(io);
  writer2D->SetInput(image);

  ITK_TRY_EXPECT_NO_EXCEPTION(writer2D->Update());


  //
  // 2D degenerate volume
  //
  auto degenerateImage = ImageType2D::New();

  // Collapse the first dimension
  size2D[0] = 1;
  region2D.SetSize(size2D);
  degenerateImage->SetRegions(region2D);
  degenerateImage->Allocate();
  degenerateImage->FillBuffer(0);

  writer2D->SetFileName(argv[2]);
  writer2D->SetImageIO(io);
  writer2D->SetInput(degenerateImage);

  ITK_TRY_EXPECT_NO_EXCEPTION(writer2D->Update());

  //
  // 1D image
  //
  auto line = ImageType1D::New();

  auto                             size1D = ImageType1D::SizeType::Filled(10);
  constexpr ImageType1D::IndexType start1D{};
  const ImageType1D::RegionType    region1D{ start1D, size1D };
  line->SetRegions(region1D);

  line->Allocate();
  line->FillBuffer(0);

  using WriterType1D = itk::ImageFileWriter<ImageType1D>;
  auto writer1D = WriterType1D::New();
  writer1D->SetFileName(argv[2]);
  writer1D->SetImageIO(io);
  writer1D->SetInput(line);

  ITK_TRY_EXPECT_NO_EXCEPTION(writer1D->Update());


  std::cout << "Test finished" << std::endl;
  return EXIT_SUCCESS;
}
