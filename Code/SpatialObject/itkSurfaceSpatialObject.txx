/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSurfaceSpatialObject.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifndef __itkSurfaceSpatialObject_txx
#define __itkSurfaceSpatialObject_txx

#include "itkSurfaceSpatialObject.h" 

namespace itk  
{ 

/** Constructor */
template< unsigned int TDimension >
SurfaceSpatialObject< TDimension > 
::SurfaceSpatialObject()  
{ 
  m_Dimension = TDimension;
  m_TypeName = "SurfaceSpatialObject";
  m_Property->SetRed(1); 
  m_Property->SetGreen(0); 
  m_Property->SetBlue(0); 
  m_Property->SetAlpha(1); 
  this->ComputeBoundingBox();
} 

/** Destructor */ 
template< unsigned int TDimension >
SurfaceSpatialObject< TDimension >  
::~SurfaceSpatialObject()
{ 
} 
 
/** Get the list of points composing the surface */
template< unsigned int TDimension >
typename SurfaceSpatialObject< TDimension > ::PointListType &  
SurfaceSpatialObject< TDimension > 
::GetPoints() 
{ 
  itkDebugMacro( "Getting SurfacePoint list" );
  return m_Points;
} 
 
/** Set the list of points composing the surface */
template< unsigned int TDimension >
void  
SurfaceSpatialObject< TDimension >  
::SetPoints( PointListType & points )  
{
  // in this function, passing a null pointer as argument will
  // just clear the list...
  m_Points.clear();
   
  typename PointListType::iterator it,end;
  it = points.begin();    
  end = points.end();
  while(it != end)
    {
    m_Points.push_back(*it);
    it++;
    } 
   
  this->Modified();
}
 
/** Print the surface object */
template< unsigned int TDimension >
void  
SurfaceSpatialObject< TDimension >  
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "SurfaceSpatialObject(" << this << ")" << std::endl; 
  os << indent << "ID: " << m_Id << std::endl; 
  os << indent << "nb of points: "<< static_cast<unsigned long>( m_Points.size() )<< std::endl;
  Superclass::PrintSelf( os, indent ); 
} 

/** Compute the bounds of the surface */
template< unsigned int TDimension >
bool 
SurfaceSpatialObject< TDimension >  
::ComputeLocalBoundingBox() const
{ 
  itkDebugMacro( "Computing surface bounding box" );

  if( m_BoundingBoxChildrenName.empty() 
        || strstr(typeid(Self).name(), m_BoundingBoxChildrenName.c_str()) )
    {
    typename PointListType::const_iterator it  = m_Points.begin();
    typename PointListType::const_iterator end = m_Points.end();
  
    if(it == end)
      {
      return false;
      }
    else
      {
      PointType pt = this->GetIndexToWorldTransform()->TransformPoint((*it).GetPosition());
      m_Bounds->SetMinimum(pt);
      m_Bounds->SetMaximum(pt);
      it++;
      while(it!= end) 
        {
        PointType pt = this->GetIndexToWorldTransform()->TransformPoint((*it).GetPosition());
        m_Bounds->ConsiderPoint(pt);
        it++;
        }
      }
    }
  return true;
} 

/** Return true is the given point is on the surface */
template< unsigned int TDimension >
bool 
SurfaceSpatialObject< TDimension >  
::IsInside( const PointType & point, unsigned int depth, char * name ) const
{
  itkDebugMacro( "Checking the point [" << point << "is on the surface" );

  if(name == NULL || strstr(typeid(Self).name(), name) )
    {
    typename PointListType::const_iterator it = m_Points.begin();
    typename PointListType::const_iterator itEnd = m_Points.end();
    
    const TransformType * giT = GetWorldToIndexTransform();
    PointType transformedPoint = giT->TransformPoint(point);
  
    if( m_Bounds->IsInside(transformedPoint) )
      {
      while(it != itEnd)
        {
        if((*it).GetPosition() == transformedPoint)
          {
          return true;
          }
        it++;
        }
      }
    }

  return Superclass::IsInside(point, depth, name);
} 

/** Return true if the surface is evaluable at a specified point */
template< unsigned int TDimension >
bool
SurfaceSpatialObject< TDimension > 
::IsEvaluableAt( const PointType & point, unsigned int depth, char * name ) const
{
  itkDebugMacro( "Checking if the surface is evaluable at " << point );
  return IsInside(point, depth, name);
}

/** Return 1 if the point is on the surface */
template< unsigned int TDimension >
bool
SurfaceSpatialObject< TDimension > 
::ValueAt( const PointType & point, double & value, unsigned int depth,
           char * name ) const
{
  itkDebugMacro( "Getting the value of the surface at " << point );
  if( IsInside(point, 0, name) )
    {
    value = 1;
    return true;
    }
  else
    {
    if( Superclass::IsEvaluableAt(point, depth, name) )
      {
      Superclass::ValueAt(point, value, depth, name);
      return true;
      }
    else
      {
      value = 0;
      return false;
      }
    }
  return false;
}

} // end namespace itk 

#endif
