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
#ifndef itkViewImages_h
#define itkViewImages_h
#include <cstddef>
#include <string>
// TODO move to other class
#include "vtkImagePlaneWidget.h"
#include "vtkSmartPointer.h"
#include "itkImageToVTKImageFilter.h"

namespace itk
{
template< typename PixelType = float>
class ViewImages
{
public:
  template<typename TViewerImage>
  struct ViewerImageInfo
  {
    ViewerImageInfo( TViewerImage * image, const std::string & description = "")
      : m_Image(image), m_Description(description) {}
    typename TViewerImage::Pointer m_Image;
    std::string m_Description;
  };

  /**
   * Add image to the container, casting from TNewImage to TImage.
   *
   * @tparam TNewImage
   * @param newImage
   */
  template<typename TNewImage>
  void Add( TNewImage * newImage, const std::string & description = "" );
  void View( const std::string & windowTitle = "itkViewImages",
      size_t windowWidth = 800, size_t windowHeight = 800);

  using Image2DType = itk::Image< PixelType, 2 >;
  using Image3DType = itk::Image< PixelType, 3 >;

  std::vector<ViewerImageInfo<Image2DType>> m_Images2D;
  std::vector<ViewerImageInfo<Image3DType>> m_Images3D;

private:
  template<typename TNewImage>
  void CastImageAndEmplaceBack( TNewImage * newImage, const std::string & description, std::true_type);
  template<typename TNewImage>
  void CastImageAndEmplaceBack( TNewImage * newImage, const std::string & description, std::false_type);
  using SlicesArrayType = FixedArray< vtkSmartPointer< vtkImagePlaneWidget >, 3 >;
  template<typename TImage>
  std::pair<typename ImageToVTKImageFilter<TImage>::Pointer, SlicesArrayType> GetConnectorAndSlices(
      TImage * image);

};
}// namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkViewImages.hxx"
#endif
#endif
