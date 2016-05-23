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

#include "itkImage.h"
#include "itkWaveletOperator.h"
#include "itkWaveletFilterBank.h"
#include "itkWaveletTransform.h"

int itkWaveletTransformNewTest(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const int Dimension = 2;
  typedef double                           PixelType;
  typedef itk::Image<PixelType, Dimension> ImageType;

  /* Wavelet choice */
  const itk::Wavelet::Wavelet wvltID = itk::Wavelet::SYMLET8;

  /* Forward Transformation */
  typedef itk::WaveletOperator<wvltID, itk::Wavelet::FORWARD, PixelType, Dimension> WaveletOperator;
  typedef itk::WaveletFilterBank<ImageType, ImageType, WaveletOperator, itk::Wavelet::FORWARD>
  ForwardFilterBank;
  typedef itk::WaveletTransform<ImageType, ImageType, ForwardFilterBank, itk::Wavelet::FORWARD>
  FilterType;

  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
