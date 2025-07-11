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
#ifndef itkGaborKernelFunction_h
#define itkGaborKernelFunction_h

#include "itkKernelFunctionBase.h"
#include <cmath>

namespace itk
{
/**
 * \class GaborKernelFunction
 * \brief Gabor kernel used for various computer vision tasks.
 *
 * This class encapsulates a complex Gabor kernel used for
 * various computer vision tasks such as texture segmentation,
 * motion analysis, and object recognition. It is essentially
 * a complex sinusoid enveloped within a Gaussian.
 * See \cite klein1997 for a basic discussion
 * including additional references.
 *
 * This implementation was contributed as a paper to the Insight Journal
 * https://doi.org/10.54294/dhogdz
 *
 * \sa KernelFunctionBase
 *
 * \ingroup Functions
 * \ingroup ITKImageSources
 */
template <typename TRealValueType>
class ITK_TEMPLATE_EXPORT GaborKernelFunction : public KernelFunctionBase<TRealValueType>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(GaborKernelFunction);

  /** Standard class type aliases. */
  using Self = GaborKernelFunction;
  using Superclass = KernelFunctionBase<TRealValueType>;
  using Pointer = SmartPointer<Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** \see LightObject::GetNameOfClass() */
  itkOverrideGetNameOfClassMacro(GaborKernelFunction);

  /** Evaluate the function. */
  TRealValueType
  Evaluate(const TRealValueType & u) const override
  {
    TRealValueType parameter = itk::Math::sqr(u / this->m_Sigma);
    TRealValueType envelope = std::exp(TRealValueType{ -0.5 } * parameter);
    TRealValueType phase = TRealValueType{ 2.0 * itk::Math::pi } * this->m_Frequency * u + this->m_PhaseOffset;

    if (this->m_CalculateImaginaryPart)
    {
      return envelope * std::sin(phase);
    }

    return envelope * std::cos(phase);
  }

  /** Set/Get the standard deviation of the Gaussian envelope. */
  /** @ITKStartGrouping */
  itkSetMacro(Sigma, TRealValueType);
  itkGetConstMacro(Sigma, TRealValueType);
  /** @ITKEndGrouping */
  /** Set/Get the modulation frequency of the sine or cosine component. */
  /** @ITKStartGrouping */
  itkSetMacro(Frequency, TRealValueType);
  itkGetConstMacro(Frequency, TRealValueType);
  /** @ITKEndGrouping */
  /** Set/Get the phase offset of the sine or cosine component .*/
  /** @ITKStartGrouping */
  itkSetMacro(PhaseOffset, TRealValueType);
  itkGetConstMacro(PhaseOffset, TRealValueType);
  /** @ITKEndGrouping */
  /** Set/Get whether the kernel function evaluation is performed using the
   * complex part. Default is false. */
  /** @ITKStartGrouping */
  itkSetMacro(CalculateImaginaryPart, bool);
  itkGetConstMacro(CalculateImaginaryPart, bool);
  itkBooleanMacro(CalculateImaginaryPart);
  /** @ITKEndGrouping */
protected:
  GaborKernelFunction()
    : m_Sigma(TRealValueType{ 1.0 })
    , m_Frequency(TRealValueType{ 0.4 })
    , m_PhaseOffset(TRealValueType{ 0.0 })

  {}
  ~GaborKernelFunction() override = default;
  void
  PrintSelf(std::ostream & os, Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);

    os << indent << "Sigma: " << this->GetSigma() << std::endl;
    os << indent << "Frequency: " << this->GetFrequency() << std::endl;
    os << indent << "PhaseOffset: " << this->GetPhaseOffset() << std::endl;
    os << indent << "CalculateImaginaryPart: " << this->GetCalculateImaginaryPart() << std::endl;
  }

private:
  TRealValueType m_Sigma{};

  TRealValueType m_Frequency{};

  TRealValueType m_PhaseOffset{};

  bool m_CalculateImaginaryPart{};
};
} // end namespace itk

#endif
