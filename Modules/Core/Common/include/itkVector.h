/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkVector_h
#define itkVector_h

#include "itkFixedArray.h"

#include "vnl/vnl_vector_ref.h" // GetVnlVector method return

namespace itk
{
/** \class Vector
 * \brief A templated class holding a n-Dimensional vector.
 *
 * Vector is a templated class that holds a single vector (i.e., an array
 * of values).  Vector can be used as the data type held at each pixel in
 * an Image or at each vertex of an Mesh. The template parameter T can
 * be any data type that behaves like a primitive (or atomic) data type (int,
 * short, float, complex).  The VVectorDimension defines the number of
 * components in the vector array.
 *
 * Vector is not a dynamically extendible array like std::vector. It is
 * intended to be used like a mathematical vector.
 *
 * If you wish a simpler pixel types, you can use Scalar, which represents
 * a single data value at a pixel. There is also the more complex type
 * ScalarVector, which supports (for a given pixel) a single scalar value
 * plus an array of vector values. (The scalar and vectors can be of
 * different data type.)
 *
 * \ingroup Geometry
 * \ingroup DataRepresentation
 *
 * \sa Image
 * \sa Mesh
 * \sa Point
 * \sa CovariantVector
 * \sa Matrix
 * \ingroup ITKCommon
 *
 * \sphinx
 * \sphinxexample{Core/Common/CreateAVector,Create a vector}
 * \sphinxexample{Core/Common/VectorDotProduct,Dot product (inner product) of two vectors}
 * \endsphinx
 */
template <typename T, unsigned int VVectorDimension = 3>
class ITK_TEMPLATE_EXPORT Vector : public FixedArray<T, VVectorDimension>
{
public:
  /** Standard class type aliases. */
  using Self = Vector;
  using Superclass = FixedArray<T, VVectorDimension>;

  /** ValueType can be used to declare a variable that is the same type
   * as a data element held in an Vector.   */
  using ValueType = T;
  using RealValueType = typename NumericTraits<ValueType>::RealType;

  /** Dimension of the vector space. */
  static constexpr unsigned int Dimension = VVectorDimension;

  /** I am a vector type. */
  using VectorType = Self;

  /** Component value type */
  using ComponentType = T;

  /** The Array type from which this vector is derived. */
  using BaseArray = FixedArray<T, VVectorDimension>;

  /** Get the dimension (size) of the vector. */
  static unsigned int
  GetVectorDimension()
  {
    return VVectorDimension;
  }

  /** Copy values from the vnl_vector input to the internal memory block.  The minimum of
   *  VVectorDimension and vnl_vector::size() elements are copied. */
  void
  SetVnlVector(const vnl_vector<T> &);

  /** Get a vnl_vector_ref referencing the same memory block. */
  vnl_vector_ref<T>
  GetVnlVector();

  /** Get a vnl_vector with a copy of the internal memory block. */
  [[nodiscard]] vnl_vector<T>
  GetVnlVector() const;

  /** Default-constructor.
   * \note The other five "special member functions" are defaulted implicitly, following the C++ "Rule of Zero". */
  Vector() = default;

#if !defined(ITK_LEGACY_REMOVE)
  /** Constructor to initialize entire vector to one value.
   * \warning Not intended to convert a scalar value into
   * a Vector filled with that value.
   * Deprecated */
  Vector(const ValueType & r);
#else

  /** Constructor to initialize entire vector to one value,
   * if explicitly invoked. */
  explicit Vector(const ValueType & r);

  /** Prevents copy-initialization from `nullptr`, as well as from `0` (NULL). */
  Vector(std::nullptr_t) = delete;
#endif

  /** Pass-through constructor for the Array base class. */
  /** @ITKStartGrouping */
  template <typename TVectorValueType>
  Vector(const Vector<TVectorValueType, VVectorDimension> & r)
    : BaseArray(r)
  {}
  Vector(const ValueType r[Dimension])
    : BaseArray(r)
  {}
  template <typename TVectorValueType>
  Vector(const TVectorValueType r[Dimension])
    : BaseArray(r)
  {}
  /** @ITKEndGrouping */

  /** Explicit constructor for std::array. */
  explicit Vector(const std::array<ValueType, VVectorDimension> & stdArray)
    : BaseArray(stdArray)
  {}

  /** Pass-through assignment operator for the Array base class. */
  template <typename TVectorValueType>
  Vector &
  operator=(const Vector<TVectorValueType, VVectorDimension> & r)
  {
    BaseArray::operator=(r);
    return *this;
  }

  Vector &
  operator=(const ValueType r[VVectorDimension]);

  /** Scalar operator*=.  Scales elements by a scalar. */
  template <typename Tt>
  inline const Self &
  operator*=(const Tt & value)
  {
    for (unsigned int i = 0; i < VVectorDimension; ++i)
    {
      (*this)[i] = static_cast<ValueType>((*this)[i] * value);
    }
    return *this;
  }

  /** Scalar operator/=.  Scales (divides) elements by a scalar. */
  template <typename Tt>
  inline const Self &
  operator/=(const Tt & value)
  {
    for (unsigned int i = 0; i < VVectorDimension; ++i)
    {
      (*this)[i] = static_cast<ValueType>((*this)[i] / value);
    }
    return *this;
  }

