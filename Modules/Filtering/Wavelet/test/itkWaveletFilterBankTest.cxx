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
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkWaveletOperator.h"
#include "itkWaveletFilterBank.h"

int itkWaveletFilterBankTest(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFileName = argv[1];
  const unsigned int decimFactor = atoi(argv[2]); // 1 for multiscale, 2 for multiresolution
  const char *       outputFileName = argv[3];

  const int Dimension = 2;
  typedef float                            PixelType;
  typedef itk::Image<PixelType, Dimension> ImageType;
  typedef itk::ImageFileReader<ImageType>  ReaderType;

  /* Reading */
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  /* Wavelet choice */
  //const itk::Wavelet::Wavelet wvltID = itk::Wavelet::HAAR;
  const itk::Wavelet::Wavelet wvltID = itk::Wavelet::SYMLET8;

  /* Forward Transformation */
  typedef itk::WaveletOperator<wvltID, itk::Wavelet::FORWARD, PixelType, Dimension> WaveletOperator;
  typedef itk::WaveletFilterBank<ImageType, ImageType, WaveletOperator, itk::Wavelet::FORWARD>
  FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetSubsampleImageFactor(decimFactor);

  /* Inverse Transformation */
  typedef itk::WaveletOperator<wvltID, itk::Wavelet::INVERSE, PixelType, Dimension> InvWaveletOperator;
  typedef itk::WaveletFilterBank<ImageType, ImageType, InvWaveletOperator, itk::Wavelet::INVERSE>
  InvFilterType;
  InvFilterType::Pointer invFilter = InvFilterType::New();
  for (unsigned int i = 0; i < filter->GetNumberOfOutputs(); ++i)
    invFilter->SetInput(i, filter->GetOutput(i));
  invFilter->SetSubsampleImageFactor(filter->GetSubsampleImageFactor());

  /* Saving output */
  typedef itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(invFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
