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
#ifndef itkVnlForwardFFTImageFilter_h
#define itkVnlForwardFFTImageFilter_h

#include "itkForwardFFTImageFilter.h"

#include "vnl/algo/vnl_fft_base.h"
#include "itkFFTImageFilterFactory.h"

namespace itk
{
/**
 * \class VnlForwardFFTImageFilter
 *
 * \brief VNL based forward Fast Fourier Transform.
 *
 * The input image size must be a multiple of combinations of 2s, 3s,
 * and/or 5s in all dimensions (2, 3, and 5 should be the only prime
 * factors of the image size along each dimension).
 *
 * \ingroup FourierTransform
 *
 * \sa ForwardFFTImageFilter
 * \ingroup ITKFFT
 *
 */
template <typename TInputImage,
          typename TOutputImage = Image<std::complex<typename TInputImage::PixelType>, TInputImage::ImageDimension>>
class ITK_TEMPLATE_EXPORT VnlForwardFFTImageFilter : public ForwardFFTImageFilter<TInputImage, TOutputImage>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(VnlForwardFFTImageFilter);

  /** Standard class type aliases. */
  using InputImageType = TInputImage;
  using InputPixelType = typename InputImageType::PixelType;
  using InputSizeType = typename InputImageType::SizeType;
  using InputSizeValueType = typename InputImageType::SizeValueType;
  using OutputImageType = TOutputImage;
  using OutputPixelType = typename OutputImageType::PixelType;

  using Self = VnlForwardFFTImageFilter;
  using Superclass = ForwardFFTImageFilter<TInputImage, TOutputImage>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** \see LightObject::GetNameOfClass() */
  itkOverrideGetNameOfClassMacro(VnlForwardFFTImageFilter);

  /** Extract the dimensionality of the images. They are assumed to be
   * the same. */
  static constexpr unsigned int ImageDimension = TOutputImage::ImageDimension;
  static constexpr unsigned int InputImageDimension = TInputImage::ImageDimension;
  static constexpr unsigned int OutputImageDimension = TOutputImage::ImageDimension;

  [[nodiscard]] SizeValueType
  GetSizeGreatestPrimeFactor() const override;

  itkConceptMacro(ImageDimensionsMatchCheck, (Concept::SameDimension<InputImageDimension, OutputImageDimension>));

protected:
  VnlForwardFFTImageFilter() = default;
  ~VnlForwardFFTImageFilter() override = default;

  void
  GenerateData() override;

private:
  using SignalVectorType = vnl_vector<std::complex<InputPixelType>>;
};

// Describe whether input/output are real- or complex-valued
// for factory registration
template <>
struct FFTImageFilterTraits<VnlForwardFFTImageFilter>
{
  template <typename TUnderlying>
  using InputPixelType = TUnderlying;
  template <typename TUnderlying>
  using OutputPixelType = std::complex<TUnderlying>;
  using FilterDimensions = std::integer_sequence<unsigned int, 4, 3, 2, 1>;
};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkVnlForwardFFTImageFilter.hxx"
#endif

#endif
