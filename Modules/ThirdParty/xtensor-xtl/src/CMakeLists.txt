include(FetchContent) # Requires cmake 3.11
FetchContent_Declare(
  xtl
  GIT_REPOSITORY https://github.com/QuantStack/xtl
  GIT_TAG        623e196e8cd837eef1a14cb17054436de816a776 # August 17 2018
)

FetchContent_GetProperties(xtl)
if(NOT xtl_POPULATED)
  FetchContent_Populate(xtl)
  add_subdirectory(${xtl_SOURCE_DIR} ${xtl_BINARY_DIR})
endif()

# Update CMAKE_MODULE_PATH for other modules depending on xtl to find it.
message(STATUS "**** xtl_BINARY_DIR ${xtl_BINARY_DIR}")
