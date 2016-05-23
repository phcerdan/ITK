itk_module_test()
set(ITKWaveletTests
itkWaveletOperatorTest.cxx
itkWaveletPacketTransformTest.cxx
itkWaveletTransformTest.cxx
itkSubsampleImageFilterTest.cxx
itkSubsampleImageFilterNewTest.cxx
itkWaveletInverseFilterBankNewTest.cxx
itkWaveletInverseTransformNewTest.cxx
itkWaveletTransformNewTest.cxx
itkWaveletPacketInverseTransformNewTest.cxx
itkWaveletFilterBankTest.cxx
itkWaveletPacketTransformNewTest.cxx
itkWaveletFilterBankNewTest.cxx
itkWaveletOperatorNewTest.cxx
)

CreateTestDriver(ITKWavelet "${ITKWavelet-Test_LIBRARIES}" "${ITKWaveletTests}" ../src/itkWaveletGenerator.cxx )
itk_module_target_label(ITKWaveletTestDriver)

itk_add_test(NAME itkWaveletOperator
  COMMAND ITKWaveletTestDriver )

itk_add_test(NAME itkWaveletPacketTransformNewTest
  COMMAND ITKWaveletTestDriver itkWaveletPacketTransformNewTest)

itk_add_test(NAME itkWaveletFilterBankNewTest
  COMMAND ITKWaveletTestDriver itkWaveletFilterBankNewTest)

itk_add_test(NAME itkSubsampleImageFilterNewTest
  COMMAND ITKWaveletTestDriver itkSubsampleImageFilterNewTest)

itk_add_test(NAME itkWaveletInverseFilterBankNewTest
  COMMAND ITKWaveletTestDriver itkWaveletInverseFilterBankNewTest)

itk_add_test(NAME itkWaveletInverseTransformNewTest
  COMMAND ITKWaveletTestDriver itkWaveletInverseTransformNewTest)

itk_add_test(NAME itkWaveletTransformNewTest
  COMMAND ITKWaveletTestDriver itkWaveletTransformNewTest)

# TODO ADD ALL THE TESTS, ADD DATA.
# TODO Split test of SubsampleImageFilter to Common/ ?
# Tests Declaration
itk_add_test(NAME msTvSubsampleImageFilterTest COMMAND ITKWaveletTestDriver
  --compare
  ${ITK_TEST_OUTPUT_DIR}/msSubsampleImageFilterOversampled.tif
  ${ITK_TEST_OUTPUT_DIR}/msSubsampleImageFilterOnePixelOutOf2.tif
  itkSubsampleImageFilterTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/msSubsampleImageFilterOversampled.tif
  ${ITK_TEST_OUTPUT_DIR}/msSubsampleImageFilterOnePixelOutOf2.tif
  )

itk_add_test(NAME msTvWaveletMultiScaleFilterBankTest COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_FilterBankMultiScale.tif
  itkWaveletFilterBankTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  1
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_FilterBankMultiScale.tif
  )

itk_add_test(NAME msTvWaveletFilterBankTest COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_FilterBank.tif
  itkWaveletFilterBankTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  2
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_FilterBank.tif
  )

