/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef itkWaveletInverseImageFilter_h
#define itkWaveletInverseImageFilter_h

#include "itkWaveletOperator.h"
#include "itkWaveletFilterBank.h"
#include "itkWaveletTransform.h"
#include "itkWaveletsSynopsisImageToWaveletsBandsListFilter.h"

namespace itk {

/**
 * \class WaveletInverseImageFilter
 * This filter performs wavelet inverse transform.
 * It takes three template arguments: Input, Output and WaveletOperator
 * This filter is not streamed.
 * \ingroup ITKWavelet
 * \sa WaveletImageFilter
 * \sa WaveletsSynopsisImageToWaveletsBandsListFilter
 * \sa WaveletGenerator
 */
template < class TInputImage,
           class TOutputImage,
           Wavelet::Wavelet TMotherWaveletOperator >
class WaveletInverseImageFilter :
  public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef TInputImage                                                                InputImageType;
  typedef TOutputImage                                                               OutputImageType;

  itkStaticConstMacro( ImageDimension,            unsigned int, InputImageType::ImageDimension );
  itkStaticConstMacro( MotherWaveletOperator,     short,        TMotherWaveletOperator );
  itkStaticConstMacro( DirectionOfTransformation, short ,       itk::Wavelet::FORWARD );

  typedef WaveletInverseImageFilter<InputImageType,OutputImageType, TMotherWaveletOperator>  Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType>                    Superclass;
  typedef itk::SmartPointer<Self>                                                     Pointer;
  typedef itk::SmartPointer<const Self>                                               ConstPointer;

  typedef typename InputImageType::PixelType                                          InputPixelType;
  typedef typename OutputImageType::PixelType                                         OutputPixelType;

  typedef itk::WaveletOperator< TMotherWaveletOperator, itk::Wavelet::INVERSE, OutputPixelType, ImageDimension >
    WaveletOperatorType;
  typedef itk::WaveletFilterBank< OutputImageType, OutputImageType, WaveletOperatorType, itk::Wavelet::INVERSE >
    FilterBankType;
  typedef itk::WaveletTransform< OutputImageType, OutputImageType, FilterBankType, itk::Wavelet::INVERSE >
    WaveletInverseTransformFilterType;
  typedef typename WaveletInverseTransformFilterType::Pointer
    WaveletInverseTransformFilterPointerType;

  typedef itk::WaveletsSynopsisImageToWaveletsBandsListFilter<InputImageType, typename WaveletInverseTransformFilterType::InputImageListType>
    WaveletsSynopsisImageToWaveletsBandsListFilterType;
  typedef typename WaveletsSynopsisImageToWaveletsBandsListFilterType::Pointer
    WaveletsSynopsisImageToWaveletsBandsListFilterPointerType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(WaveletInverseImageFilter, ImageToImageFilter);

  itkGetMacro(NumberOfDecompositions,unsigned int);
  itkSetMacro(NumberOfDecompositions,unsigned int);

  /** If the filter is modified, the internal filters need to be modified too */
  virtual void Modified() const;

protected:
  WaveletInverseImageFilter();
  virtual ~WaveletInverseImageFilter();

  virtual void GenerateInputRequestedRegion();

  virtual void GenerateData();

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  WaveletInverseImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  WaveletsSynopsisImageToWaveletsBandsListFilterPointerType m_SynopsisImageToWaveletsBandsList;
  WaveletInverseTransformFilterPointerType m_WaveletTransform;
  unsigned int m_NumberOfDecompositions;
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWaveletInverseImageFilter.txx"
#endif

#endif
