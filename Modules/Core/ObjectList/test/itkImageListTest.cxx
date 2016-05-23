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

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageList.h"
#include "itkImage.h"

int itkImageListTest(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFilename = argv[1];
  const char *       outputFilename = argv[2];
  const unsigned int Dimension = 2;

  typedef unsigned char                         InputPixelType;
  typedef itk::Image<InputPixelType, Dimension> InputImageType;
  typedef itk::ImageFileReader<InputImageType>  ReaderType;
  typedef itk::ImageFileWriter<InputImageType>  WriterType;
  typedef itk::ImageList<InputImageType>        ImageListType;

  // Reading image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  // Instantiating ImageList object
  ImageListType::Pointer imageList = ImageListType::New();

  // Appending one image to the list
  imageList->PushBack(reader->GetOutput());

  // Getting the image from the list and writing it to file
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(imageList->Back());
  writer->Update();

  return EXIT_SUCCESS;
}
