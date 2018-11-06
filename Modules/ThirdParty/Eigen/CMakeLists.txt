project(ITKEigen)
set(ITKEigen_THIRD_PARTY 1)

option(ITK_USE_SYSTEM_EIGEN "Use an outside build of Eigen." ${ITK_USE_SYSTEM_LIBRARIES})
mark_as_advanced(ITK_USE_SYSTEM_EIGEN)

if(ITK_USE_SYSTEM_EIGEN)
  find_package(Eigen3 REQUIRED)
  set(ITKEigen_INCLUDE_DIRS
    ${ITKEigen_BINARY_DIR}/src
    )
  get_target_property(EIGEN_INCLUDE_DIRS Eigen3::Eigen INCLUDE_DIRECTORIES)
  set(ITKEigen_SYSTEM_INCLUDE_DIRS
    ${Eigen_INCLUDE_DIRS}
    )
  set(ITKEigen_NO_SRC 1)
else()
  set(ITKEigen_INCLUDE_DIRS
    ${ITKEigen_BINARY_DIR}/src/
    ${ITKEigen_SOURCE_DIR}/src/
    )
endif()

configure_file(src/itk_eigen.h.in src/itk_eigen.h)
itk_module_impl()

install(FILES ${ITKEigen_BINARY_DIR}/src/itk_eigen.h
  DESTINATION ${ITKEigen_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
