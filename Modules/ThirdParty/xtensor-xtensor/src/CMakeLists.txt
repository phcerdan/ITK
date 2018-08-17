# This is installed at xtl
# TODO, hardcode is not good, but not sure how to share variables between two third-party projects.
set(xtl_DIR "${CMAKE_BINARY_DIR}/_deps/xtl-build")
message(WARNING "Hardcoded xtl_DIR in xtensor-xtensor: ${xtl_DIR}")

include(FetchContent) # Requires cmake 3.11
FetchContent_Declare(
  xtensor
  GIT_REPOSITORY https://github.com/QuantStack/xtensor
  GIT_TAG        11ff2f1470b62c06c28fe113314b6415a8ca72e4 # August 17 2018
)

FetchContent_GetProperties(xtensor)
if(NOT xtensor_POPULATED)
  FetchContent_Populate(xtensor)
  add_subdirectory(${xtensor_SOURCE_DIR} ${xtensor_BINARY_DIR})
endif()