  /** Vector operator+=.  Adds a vectors to the current vector. */
  const Self &
  operator+=(const Self & vec);

  /** Vector operator-=.  Subtracts a vector from a current vector. */
  const Self &
  operator-=(const Self & vec);

  /** Vector negation.  Negate all the elements of a vector. Return a new
   *  vector */
  Self
  operator-() const;

  /** Vector addition. Add two vectors. Return a new vector. */
  Self
  operator+(const Self & vec) const;

  /** Vector subtraction. Subtract two vectors. Return a new vector. */
  Self
  operator-(const Self & vec) const;

  /** Vector operator*.  Performs the inner product of two vectors.
   * this is also known as the scalar product. */
  ValueType
  operator*(const Self & other) const;

  /** Scalar operator*. Scale the elements of a vector by a scalar.
   * Return a new vector. */
  inline Self
  operator*(const ValueType & value) const
  {
    Self result;

    for (unsigned int i = 0; i < VVectorDimension; ++i)
    {
      result[i] = static_cast<ValueType>((*this)[i] * value);
    }
    return result;
  }

  /** Scalar operator/. Scale (divide) the elements of a vector by a scalar.
   * Return a new vector. */
  template <typename Tt>
  inline Self
  operator/(const Tt & value) const
  {
    Self result;

    for (unsigned int i = 0; i < VVectorDimension; ++i)
    {
      result[i] = static_cast<ValueType>((*this)[i] / value);
    }
    return result;
  }

  /** Operators == and != compare a vector component by component. All
   * components must be equal for two vectors to be equal. (Of course
   * compile-time constraints on the template parameters length and type
   * prevent comparisons between vectors of different type and length.) */
  bool
  operator==(const Self & v) const
  {
    return Superclass::operator==(v);
  }

  ITK_UNEQUAL_OPERATOR_MEMBER_FUNCTION(Self);

  /** Returns the Euclidean Norm of the vector (also referred to as its "magnitude"). */
  [[nodiscard]] RealValueType
  GetNorm() const;

  /** Returns vector's Squared Euclidean Norm  */
  [[nodiscard]] RealValueType
  GetSquaredNorm() const;

  /** Returns the number of components in this vector type */
  static unsigned int
  GetNumberOfComponents()
  {
    return VVectorDimension;
  }

  /** Divides the vector components by the vector norm (when the norm is not
   * null). The norm used is returned. */
  RealValueType
  Normalize();

  void
  SetNthComponent(int c, const ComponentType & v)
  {
    this->operator[](c) = v;
  }

  /** Copy from another Vector with a different representation type.
   *  Casting is done with C-Like rules  */
  template <typename TCoordinateB>
  void
  CastFrom(const Vector<TCoordinateB, VVectorDimension> & pa)
  {
    for (unsigned int i = 0; i < VVectorDimension; ++i)
    {
      (*this)[i] = static_cast<T>(pa[i]);
    }
  }

  template <typename TCoordinateB>
  operator Vector<TCoordinateB, VVectorDimension>()
  {
    Vector<TCoordinateB, VVectorDimension> r;
    for (unsigned int i = 0; i < VVectorDimension; ++i)
    {
      r[i] = static_cast<TCoordinateB>((*this)[i]);
    }
    return r;
  }
};

/** Premultiply Operator for product of a vector and a scalar.
 *  Vector< T, N >  =  T * Vector< T,N > */
template <typename T, unsigned int VVectorDimension>
inline Vector<T, VVectorDimension>
operator*(const T & scalar, const Vector<T, VVectorDimension> & v)
{
  return v.operator*(scalar);
}

/** Print content to an ostream */
template <typename T, unsigned int VVectorDimension>
std::ostream &
operator<<(std::ostream & os, const Vector<T, VVectorDimension> & vct);

/** Read content from an istream */
template <typename T, unsigned int VVectorDimension>
std::istream &
operator>>(std::istream & is, Vector<T, VVectorDimension> & vct);

ITKCommon_EXPORT Vector<double, 3>
                 CrossProduct(const Vector<double, 3> &, const Vector<double, 3> &);

ITKCommon_EXPORT Vector<float, 3>
                 CrossProduct(const Vector<float, 3> &, const Vector<float, 3> &);

ITKCommon_EXPORT Vector<int, 3>
                 CrossProduct(const Vector<int, 3> &, const Vector<int, 3> &);


template <typename T, unsigned int VVectorDimension>
inline void
swap(Vector<T, VVectorDimension> & a, Vector<T, VVectorDimension> & b) noexcept
{
  a.swap(b);
}


/** Makes a Vector object, having the specified values as coordinates. */
template <typename TValue, typename... TVariadic>
auto
MakeVector(const TValue firstValue, const TVariadic... otherValues)
{
  static_assert(std::conjunction_v<std::is_same<TVariadic, TValue>...>,
                "The other values should have the same type as the first value.");

  constexpr unsigned int              dimension{ 1 + sizeof...(TVariadic) };
  const std::array<TValue, dimension> stdArray{ { firstValue, otherValues... } };
  return Vector<TValue, dimension>{ stdArray };
}

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkVector.hxx"
#endif

#endif
