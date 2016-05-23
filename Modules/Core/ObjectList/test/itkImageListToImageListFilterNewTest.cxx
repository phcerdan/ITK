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

#include "itkImageListToImageListFilter.h"
#include "itkImage.h"

int itkImageListToImageListFilterNewTest(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef unsigned char                                                    InputPixelType;
  typedef unsigned char                                                    OutputPixelType;
  typedef itk::Image<InputPixelType, Dimension>                            InputImageType;
  typedef itk::Image<OutputPixelType, Dimension>                           OutputImageType;
  typedef itk::ImageListToImageListFilter<InputImageType, OutputImageType> ImageToImageListFilterType;

  // Instantiating ImageListSource object
  ImageToImageListFilterType::Pointer imageList = ImageToImageListFilterType::New();

  std::cout << imageList << std::endl;

  return EXIT_SUCCESS;
}
