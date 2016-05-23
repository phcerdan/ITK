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
#include "itkMacro.h"
#include "itkObjectListToObjectListFilter.h"
#include "itkObjectList.h"
#include "itkObject.h"
#include "itkPolyLineParametricPath.h"
#include <complex>

int itkObjectListToObjectListFilterNewTest(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef itk::PolyLineParametricPath<2>    InputObjectType;
  typedef itk::Object                       OutputObjectType;
  typedef itk::ObjectList<InputObjectType>  InputListType;
  typedef itk::ObjectList<OutputObjectType> OutputListType;

  typedef itk::ObjectListToObjectListFilter<InputListType, OutputListType> ObjectListFilterType;

  ObjectListFilterType::Pointer objectList = ObjectListFilterType::New();

  std::cout << objectList << std::endl;

  return EXIT_SUCCESS;
}
