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
#ifndef itkViewImages_hxx
#define itkViewImages_hxx
#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleRubberBand3D.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkImageMapper.h"
#include "vtkImagePlaneWidget.h"

#include "vtkRendererCollection.h"
#include "itkCastImageFilter.h"
#include "itkImage.h"
#include "itkImageToVTKImageFilter.h"
#include "itkStatisticsImageFilter.h"
#include "itkFixedArray.h"
#include "itkViewImage.h"

namespace itk
{
template< typename PixelType >
template< typename TNewImage >
void
ViewImages< PixelType >
::CastImageAndEmplaceBack( TNewImage * newImage,
  const std::string & description, std::true_type)
{
  using CastFilterType = itk::CastImageFilter<TNewImage, Image2DType>;
  auto castFilter = CastFilterType::New();
  castFilter->SetInput(newImage);
  castFilter->Update();
  m_Images2D.emplace_back(ViewerImageInfo<Image2DType>(castFilter->GetOutput(), description));
}

template< typename PixelType >
template< typename TNewImage >
void
ViewImages< PixelType >
::CastImageAndEmplaceBack( TNewImage * newImage,
  const std::string & description, std::false_type)
{
  using CastFilterType = itk::CastImageFilter<TNewImage, Image3DType>;
  auto castFilter = CastFilterType::New();
  castFilter->SetInput(newImage);
  castFilter->Update();
  m_Images3D.emplace_back(ViewerImageInfo<Image3DType>(castFilter->GetOutput(), description));
}

template< typename PixelType >
template< typename TNewImage >
void
ViewImages< PixelType >
::Add( TNewImage * newImage,
       const std::string & description)
{
  constexpr unsigned int NewImageDimension = TNewImage::ImageDimension;
  constexpr bool is2D = NewImageDimension == 2;
  if (NewImageDimension > 3)
    {
    itkGenericExceptionMacro( << "Cannot add the image to ViewImages container."
      "Unhandled dimension : " << NewImageDimension );
    }
  // Cast to PixelType
  // Push to appropiate container
  this->CastImageAndEmplaceBack(newImage, description,
    std::integral_constant<bool, is2D>());

}

template< typename PixelType >
void
ViewImages< PixelType >
::View(
  const std::string & windowTitle,
  size_t windowWidth,
  size_t windowHeight )
{
  // Setup the render window and interactor
  vtkSmartPointer< vtkRenderWindow > renderWindow = vtkSmartPointer< vtkRenderWindow >::New();
  renderWindow->SetWindowName(windowTitle.c_str());
  renderWindow->SetSize(windowWidth, windowHeight);

  // Setup render window interactor
  vtkSmartPointer< vtkRenderWindowInteractor > renderWindowInteractor =
    vtkSmartPointer< vtkRenderWindowInteractor >::New();
  vtkSmartPointer< vtkInteractorStyleRubberBand3D > style =
    vtkSmartPointer< vtkInteractorStyleRubberBand3D >::New();
  renderWindowInteractor->SetInteractorStyle(style);
  renderWindowInteractor->SetRenderWindow(renderWindow);

  size_t numberOf2DImages = m_Images2D.size();
  size_t numberOf3DImages = m_Images3D.size();
  size_t numberOfRenderers = numberOf2DImages + numberOf3DImages;
  // Render all of the images
  // Optimize ~square~ number of columns and rows, given the number of images.
  size_t numberOfColumns = static_cast<size_t>(
      sqrt(static_cast<double>(numberOfRenderers))
  );
  size_t numberOfRows = static_cast<size_t>(
      std::ceil( numberOfRenderers / static_cast<double>(numberOfColumns) )
  );
  std::cout << "Renderers: " << numberOfRenderers <<
    ". 2D: " << numberOf2DImages << ". 3D: " << numberOf3DImages << std::endl;
  std::cout << "Cols: " << numberOfColumns << std::endl;
  std::cout << "Rows: " << numberOfRows << std::endl;
  // unsigned int rendererSizeWidth = static_cast<unsigned int>(
  //   windowWidth / static_cast<float>(numberOfColumns)
  // );
  // unsigned int rendererSizeHeight = static_cast<unsigned int>(
  //   windowHeight / static_cast<float>(numberOfRows)
  // );

  std::vector<double*> viewports;
  std::vector<vtkSmartPointer<vtkRenderer>> renderers;
  for(unsigned int row = 0; row < numberOfRows; ++row)
    {
    for(unsigned int col = 0; col < numberOfColumns; ++col)
      {
      size_t linearCount = row * numberOfColumns + col;
      double viewport[4] = { // (xmin, ymin, xmax, ymax)
         static_cast<double>(col) / numberOfColumns,
         static_cast<double>(numberOfRows - (row+1)) / numberOfRows ,
         static_cast<double>(col+1) / numberOfColumns,
         static_cast<double>(numberOfRows - row)  / numberOfRows };
      viewports.push_back(viewport);
      std::cout << viewport[0] << ", " << viewport[1] << ", " << viewport[2] << ", " << viewport[3] << std::endl;

      // Setup renderer
      renderers.emplace_back( vtkSmartPointer<vtkRenderer>::New());
      auto renderer = renderers.back();
      renderWindow->AddRenderer(renderer);
      renderer->SetViewport(viewports[linearCount]);
      double background[6] = {.4, .5, .6, .6, .5, .4};
      renderer->SetBackground(background);
      // Flip camera because VTK-ITK different corner for origin.
      double pos[3];
      double vup[3];
      vtkCamera *cam = renderer->GetActiveCamera();
      cam->GetPosition(pos);
      cam->GetViewUp(vup);
      std::cout << "cam pos: " << pos[0] << ", " << pos[1] << ", " <<pos[2] << std::endl;
      std::cout << "cam vup: " << vup[0] << ", " << vup[1] << ", " <<vup[2] << std::endl;
      for ( unsigned int i = 0; i < 3; ++i )
        {
        pos[i] = -pos[i];
        vup[i] = -vup[i];
        // pos[i] = -pos[i] * 0.5;
        // vup[i] = -vup[i];
        }
      cam->SetPosition(pos);
      cam->SetViewUp(vup);
      renderer->ResetCamera();
      renderer->SetActiveCamera( renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera() );
      }
    }
  //
  // vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  // renderWindow->AddRenderer(renderer);
  // Flip camera because VTK-ITK different corner for origin.
  // double pos[3];
  // double vup[3];
  // vtkCamera *cam = renderer->GetActiveCamera();
  // cam->GetPosition(pos);
  // cam->GetViewUp(vup);
  // for ( unsigned int i = 0; i < 3; ++i )
  //   {
  //   pos[i] = -pos[i];
  //   vup[i] = -vup[i];
  //   }
  // cam->SetPosition(pos);
  // cam->SetViewUp(vup);
  // renderer->ResetCamera();
  using Connector2DAndSlicesPair = std::pair< typename ImageToVTKImageFilter<Image2DType>::Pointer, SlicesArrayType>;
  using Connector3DAndSlicesPair = std::pair< typename ImageToVTKImageFilter<Image3DType>::Pointer, SlicesArrayType>;
    // Keep alive the connectors
  std::vector<Connector2DAndSlicesPair> connectors_and_planes2D;
  std::vector<Connector3DAndSlicesPair> connectors_and_planes3D;
  for (size_t n = 0; n < numberOf2DImages; ++n) {
    connectors_and_planes2D.push_back(
      this->GetConnectorAndSlices(m_Images2D[n].m_Image.GetPointer()));
    for ( unsigned i = 0; i < 3; ++i )
      {
      auto widget = connectors_and_planes2D.back().second[i];
      widget->SetInteractor(renderWindowInteractor);
      widget->PlaceWidget();
      widget->On();
      }
  }
  // vtkCollectionSimpleIterator cookie;
  // renderWindow->GetRenderers()->InitTraversal(cookie);
  for (size_t n = 0; n < numberOf3DImages; ++n) {
    connectors_and_planes3D.push_back(
      this->GetConnectorAndSlices(m_Images3D[n].m_Image.GetPointer()));
    for ( unsigned i = 0; i < 3; ++i )
      {
      auto widget = connectors_and_planes3D.back().second[i];
      widget->SetInteractor(renderWindowInteractor);
      // double bounds[6] = {
      //   viewports[n][0], viewports[n][1], viewports[n][2], viewports[n][3], 0, 0};
      // widget->PlaceWidget(bounds);
      widget->PlaceWidget();
      //TODO n is wrong here
      auto currentRenderer = renderers[n];
      widget->SetCurrentRenderer(currentRenderer);
      // widget->UpdatePlacement();
      widget->On();
      double worldPt[4];
      double x(0),y(0),z(0);
      widget->ComputeDisplayToWorld(currentRenderer, x, y, z, worldPt);
      std::cout << "worldPt at " << n <<
        ". " << worldPt[0] << "," << worldPt[2]<< "," <<worldPt[2]<<  "," <<worldPt[3]<< std::endl;
      double displayPt[3];
      widget->ComputeWorldToDisplay(currentRenderer, 0, 0, 0, displayPt);
      std::cout << "displayPt at " << n <<
        ". " << displayPt[0] << "," << displayPt[2]<< "," <<displayPt[2]<<  std::endl;
      }
  }
  renderWindow->Render();
  renderWindowInteractor->Initialize();

  renderWindowInteractor->Start();
}

template< typename PixelType >
template< typename TImage >
std::pair<typename ImageToVTKImageFilter<TImage>::Pointer, typename ViewImages< PixelType >::SlicesArrayType>
ViewImages< PixelType >
::GetConnectorAndSlices(TImage * inputImage)
{
  using ConnectorType = ImageToVTKImageFilter< TImage >;
  auto connector = ConnectorType::New();
  connector->SetInput(inputImage);
  connector->Update();
  connector->UpdateLargestPossibleRegion();

  // Prepare for slices.
  using FilterType = StatisticsImageFilter< TImage>;
  auto filter = FilterType::New();
  filter->SetInput(inputImage);
  filter->Update();
  filter->UpdateLargestPossibleRegion();
  double minIntensity = filter->GetMinimum();
  double maxIntensity = filter->GetMaximum();
  double window = maxIntensity - minIntensity;
  double level  = minIntensity + window / 2;
  /** SLICES */
  SlicesArrayType slicePlanes;
  for ( unsigned i = 0; i < 3; ++i )
    {
    slicePlanes[i] = vtkSmartPointer< vtkImagePlaneWidget >::New();
    slicePlanes[i]->SetResliceInterpolateToCubic();
    slicePlanes[i]->DisplayTextOn();
    // slicePlanes[i]->SetInteractor(renderWindowInteractor);
    // slicePlanes[i]->PlaceWidget();
    slicePlanes[i]->SetSliceIndex(0);
    slicePlanes[i]->SetMarginSizeX(0);
    slicePlanes[i]->SetMarginSizeY(0);
    slicePlanes[i]->SetRightButtonAction(
      vtkImagePlaneWidget::VTK_SLICE_MOTION_ACTION);
    slicePlanes[i]->SetMiddleButtonAction(
      vtkImagePlaneWidget::VTK_WINDOW_LEVEL_ACTION);
    slicePlanes[i]->TextureInterpolateOff();

    slicePlanes[i]->SetInputData(connector->GetOutput());
    slicePlanes[i]->SetPlaneOrientation(i);
    slicePlanes[i]->UpdatePlacement();
    slicePlanes[i]->SetWindowLevel(window, level);
    // slicePlanes[i]->On();
    }

  return std::make_pair(connector, slicePlanes);
}
}// namespace itk
#endif
