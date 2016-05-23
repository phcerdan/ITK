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

int itkVectorImageToImageListFilterTest(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  char *             infname   = argv[1];
  char *             outfname1 = argv[2];
  char *             outfname2 = argv[3];
  char *             outfname3 = argv[4];

  typedef unsigned char                          PixelType;
  typedef itk::Image<PixelType, Dimension>       ImageType;
  typedef itk::VectorImage<PixelType, Dimension> VectorImageType;
  typedef itk::ImageList<ImageType>              ImageListType;

  // IO
  typedef itk::ImageFileReader<VectorImageType> ReaderType;
  typedef itk::ImageFileWriter<ImageType>       WriterType;

  typedef itk::VectorImageToImageListFilter<VectorImageType, ImageListType> VectorImageToImageListFilterType;

  // Instantiating object
  VectorImageToImageListFilterType::Pointer filter = VectorImageToImageListFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  filter->SetInput(reader->GetOutput());

  filter->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname1);
  writer->SetInput(filter->GetOutput()->GetNthElement(0));
  writer->Update();

  writer = WriterType::New();
  writer->SetFileName(outfname2);
  writer->SetInput(filter->GetOutput()->GetNthElement(1));
  writer->Update();

  writer = WriterType::New();
  writer->SetFileName(outfname3);
  writer->SetInput(filter->GetOutput()->GetNthElement(2));
  writer->Update();

  return EXIT_SUCCESS;
}
