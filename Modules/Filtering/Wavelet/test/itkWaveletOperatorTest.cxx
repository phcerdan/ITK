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

#include "itkWaveletOperator.h"

int itkWaveletOperatorTest(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const itk::Wavelet::WaveletDirection directionID = itk::Wavelet::FORWARD;
  const itk::Wavelet::Wavelet          wvltID      = itk::Wavelet::HAAR;

  const int Dimension = 2;
  typedef double PixelType;

  typedef itk::WaveletOperator<wvltID, directionID, PixelType, Dimension>
                                            WaveletOperator;
  typedef WaveletOperator::LowPassOperator  LowPassOperatorType;
  typedef WaveletOperator::HighPassOperator HighPassOperatorType;

  LowPassOperatorType h_filter;
  h_filter.SetDirection(0);
  h_filter.SetUpSampleFactor(2);
  h_filter.CreateDirectional();

  HighPassOperatorType g_filter;
  g_filter.SetDirection(0);
  g_filter.SetUpSampleFactor(2);
  g_filter.CreateDirectional();

  return EXIT_SUCCESS;
}
