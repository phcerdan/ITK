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


#include "itkObjectList.h"
#include "itkObject.h"
// #include "otbObject.h"
// #include "otbMacro.h"
#include "itkMacro.h"
#include <vector>

///// MACROS (from otbMacro.h) /////

#define TestingCheckNotValidCommand(command) \
    { \
    int result(1); \
    try \
      { \
      command; \
      } \
    catch (std::bad_alloc& err)     { \
      throw err; } \
    catch (itk::ExceptionObject&) { std::cout << "Checking not valid Command " << # command " ok." << std::endl; \
                                    result = 0; } \
    catch (const std::exception& stde)   { \
      throw stde; } \
    catch (...) \
      { \
      std::ostringstream message; \
      message << "otb::ERROR Unknow error while running " << # command << " (catch(...) )"; \
      ::itk::ExceptionObject e_(__FILE__, __LINE__, message.str().c_str(), ITK_LOCATION); \
      throw e_; \
      } \
    if (result == 1) \
      { \
      std::ostringstream message; \
      message << "otb::ERROR: " << # command << " should be throwing an exception."; \
      ::itk::ExceptionObject e_(__FILE__, __LINE__, message.str().c_str(), ITK_LOCATION); \
      throw e_; \
      } \
    }
/** Testing macro. This macro doesn't throw a exception if the called command
 * generate a itk::ExceptionObject object. For alls others use cases, the macro
 * generate a exception. */
#define TestingCheckValidCommand(command) \
    { \
    try \
      { \
      command; \
      } \
    catch (const std::exception&)   { \
      throw; } \
    catch (...) \
      { \
      std::ostringstream message; \
      message << "::ERROR Unknow error while running " << # command << " (catch(...) )"; \
      ::itk::ExceptionObject e_(__FILE__, __LINE__, message.str().c_str(), ITK_LOCATION); \
      throw e_; \
      } \
    std::cout << " Checking valid command " << # command " ok." << std::endl; \
    }

/** This macro is used to control condition.
  *
  */
#define ControlConditionTestMacro(condition, message) \
    { \
    if ((condition)) itkGenericExceptionMacro(<< message); \
    }

////////////////////////////////////

int itkObjectListTest2(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  typedef itk::Object                 ObjectType;
  typedef itk::ObjectList<ObjectType> ObjectListType;

  ObjectType::Pointer     object     = ObjectType::New();
  ObjectListType::Pointer listObject = ObjectListType::New();

  TestingCheckNotValidCommand(listObject->Erase(1));
  TestingCheckValidCommand(listObject->Clear());
  TestingCheckNotValidCommand(listObject->SetNthElement(0, object));
  TestingCheckNotValidCommand(listObject->SetNthElement(2, object));
  TestingCheckNotValidCommand(listObject->GetNthElement(2));

  std::vector<int> tab;
  tab.reserve(3);
  ControlConditionTestMacro(tab.capacity() != 3, "Check std::vector Capacity");
  ControlConditionTestMacro(tab.size() != 0, "Check std::vector Size");
  tab.resize(2);
  ControlConditionTestMacro(tab.capacity() != 3, "Check std::vector Capacity");
  ControlConditionTestMacro(tab.size() != 2, "Check std::vector Size");

  listObject->Reserve(3);
  ControlConditionTestMacro(listObject->Capacity() != 3, "Check Reserve and Capacity coherence");
  listObject->Resize(2);
  ControlConditionTestMacro(listObject->Size() != 2, "Check Resize and Size coherence");

  TestingCheckValidCommand(listObject->SetNthElement(0, object));
  TestingCheckNotValidCommand(listObject->SetNthElement(2, object));
  return EXIT_SUCCESS;
}
