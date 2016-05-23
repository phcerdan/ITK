/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Mines-Telecom. All rights reserved.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkImage.h"
#include "itkWaveletOperator.h"
#include "itkWaveletFilterBank.h"
#include "itkWaveletTransform.h"

int itkWaveletInverseTransformNewTest(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const int Dimension = 2;
  typedef double                           PixelType;
  typedef itk::Image<PixelType, Dimension> ImageType;

  /* Wavelet choice */
  const itk::Wavelet::Wavelet wvltID = itk::Wavelet::SYMLET8;

  /* Inverse Transformation */
  typedef itk::WaveletOperator<wvltID, itk::Wavelet::INVERSE, PixelType, Dimension>
  InverseWaveletOperator;
  typedef itk::WaveletFilterBank<ImageType, ImageType, InverseWaveletOperator, itk::Wavelet::INVERSE>
  InverseFilterBank;
  typedef itk::WaveletTransform<ImageType, ImageType, InverseFilterBank, itk::Wavelet::INVERSE>
  InvFilterType;

  InvFilterType::Pointer invFilter = InvFilterType::New();

  std::cout << invFilter << std::endl;

  return EXIT_SUCCESS;
}
