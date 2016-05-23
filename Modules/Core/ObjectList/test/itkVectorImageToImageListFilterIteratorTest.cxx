/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkMacro.h"

#include "itkVectorImageToImageListFilter.h"
#include "itkVectorImage.h"
#include "itkImageList.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int itkVectorImageToImageListFilterIteratorTest(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  const char *       infname   = argv[1];
  const char *       outputFilenamePrefix = argv[2];
  const char *       outputFilenameSuffix = argv[3];

  typedef unsigned char                          PixelType;
  typedef itk::Image<PixelType, Dimension>       ImageType;
  typedef itk::VectorImage<PixelType, Dimension> VectorImageType;
  typedef itk::ImageList<ImageType>              ImageListType;

  // IO
  typedef itk::ImageFileReader<VectorImageType> ReaderType;
  typedef itk::ImageFileWriter<ImageType>       WriterType;

  typedef itk::VectorImageToImageListFilter<VectorImageType, ImageListType> VectorImageToImageListFilterType;

  typedef VectorImageToImageListFilterType::OutputImageListType::Iterator ImageListIterator;

  // Instantiating object

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  VectorImageToImageListFilterType::Pointer filter = VectorImageToImageListFilterType::New();
  filter->SetInput(reader->GetOutput());
  // We need to do that to fill the Imagelist before update and use it as input of other filter
  filter->UpdateOutputInformation();

  ImageListIterator itOutput = filter->GetOutput()->Begin();

  WriterType::Pointer writer = WriterType::New();

  int i = 1;

  while ((itOutput != filter->GetOutput()->End()) )
    {
    std::stringstream oss;
    oss << outputFilenamePrefix << "Band" << i << "." << outputFilenameSuffix;

    writer->SetInput(itOutput.Get());

    writer->SetFileName(oss.str().c_str());
    // TODO otbFileWriter method, missing in ITK, important?
    // writer->SetNumberOfDivisionsTiledStreaming(8);
    writer->Update();

    ++itOutput;
    ++i;
    }

  /*
  while ((itOutput != filter->GetOutput()->End()) )
    {
    std::stringstream oss;
    WriterType::Pointer writer = WriterType::New();

    oss << outputFilenamePrefix << "Band" << i << "." << outputFilenameSuffix;

    writer->SetInput(itOutput.Get());

    writer->SetFileName(oss.str().c_str());

    writer->Update();

    ++itOutput;
    ++i;
    }
    */
  return EXIT_SUCCESS;
}
