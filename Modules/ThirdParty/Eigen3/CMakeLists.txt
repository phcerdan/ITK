project(ITKEigen3)
set(ITKEigen3_THIRD_PARTY 1)

option(ITK_USE_SYSTEM_EIGEN "Use an outside build of Eigen." ${ITK_USE_SYSTEM_LIBRARIES})
mark_as_advanced(ITK_USE_SYSTEM_EIGEN)

if(ITK_USE_SYSTEM_EIGEN)
  find_package(Eigen3 REQUIRED)
  set(ITKEigen3_INCLUDE_DIRS
    ${ITKEigen3_BINARY_DIR}/src
    )
  get_target_property(EIGEN_INCLUDE_DIRS Eigen3::Eigen INTERFACE_INCLUDE_DIRECTORIES)
  set(ITKEigen3_SYSTEM_INCLUDE_DIRS
    ${Eigen_INCLUDE_DIRS}
    )
  set(ITKEigen3_NO_SRC 1)
  # When this module is loaded by an app, load Eigen too.
  set(ITKEigen3_EXPORT_CODE_INSTALL "
  set(Eigen3_DIR \"${Eigen3_DIR}\")
  find_package(Eigen3 REQUIRED)
  ")
  set(ITKEigen3_EXPORT_CODE_BUILD "
  if(NOT ITK_BINARY_DIR)
    set(CMAKE_MODULE_PATH
      \"${CMAKE_CURRENT_SOURCE_DIR}/CMake\" \${CMAKE_MODULE_PATH})
    set(Eigen3_DIR \"${Eigen3_DIR}\")
    find_package(Eigen3 REQUIRED)
  endif()
  ")

else()
  set(ITKEigen3_INCLUDE_DIRS
    ${ITKEigen3_BINARY_DIR}/src/
    ${ITKEigen3_SOURCE_DIR}/src/
    )
endif()

configure_file(src/itk_eigen.h.in src/itk_eigen.h)
itk_module_impl()

install(FILES ${ITKEigen3_BINARY_DIR}/src/itk_eigen.h
  DESTINATION ${ITKEigen3_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
