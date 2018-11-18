project(ITKEigen3)
set(ITKEigen3_THIRD_PARTY 1)
set(ITKEigen3_LIBRARIES ITK::Eigen)

# ITKCommon publicly depends on ITKEigen explicitly in itk-module.cmake (DEPENDS).
# Just note that when ITKCommon is a PRIVATE dependency of ModuleFoo ITK::Eigen is not propagated. Make it a PUBLIC dependency if needed.

option(ITK_USE_SYSTEM_EIGEN "Use an outside build of Eigen." ${ITK_USE_SYSTEM_LIBRARIES})
mark_as_advanced(ITK_USE_SYSTEM_EIGEN)

if(ITK_USE_SYSTEM_EIGEN)
  set(Eigen3_DIR_INSTALL ${Eigen3_DIR})
  set(Eigen3_DIR_BUILD ${Eigen3_DIR})
  set(Eigen3_DIR ${Eigen3_DIR_BUILD})
  find_package(Eigen3 REQUIRED)
else()
  set(Eigen3_DIR_INSTALL "${CMAKE_INSTALL_PREFIX}/${ITK_INSTALL_LIBRARY_DIR}/cmake/ITK-${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}/Modules")
  set(Eigen3_DIR_BUILD "${CMAKE_CURRENT_BINARY_DIR}/src/itkeigen")
endif()

# Eigen3 is header only, but there are compile definitions that we want to provide
# to enforce use of MPL only code, and to disable warnings.
# We only need the BINARY_DIR, we rely on the target ITK::Eigen for the include headers.
set(ITKEigen3_INCLUDE_DIRS
  ${ITKEigen3_BINARY_DIR}/src
#   ${ITKEigen3_SOURCE_DIR}/src
  )

if(ITK_USE_SYSTEM_EIGEN)
  get_target_property(Eigen_INCLUDE_DIRS Eigen3::Eigen INTERFACE_INCLUDE_DIRECTORIES)
  set(ITKEigen3_SYSTEM_INCLUDE_DIRS
    ${Eigen_INCLUDE_DIRS}
    )
endif()

# When this module is loaded by an app, load Eigen too.
set(ITKEigen3_EXPORT_CODE_INSTALL "
set(Eigen3_DIR \"${Eigen3_DIR_INSTALL}\")
find_package(Eigen3 REQUIRED)
")
set(ITKEigen3_EXPORT_CODE_BUILD "
if(NOT ITK_BINARY_DIR)
  set(Eigen3_DIR \"${Eigen3_DIR_BUILD}\")
  find_package(Eigen3 REQUIRED)
endif()
")

configure_file(src/itk_eigen.h.in src/itk_eigen.h)
itk_module_impl()

# Eigen3::Eigen is an alias of the interface library eigen when using internal eigen,
# and it is also found when using find_package(Eigen3)
# We create an alias to avoid clashing with others Eigen3::Eigen.
# When used internally in ITK, the target to use would be:
# ITKEigen3 for module dependencies
# In the case an external module want to re-use the original Eigen3 target --because for example is bridging a third-party library requiring Eigen3--
# it should set Eigen3_DIR to the internal BUILD or INSTALL cmake folder containing Eigen3Config.
if(TARGET eigen)
  # When using internal eigen (cannot create ALIAS of an existing ALIAS -Eigen3::Eigen-, so we use the real target)
  add_library(ITK::Eigen ALIAS eigen)
else()
  # When ITK_USE_SYSTEM_EIGEN: find_package
  add_library(ITK::Eigen ALIAS Eigen3::Eigen)
endif()

install(FILES ${ITKEigen3_BINARY_DIR}/src/itk_eigen.h
  DESTINATION ${ITKEigen3_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
