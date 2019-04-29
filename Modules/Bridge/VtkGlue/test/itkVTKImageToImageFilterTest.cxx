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

#include "itkVTKImageToImageFilter.h"

#include "vtkImageNoiseSource.h"
#include "vtkMatrix3x3.h"
#include "vtkSmartPointer.h"

int itkVTKImageToImageFilterTest(int, char*[])
{
  const int dim = 2;
  using ImageType = itk::Image<double, dim>;
  using VTKNoiseType = vtkSmartPointer<vtkImageNoiseSource>;
  using ConnectorType = itk::VTKImageToImageFilter<ImageType>;

  VTKNoiseType noise_source = VTKNoiseType::New();
  noise_source->SetWholeExtent(0,20,0,20,0,0);
  noise_source->SetMinimum(0.0);
  noise_source->SetMaximum(1.0);
  noise_source->Update();

  auto input = noise_source->GetOutput();
  input->SetSpacing(0.1, 2.0, 0.0);
  input->SetOrigin(-0.1, -10, 0.0);
  input->SetDirectionMatrix(0, 1, 0, -1, 0, 0, 0, 0, 1);
  input->Print(std::cout);

  ConnectorType::Pointer connector = ConnectorType::New();
  connector->SetInput(input);
  connector->Update();

  auto output = connector->GetOutput();
  output->Print(std::cout);

  for (int i = 0; i < dim; ++i)
  {
    if (output->GetLargestPossibleRegion().GetSize()[i] != static_cast<itk::SizeValueType>(input->GetDimensions()[i]))
    {
      std::cerr << "Error: sizes do not match for component (" << i << ")." << std::endl;
      return EXIT_FAILURE;
    }
    if (output->GetSpacing()[i] != input->GetSpacing()[i])
    {
      std::cerr << "Error: spacings do not match for component (" << i << ")." << std::endl;
      return EXIT_FAILURE;
    }
    if (output->GetOrigin()[i] != input->GetOrigin()[i])
    {
      std::cerr << "Error: origins do not match for component (" << i << ")." << std::endl;
      return EXIT_FAILURE;
    }
    for (int j = 0; j < dim; ++j)
    {
      if (output->GetDirection()[i][j] != input->GetDirectionMatrix()->GetData()[i*3+j])
      {
        std::cerr << "Error: directions do not match for component (" << i << "," << j << ")." << std::endl;
        return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}
