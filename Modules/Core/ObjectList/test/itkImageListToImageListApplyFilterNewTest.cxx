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

#include "itkImageListToImageListApplyFilter.h"
#include "itkImageList.h"
#include "itkImage.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkMeanImageFilter.h"

int itkImageListToImageListApplyFilterNewTest(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef unsigned char                              PixelType;
  typedef itk::Image<PixelType, Dimension>           ImageType;
  typedef itk::ImageList<ImageType>                  ImageListType;
  typedef itk::MeanImageFilter<ImageType, ImageType> MeanFilterType;

  typedef itk::ImageListToImageListApplyFilter<ImageListType, ImageListType, MeanFilterType>
  ImageListToImageListApplyFilterType;

  // Instantiating object
  ImageListToImageListApplyFilterType::Pointer filter = ImageListToImageListApplyFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
