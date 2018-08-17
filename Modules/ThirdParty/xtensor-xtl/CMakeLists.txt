project(ITKxtl)
set(ITKxtl_THIRD_PARTY 1)

set(xtl_DIR ${xtl_DIR} PARENT_SCOPE)

option(ITK_USE_SYSTEM_xtl "Use an outside build of xtl." OFF)
mark_as_advanced(ITK_USE_SYSTEM_xtl)

if(ITK_USE_SYSTEM_xtl)
  find_package(xtl REQUIRED)
  set(ITKxtl_SYSTEM_INCLUDE_DIRS ${xtl_INCLUDE_DIRS} ${xtl_INCLUDE_DIR})
  set(ITKxtl_SYSTEM_LIBRARY_DIRS "${xtl_LIBRARY_DIR}")
  set(ITKxtl_NO_SRC 1)
endif()

itk_module_impl()
