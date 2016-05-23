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

int itkVectorImageToImageListFilterNewTest(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;

  typedef unsigned char                          PixelType;
  typedef itk::Image<PixelType, Dimension>       ImageType;
  typedef itk::VectorImage<PixelType, Dimension> VectorImageType;
  typedef itk::ImageList<ImageType>              ImageListType;

  typedef itk::VectorImageToImageListFilter<VectorImageType, ImageListType> VectorImageToImageListFilterType;

  // Instantiating object
  VectorImageToImageListFilterType::Pointer filter = VectorImageToImageListFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