itk_add_test(NAME itkWaveletPacketMultiScaleTransformSPLINE_BIORTHOGONAL_4_4
  COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
  itkWaveletPacketTransformTest
  DATA{Baseline//ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
  2 1
  7 #SPLINE_BIORTHOGONAL_4_4
  )
#
itk_add_test(NAME itkWaveletPacketMultiScaleTransformHaar
  COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleHaar.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleHaar.tif
  2 1
  0 #Haar
  )

itk_add_test(NAME msTvWaveletPacketMultiScaleTransformDB8 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB8.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB8.tif
  2 1
  3 #DB8
  )

itk_add_test(NAME msTvWaveletPacketTransformDB4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketDB4.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketDB4.tif
  2 2
  1 #DB4
  )

itk_add_test(NAME msTvWaveletPacketTransformDB20 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketDB20.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketDB20.tif
  2 2
  5 #DB20
  )

itk_add_test(NAME msTvWaveletPacketTransformDB6 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketDB6.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketDB6.tif
  2 2
  2 #DB6
  )

itk_add_test(NAME msTvWaveletPacketMultiScaleTransformSPLINE_BIORTHOGONAL_2_4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
  2 1
  6 #SPLINE_BIORTHOGONAL_2_4
  )

itk_add_test(NAME msTvWaveletPacketMultiScaleTransformDB6 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB6.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB6.tif
  2 1
  2 #DB6
  )

itk_add_test(NAME msTvWaveletPacketMultiScaleTransformDB4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB4.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB4.tif
  2 1
  1 #DB4
  )

itk_add_test(NAME msTvWaveletPacketTransformDB8 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketDB8.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketDB8.tif
  2 2
  3 #DB8
  )

itk_add_test(NAME msTvWaveletPacketTransformSYMLET8 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketSYMLET8.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketSYMLET8.tif
  2 2
  8 #SYMLET8
  )

itk_add_test(NAME msTvWaveletPacketTransformHaar COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketHaar.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketHaar.tif
  2 2
  0 #Haar
  )

itk_add_test(NAME msTvWaveletPacketTransformSPLINE_BIORTHOGONAL_2_4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_2_4.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_2_4.tif
  2 2
  6 #SPLINE_BIORTHOGONAL_2_4
  )

itk_add_test(NAME msTvWaveletPacketMultiScaleTransformDB20 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB20.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB20.tif
  2 1
  5 #DB20
  )

itk_add_test(NAME msTvWaveletPacketMultiScaleTransformSYMLET8 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSYMLET8.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSYMLET8.tif
  2 1
  8 #SYMLET8
  )

itk_add_test(NAME msTvWaveletPacketTransformSPLINE_BIORTHOGONAL_4_4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_4_4.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_4_4.tif
  2 2
  7 #SPLINE_BIORTHOGONAL_4_4
  )

itk_add_test(NAME msTvWaveletPacketTransformDB12 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketDB12.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketDB12.tif
  2 2
  4 #DB12
  )

itk_add_test(NAME msTvWaveletPacketMultiScaleTransformDB12 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB12.tif
  itkWaveletPacketTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB12.tif
  2 1
  4 #DB12
  )

itk_add_test(NAME msTvWaveletTransformDB4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltDB4.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltDB4.tif
  2 2
  1 #DB4
  )

itk_add_test(NAME msTvWaveletTransformDB6 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltDB6.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltDB6.tif
  2 2
  2 #DB6
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformSYMLET8 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSYMLET8.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSYMLET8.tif
  2 1
  8 #SYMLET8
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformSPLINE_BIORTHOGONAL_2_4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
  2 1
  6 #SPLINE_BIORTHOGONAL_2_4
  )

itk_add_test(NAME msTvWaveletTransformDB8 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltDB8.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltDB8.tif
  2 2
  3 #DB8
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformSPLINE_BIORTHOGONAL_4_4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
  2 1
  7 #SPLINE_BIORTHOGONAL_4_4
  )

itk_add_test(NAME msTvWaveletTransformDB20 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltDB20.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltDB20.tif
  2 2
  5 #DB20
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformDB8_1 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8_1.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8_1.tif
  2 1
  3 #DB8
  1
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformHaar_1 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_1.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_1.tif
  2 1
  0 #Haar
  1
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformDB4_4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_4.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_4.tif
  2 1
  1 #DB4
  4
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformHaar_4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_4.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_4.tif
  2 1
  0 #Haar
  4
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformDB4_1 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_1.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_1.tif
  2 1
  1 #DB4
  1
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformHaar_8 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_8.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_8.tif
  2 1
  0 #Haar
  8
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformDB4_8 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_8.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_8.tif
  2 1
  1 #DB4
  8
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformDB6_1 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6_1.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6_1.tif
  2 1
  2 #DB6
  1
  )

itk_add_test(NAME msTvWaveletTransformSPLINE_BIORTHOGONAL_2_4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_2_4.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_2_4.tif
  2 2
  6 #SPLINE_BIORTHOGONAL_2_4
  )

itk_add_test(NAME msTvWaveletTransformSYMLET8 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltSYMLET8.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltSYMLET8.tif
  2 2
  8 #SYMLET8
  )

itk_add_test(NAME msTvWaveletTransformSPLINE_BIORTHOGONAL_4_4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_4_4.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_4_4.tif
  2 2
  7 #SPLINE_BIORTHOGONAL_4_4
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformDB12 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB12.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB12.tif
  2 1
  4 #DB12
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformDB4 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4.tif
  2 1
  1 #DB4
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformDB6 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6.tif
  2 1
  2 #DB6
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformHaar COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar.tif
  2 1
  0 #Haar
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformDB8 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8.tif
  2 1
  3 #DB8
  )

itk_add_test(NAME msTvWaveletTransformDB12 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltDB12.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltDB12.tif
  2 2
  4 #DB12
  )

itk_add_test(NAME msTvWaveletMultiScaleTransformDB20 COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB20.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB20.tif
  2 1
  5 #DB20
  )

itk_add_test(NAME msTvWaveletTransformHaar COMMAND ITKWaveletTestDriver
  --compare
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltHaar.tif
  itkWaveletTransformTest
  DATA{Baseline/ROI_IKO_PAN_LesHalles.tif}
  ${ITK_TEST_OUTPUT_DIR}/ROI_IKO_PAN_LesHalles_WvltHaar.tif
  2 2
  0 #Haar
  )
