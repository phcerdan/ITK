set(DOCUMENTATION "This module contains classes that allow manipulating an
ITK object list with the appropriate formalism. For instance, it is possible to
apply a given scalar image filter to a list of images. or to produce an output
corresponding to the elements selected from an image list. and so on.
Note: This module was contributed to ITK from Orfeo Toolbox: https://en.wikipedia.org/wiki/Orfeo_toolbox ")

itk_module(ITKObjectList
  DEPENDS
    ITKCommon

  TEST_DEPENDS
    ITKTestKernel
    ITKSmoothing
    ITKPath

  DESCRIPTION
    "${DOCUMENTATION}"
)
