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

#include "itkArray.h"
#include "vnl/vnl_vector.h"

#include <gtest/gtest.h>
#include <type_traits>

// type_traits cannot differentiate if a explicit
// move constructible/assignmet exist. The only way would be to delete the copy.
TEST(ArrayTest, type_traits) {
  using T = itk::Array<double>;
  EXPECT_EQ(std::is_trivial<T>::value, false);
  EXPECT_EQ(std::is_standard_layout<T>::value, false);
  EXPECT_EQ(std::is_default_constructible<T>::value, true);
}

TEST(VnlVectorTest, type_traits) {
  using T = vnl_vector<double>;
  EXPECT_EQ(std::is_trivial<T>::value, false);
  EXPECT_EQ(std::is_standard_layout<T>::value, true);
  EXPECT_EQ(std::is_default_constructible<T>::value, true);
}
