project(ITKxtensor)
set(ITKxtensor_THIRD_PARTY 1)

option(ITK_USE_SYSTEM_xtensor "Use an outside build of xtensor." OFF)
mark_as_advanced(ITK_USE_SYSTEM_xtensor)

if(ITK_USE_SYSTEM_xtensor)
  find_package(xtensor REQUIRED)
  set(ITKxtensor_SYSTEM_INCLUDE_DIRS ${xtensor_INCLUDE_DIRS} ${xtensor_INCLUDE_DIR})
  set(ITKxtensor_SYSTEM_LIBRARY_DIRS "${xtensor_LIBRARY_DIR}")
  set(ITKxtensor_NO_SRC 1)
endif()

itk_module_impl()
