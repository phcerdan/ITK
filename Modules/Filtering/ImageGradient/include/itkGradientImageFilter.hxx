/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkGradientImageFilter_hxx
#define __itkGradientImageFilter_hxx
#include "itkGradientImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkDerivativeOperator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

namespace itk
{
//
// Constructor
//
template< class TInputImage, class TOperatorValueType, class TOutputValueType , class TOutputImageType >
GradientImageFilter< TInputImage, TOperatorValueType, TOutputValueType, TOutputImageType >
::GradientImageFilter()
{
  this->m_UseImageSpacing   = true;
  this->m_UseImageDirection = true;
}

//
// Destructor
//
template< class TInputImage, class TOperatorValueType, class TOutputValueType , class TOutputImageType >
GradientImageFilter< TInputImage, TOperatorValueType, TOutputValueType, TOutputImageType >
::~GradientImageFilter()
{}

template< class TInputImage, class TOperatorValueType, class TOutputValueType , class TOutputImageType >
void
GradientImageFilter< TInputImage, TOperatorValueType, TOutputValueType, TOutputImageType >
::GenerateInputRequestedRegion()
throw ( InvalidRequestedRegionError )
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  InputImagePointer inputPtr =
    const_cast< InputImageType * >( this->GetInput() );
  OutputImagePointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // Build an operator so that we can determine the kernel size
  DerivativeOperator< OperatorValueType, InputImageDimension > oper;
  oper.SetDirection(0);
  oper.SetOrder(1);
  oper.CreateDirectional();
  SizeValueType radius = oper.GetRadius()[0];

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius(radius);

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop( inputPtr->GetLargestPossibleRegion() ) )
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

template< class TInputImage, class TOperatorValueType, class TOutputValueType , class TOutputImageType >
void
GradientImageFilter< TInputImage, TOperatorValueType, TOutputValueType, TOutputImageType >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                       ThreadIdType threadId)
{
  unsigned int    i;
  CovariantVectorType gradient;

  ZeroFluxNeumannBoundaryCondition< InputImageType > nbc;

  ConstNeighborhoodIterator< InputImageType > nit;
  ImageRegionIterator< OutputImageType >      it;

  NeighborhoodInnerProduct< InputImageType, OperatorValueType,
                            OutputValueType > SIP;

  // Get the input and output
  OutputImageType *     outputImage = this->GetOutput();
  const InputImageType *inputImage  = this->GetInput();

  // Set up operators
  DerivativeOperator< OperatorValueType, InputImageDimension > op[InputImageDimension];

  for ( i = 0; i < InputImageDimension; i++ )
    {
    op[i].SetDirection(0);
    op[i].SetOrder(1);
    op[i].CreateDirectional();

    // Reverse order of coefficients for the convolution with the image to
    // follow.
    op[i].FlipAxes();

    // Take into account the pixel spacing if necessary
    if ( m_UseImageSpacing == true )
      {
      if ( this->GetInput()->GetSpacing()[i] == 0.0 )
        {
        itkExceptionMacro(<< "Image spacing cannot be zero.");
        }
      else
        {
        op[i].ScaleCoefficients(1.0 / this->GetInput()->GetSpacing()[i]);
        }
      }
    }

  // Calculate iterator radius
  Size< InputImageDimension > radius;
  for ( i = 0; i < InputImageDimension; ++i )
    {
    radius[i]  = op[0].GetRadius()[0];
    }

  // Find the data-set boundary "faces"
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType faceList;
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType > bC;
  faceList = bC(inputImage, outputRegionForThread, radius);

  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType::iterator fit;
  fit = faceList.begin();

  // support progress methods/callbacks
  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() );

  // Initialize the x_slice array
  nit = ConstNeighborhoodIterator< InputImageType >(radius, inputImage, *fit);

  std::slice          x_slice[InputImageDimension];
  const SizeValueType center = nit.Size() / 2;
  for ( i = 0; i < InputImageDimension; ++i )
    {
    x_slice[i] = std::slice( center - nit.GetStride(i) * radius[i],
                             op[i].GetSize()[0], nit.GetStride(i) );
    }

  // Process non-boundary face and then each of the boundary faces.
  // These are N-d regions which border the edge of the buffer.
  for ( fit = faceList.begin(); fit != faceList.end(); ++fit )
    {
    nit = ConstNeighborhoodIterator< InputImageType >(radius,
                                                      inputImage, *fit);
    it = ImageRegionIterator< OutputImageType >(outputImage, *fit);
    nit.OverrideBoundaryCondition(&nbc);
    nit.GoToBegin();

    while ( !nit.IsAtEnd() )
      {
      for ( i = 0; i < InputImageDimension; ++i )
        {
        gradient[i] = SIP(x_slice[i], nit, op[i]);
        }

      // This method optionally performs a tansform for Physical
      // coordiantes and potential conversion to a different output
      // pixel type.
      this->SetOutputPixel( it, gradient );

      ++nit;
      ++it;
      progress.CompletedPixel();
      }
    }
}

template< class TInputImage, class TOperatorValueType, class TOutputValueType , class TOutputImageType >
void
GradientImageFilter< TInputImage, TOperatorValueType, TOutputValueType, TOutputImageType >
::GenerateOutputInformation()
{
  // this methods is overloaded so that if the output image is a
  // VectorImage then the correct number of components are set.

  Superclass::GenerateOutputInformation();
  OutputImageType* output = this->GetOutput();

  if ( !output )
    {
    return;
    }
  if ( output->GetNumberOfComponentsPerPixel() != InputImageDimension )
    {
    output->SetNumberOfComponentsPerPixel( InputImageDimension );
    }
}


/**
 * Standard "PrintSelf" method
 */
template< class TInputImage, class TOperatorValueType, class TOutputValueType , class TOutputImageType >
void
GradientImageFilter< TInputImage, TOperatorValueType, TOutputValueType, TOutputImageType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "UseImageSpacing: "
     << ( this->m_UseImageSpacing ? "On" : "Off" ) << std::endl;
  os << indent << "UseImageDirection = "
     << ( this->m_UseImageDirection ? "On" : "Off" ) << std::endl;
}
} // end namespace itk

#endif
