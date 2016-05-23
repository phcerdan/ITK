set(DOCUMENTATION "This module contains code for wavelet analysis.")

itk_module(ITKWavelet
  ENABLE_SHARED
  DEPENDS
    ITKCommon
    ITKObjectList
  TEST_DEPENDS
    ITKTestKernel
    ITKImageIntensity
    ITKIOTIFF
  DESCRIPTION
    "${DOCUMENTATION}"
)
