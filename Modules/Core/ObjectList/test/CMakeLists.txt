itk_module_test()
set(ITKObjectListTests
  itkObjectListTest.cxx
  itkObjectListNewTest.cxx
  itkImageListTest.cxx
  itkImageListNewTest.cxx
  itkImageListSourceNewTest.cxx
  itkImageListToImageFilterNewTest.cxx
  itkImageListToImageListApplyFilterTest.cxx
  itkImageListToImageListApplyFilterNewTest.cxx
  itkImageListToImageListFilterNewTest.cxx
  itkImageListToVectorImageFilterTest2.cxx
  itkImageListToVectorImageFilterTest.cxx
  itkImageListToVectorImageFilterNewTest.cxx
  itkImageToImageListFilterNewTest.cxx
  itkVectorImageToImageListFilterTest.cxx
  itkVectorImageToImageListFilterIteratorTest.cxx
  itkVectorImageToImageListFilterNewTest.cxx
  itkObjectListTest2.cxx
  itkObjectListToObjectListFilterNewTest.cxx
  )

CreateTestDriver(ITKObjectList "${ITKObjectList-Test_LIBRARIES}" "${ITKObjectListTests}")

itk_add_test(NAME itkObjectListNewTest
  COMMAND ITKObjectListTestDriver itkObjectListNewTest)

itk_add_test(NAME itkImageListToImageListApplyFilterNewTest
  COMMAND ITKObjectListTestDriver itkImageListToImageListApplyFilterNewTest)

itk_add_test(NAME itkImageListToImageListFilterNewTest
  COMMAND ITKObjectListTestDriver itkImageListToImageListFilterNewTest)

itk_add_test(NAME itkImageListToVectorImageFilterNewTest
  COMMAND ITKObjectListTestDriver itkImageListToVectorImageFilterNewTest)

itk_add_test(NAME itkImageToImageListFilterNewTest
  COMMAND ITKObjectListTestDriver itkImageToImageListFilterNewTest)

itk_add_test(NAME itkVectorImageToImageListFilterNewTest
  COMMAND ITKObjectListTestDriver itkVectorImageToImageListFilterNewTest)

itk_add_test(NAME itkImageListNewTest
  COMMAND ITKObjectListTestDriver itkImageListNewTest)

itk_add_test(NAME itkImageListSourceNewTest
  COMMAND ITKObjectListTestDriver itkImageListSourceNewTest)

itk_add_test(NAME itkImageListToImageFilterNewTest
  COMMAND ITKObjectListTestDriver itkImageListToImageFilterNewTest)

# TODO Port Tests/Data to ITK.
# TODO Next 2 test ommited because otbPolygon
itk_add_test(NAME itkObjectListTestNotValidTest
  COMMAND ITKObjectListTestDriver itkObjectListTest2)
itk_add_test(NAME coTuObjectListToObjectListFilterNewTest
  COMMAND ITKObjectListTestDriver itkObjectListToObjectListFilterNewTest)

itk_add_test(NAME itkObjectListTest
  COMMAND ITKObjectListTestDriver itkObjectListTest
  DATA{Baseline/couleurs.jpg}
  DATA{Baseline/poupees.png}
  DATA{Baseline/couleurs_extrait.png}
  )

itk_add_test(NAME itkImageListTest
  COMMAND ITKObjectListTestDriver
  --compare
  DATA{Baseline/amst.png}
  ${ITK_TEST_OUTPUT_DIR}/amst.png
  itkImageListTest
  DATA{Baseline/amst.png}
  ${ITK_TEST_OUTPUT_DIR}/amst.png
  )

