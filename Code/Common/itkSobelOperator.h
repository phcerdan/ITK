/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSobelOperator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#ifndef __itkSobelOperator_h
#define __itkSobelOperator_h

#include "itkExceptionObject.h"
#include "itkNeighborhoodOperator.h"

namespace itk {

/**
 * \class SobelOperator
 * \brief A NeighborhoodOperator for doing Sobel operation
 * at a pixel
 * 
 * SobelOperator's coefficients are a tightest-fitting convolution
 * kernel for calculating the laplacian value at a pixel.
 * SobelOperator is a directional NeighborhoodOperator that should be
 * applied to a Neighborhood or NeighborhoodPointer using the inner product
 * method. To create the operator, you need:
 * 
 * 1) Set the direction  2) call CreateOperator() 
 * For example, the Sobel Operator in vertical direction is
 *             -1  -2  -1  
 *             0    0   0 
 *             1    2   1
 * while  the Sobel Operator in horizonal direction is
 *             -1   0   1      
 *             -2   0   2 
 *             -1   0   1
 *
 * The Sobel Operator in the Nth dimension can be calculated accordingly.
 *
 * \sa NeighborhoodOperator
 * \sa Neighborhood
 * \sa ForwardDifferenceOperator
 * \sa BackwardDifferenceOperator
 * 
 * \ingroup Operators
 */
template<class TPixel, unsigned int VDimension=2,
  class TAllocator = NeighborhoodAllocator<TPixel> >
class ITK_EXPORT SobelOperator
  : public NeighborhoodOperator<TPixel, VDimension, TAllocator>
{

public:
  /**
   * Standard "Self" typedef support.
   */
  typedef SobelOperator Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef NeighborhoodOperator<TPixel, VDimension, TAllocator>  Superclass;


  /**
   * Default constructor
   */

  SobelOperator() {}

  /**
   * Copy constructor
   */
  
  SobelOperator(const Self& other)
    : NeighborhoodOperator<TPixel, VDimension, TAllocator>(other) 
  {  }
  

  /** 
   * This function is called to create the operator
   */ 
  void CreateOperator();   
 
  /**
   * Assignment operator
   */
  Self &operator=(const Self& other)
  {
    Superclass::operator=(other);
    return *this;
  }
  /**
   * Prints some debugging information
   */
  virtual void PrintSelf(std::ostream &os, Indent i) const  
  { 
    os << i << "SobelOperator { this=" << this
       << "}" << std::endl;
    Superclass::PrintSelf(os, i.GetNextIndent());
  }
  
protected:
  /**
   * Typedef support for coefficient vector type.  Necessary to
   * work around compiler bug on VC++.
   */
  typedef typename Superclass::CoefficientVector CoefficientVector;

  /**
   * Calculates operator coefficients.
   */
  CoefficientVector GenerateCoefficients();

  /**
   * Arranges coefficients spatially in the memory buffer.
   */
  void Fill(const CoefficientVector &);
//{   Superclass::FillCenteredDirectional(coeff);  }



 
private:

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSobelOperator.txx"
#endif

#endif


