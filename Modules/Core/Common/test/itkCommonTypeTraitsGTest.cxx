/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkFixedArray.h"
// Derived from FixedArray (1st)
#include "itkCovariantVector.h"
#include "itkRGBAPixel.h"
#include "itkRGBPixel.h"
#include "itkPoint.h"
#include "itkVector.h"
// 2nd generation Fixed Array
#include "itkContinuousIndex.h" // From Point

#include <gtest/gtest.h>
#include <type_traits>

// info: is_pod (c++20 deprecated) is equivalent to is_trivial && is_standard_layout
// info: type_traits cannot differentiate if a explicit move constructible/assignmet exist.

TEST(TypeTraitsPOD, FixedArray) {
  using T = itk::FixedArray<float, 3>;
  EXPECT_EQ(std::is_trivial<T>::value, true);
  EXPECT_EQ(std::is_standard_layout<T>::value, true);
}

/************ First Generation FixedArray *************/
TEST(TypeTraitsPOD, Vector) {
  using T = itk::Vector<float, 3>;
  EXPECT_EQ(std::is_trivial<T>::value, true);
  EXPECT_EQ(std::is_standard_layout<T>::value, true);
}

TEST(TypeTraitsPOD, CovariantVector) {
  using T = itk::CovariantVector<float, 3>;
  EXPECT_EQ(std::is_trivial<T>::value, true);
  EXPECT_EQ(std::is_standard_layout<T>::value, true);
}

TEST(TypeTraitsPOD, Point) {
  using T = itk::Point<float, 3>;
  EXPECT_EQ(std::is_trivial<T>::value, true);
  EXPECT_EQ(std::is_standard_layout<T>::value, true);
}

TEST(TypeTraitsPOD, RGBAPixel) {
  using T = itk::RGBAPixel<unsigned int>;
  EXPECT_EQ(std::is_trivial<T>::value, true);
  EXPECT_EQ(std::is_standard_layout<T>::value, true);
}

TEST(TypeTraitsPOD, RGBPixel) {
  using T = itk::RGBPixel<unsigned int>;
  EXPECT_EQ(std::is_trivial<T>::value, true);
  EXPECT_EQ(std::is_standard_layout<T>::value, true);
}

/************ Second Generation FixedArray *************/
/* Derived from Point */
TEST(TypeTraitsPOD, ContinuousIndex) {
  using T = itk::ContinuousIndex<float, 2>;
  EXPECT_EQ(std::is_trivial<T>::value, true);
  EXPECT_EQ(std::is_standard_layout<T>::value, true);
}
