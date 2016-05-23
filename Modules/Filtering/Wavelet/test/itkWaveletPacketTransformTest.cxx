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
#include "itkWaveletPacketTransform.h"

template<itk::Wavelet::Wavelet TWavelet>
int itkWaveletPacketTransform_generic(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFileName = argv[1];
  const char *       outputFileName = argv[2];
  const unsigned int level = atoi(argv[3]);
  const unsigned int decimFactor = atoi(argv[4]);

  const int Dimension = 2;
  typedef float                            PixelType;
  typedef itk::Image<PixelType, Dimension> ImageType;
  typedef itk::ImageFileReader<ImageType>  ReaderType;

  /* Reading */
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  /* Wavelet choice */
  const itk::Wavelet::Wavelet wvltID = TWavelet;

  /* Wavelet packet configuration */
  typedef itk::FullyDecomposedWaveletPacketCost<ImageType> CostType;
  typename CostType::Pointer cost = CostType::New();
  cost->SetNumberOfAllowedDecompositions(level);

  /* Forward Transformation */
  typedef itk::WaveletOperator<wvltID, itk::Wavelet::FORWARD, PixelType, Dimension>            WaveletOperator;
  typedef itk::WaveletFilterBank<ImageType, ImageType, WaveletOperator, itk::Wavelet::FORWARD> ForwardFilterBank;
  typedef itk::WaveletPacketTransform<ImageType, ImageType, ForwardFilterBank,
                                      itk::Wavelet::FORWARD, CostType>                         FilterType;

  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetCost(cost);
  filter->SetSubsampleImageFactor(decimFactor);
  filter->Update();

  /* Inverse Transformation */
  typedef itk::WaveletOperator<wvltID, itk::Wavelet::INVERSE, PixelType, Dimension>               InvWaveletOperator;
  typedef itk::WaveletFilterBank<ImageType, ImageType, InvWaveletOperator, itk::Wavelet::INVERSE> InvFilterBank;
  typedef itk::WaveletPacketTransform<ImageType, ImageType, InvFilterBank, itk::Wavelet::INVERSE> InvFilterType;

  typename InvFilterType::Pointer invFilter = InvFilterType::New();
  invFilter->SetInput(filter->GetOutput());
  invFilter->SetWaveletPacketRule(filter->GetWaveletPacketRule());
  invFilter->SetSubsampleImageFactor(decimFactor);
  invFilter->Update();

  /* Writing the output */
  typedef itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(invFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

int itkWaveletPacketTransformTest(int argc, char * argv[])
{
  if (argc != 6)
    {
    std::cerr << "Usage: " << argv[0]
              << "<InputImage> <OutputImage> <level> <decimFactor> <waveletType>" << std::endl;
    return EXIT_FAILURE;
    }
  int waveletType = atoi(argv[5]);

  switch (waveletType)
    {
    case 0:
      return itkWaveletPacketTransform_generic<itk::Wavelet::HAAR> (argc, argv);
      break;
    case 1:
      return itkWaveletPacketTransform_generic<itk::Wavelet::DB4> (argc, argv);
      break;
    case 2:
      return itkWaveletPacketTransform_generic<itk::Wavelet::DB6> (argc, argv);
      break;
    case 3:
      return itkWaveletPacketTransform_generic<itk::Wavelet::DB8> (argc, argv);
      break;
    case 4:
      return itkWaveletPacketTransform_generic<itk::Wavelet::DB12> (argc, argv);
      break;
    case 5:
      return itkWaveletPacketTransform_generic<itk::Wavelet::DB20> (argc, argv);
      break;
    case 6:
      return itkWaveletPacketTransform_generic<itk::Wavelet::SPLINE_BIORTHOGONAL_2_4> (argc, argv);
      break;
    case 7:
      return itkWaveletPacketTransform_generic<itk::Wavelet::SPLINE_BIORTHOGONAL_4_4> (argc, argv);
      break;
    case 8:
      return itkWaveletPacketTransform_generic<itk::Wavelet::SYMLET8> (argc, argv);
      break;
    default:
      std::cerr << "No more wavelet available\n";
      return EXIT_FAILURE;
    }
}
