set(DOCUMENTATION "This module contains the third party <a
href=\"https://github.com/QuantStack/xtensor\">xtensor</a> a numeric library for c++14.")

itk_module(ITKxtensor
  DESCRIPTION
    "${DOCUMENTATION}"
  DEPENDS
    "ITKxtl"
)
