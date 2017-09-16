/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#ifndef itkWaveletPacketDecompositionCosts_h
#define itkWaveletPacketDecompositionCosts_h

#include "itkObject.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"

namespace itk {

/** \class FullyDecomposedWaveletPacketCost
 * \brief Cost evaluation to be used into the Wavelet Packet decomposition class.
 *
 * This class implements the criteria to perform fully decomposed wavelet packet.
 * It is based on the depth of the decomposition only...
 *
 * \sa WaveletPacketForwardTransform
 *
 * \ingroup ITKWavelet
 */
template <class TImage>
class ITK_EXPORT FullyDecomposedWaveletPacketCost
  : public itk::Object
{
public:
  /** Standard typedefs */
  typedef FullyDecomposedWaveletPacketCost Self;
  typedef itk::Object                      Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(FullyDecomposedWaveletPacketCost, Object);

  typedef TImage ImageType;

  /** Access to the data */
  itkGetMacro(NumberOfAllowedDecompositions, unsigned int);
  itkSetMacro(NumberOfAllowedDecompositions, unsigned int);

  /** Evaluate the cost */
  bool Evaluate(unsigned int decomposition, const ImageType * itkNotUsed(image))
  {
    return (decomposition < m_NumberOfAllowedDecompositions);
  }

protected:
  FullyDecomposedWaveletPacketCost () {}
  ~FullyDecomposedWaveletPacketCost() ITK_OVERRIDE {}
  unsigned int m_NumberOfAllowedDecompositions = 1;

private:
  FullyDecomposedWaveletPacketCost (const Self &);   // not implemented
  void operator =(const Self&);

}; // end of class

} // end of namespace itk

#endif