itk_add_test(NAME itkImageListToImageListApplyFilterTest
  COMMAND ITKObjectListTestDriver
  --compare
  DATA{Baseline/itkImageListToImageListApplyFilterTestBand1.png}
  ${ITK_TEST_OUTPUT_DIR}/itkImageListToImageListApplyFilterTestBand1.png
  --compare
  DATA{Baseline/itkImageListToImageListApplyFilterTestBand2.png}
  ${ITK_TEST_OUTPUT_DIR}/itkImageListToImageListApplyFilterTestBand2.png
  --compare
  DATA{Baseline/itkImageListToImageListApplyFilterTestBand3.png}
  ${ITK_TEST_OUTPUT_DIR}/itkImageListToImageListApplyFilterTestBand3.png
  itkImageListToImageListApplyFilterTest
  DATA{${ITK_DATA_ROOT}/Input/Number1inText.png}
  DATA{${ITK_DATA_ROOT}/Input/Number2inText.png}
  DATA{${ITK_DATA_ROOT}/Input/Number3inText.png}
  ${ITK_TEST_OUTPUT_DIR}/itkImageListToImageListApplyFilterTestBand1.png
  ${ITK_TEST_OUTPUT_DIR}/itkImageListToImageListApplyFilterTestBand2.png
  ${ITK_TEST_OUTPUT_DIR}/itkImageListToImageListApplyFilterTestBand3.png
  )

itk_add_test(NAME itkImageListToVectorImageFilterTest2
  COMMAND ITKObjectListTestDriver
  itkImageListToVectorImageFilterTest2
  DATA{${ITK_DATA_ROOT}/Input/Number1inText.png}
  DATA{${ITK_DATA_ROOT}/Input/Number2inText.png}
  DATA{${ITK_DATA_ROOT}/Input/Number3inText.png}
  )

itk_add_test(NAME itkImageListToVectorImageFilterTest
  COMMAND ITKObjectListTestDriver
  --compare
  DATA{Baseline/itkImageListToVectorImageFilterTest.png}
  ${ITK_TEST_OUTPUT_DIR}/itkImageListToVectorImageFilterTest.png
  itkImageListToVectorImageFilterTest
  DATA{${ITK_DATA_ROOT}/Input/Number1inText.png}
  DATA{${ITK_DATA_ROOT}/Input/Number2inText.png}
  DATA{${ITK_DATA_ROOT}/Input/Number3inText.png}
  ${ITK_TEST_OUTPUT_DIR}/itkImageListToVectorImageFilterTest.png
  )

itk_add_test(NAME itkVectorImageToImageListFilterTest
  COMMAND ITKObjectListTestDriver
  --compare
  DATA{Baseline/itkVectorImageToImageListFilterTestBand1.png}
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterTestBand1.png
  --compare
  DATA{Baseline/itkVectorImageToImageListFilterTestBand2.png}
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterTestBand2.png
  --compare
  DATA{Baseline/itkVectorImageToImageListFilterTestBand3.png}
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterTestBand3.png
  itkVectorImageToImageListFilterTest
  DATA{Baseline/poupees.png}
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterTestBand1.png
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterTestBand2.png
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterTestBand3.png
  )

itk_add_test(NAME itkVectorImageToImageListFilterIteratorTest
  COMMAND ITKObjectListTestDriver
  --compare
  DATA{Baseline/itkVectorImageToImageListFilterTestBand1.png}
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterItBand1.png
  --compare
  DATA{Baseline/itkVectorImageToImageListFilterTestBand2.png}
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterItBand2.png
  --compare
  DATA{Baseline/itkVectorImageToImageListFilterTestBand3.png}
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterItBand3.png
  itkVectorImageToImageListFilterIteratorTest
  DATA{Baseline/poupees.png}
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterIt
  png
  )

itk_add_test(NAME itkVectorImageToImageListFilterIteratorTest2
  COMMAND ITKObjectListTestDriver
  --compare
  DATA{Baseline/itkVectorImageToImageListFilterTestBand1.png}
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterIt2Band1.tif
  --compare
  DATA{Baseline/itkVectorImageToImageListFilterTestBand2.png}
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterIt2Band2.tif
  --compare
  DATA{Baseline/itkVectorImageToImageListFilterTestBand3.png}
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterIt2Band3.tif
  itkVectorImageToImageListFilterIteratorTest
  DATA{Baseline/poupees.png}
  ${ITK_TEST_OUTPUT_DIR}/itkVectorImageToImageListFilterIt2
  tif
  )
