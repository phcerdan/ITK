set(ITKWavelet_SRC
itkWaveletGenerator.cxx
)
add_library(ITKWavelet ${ITKWavelet_SRC})
itk_module_link_dependencies()
itk_module_target(ITKWavelet)
