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
#ifndef itkNormalQuadEdgeMeshFilter_hxx
#define itkNormalQuadEdgeMeshFilter_hxx

#include "itkMath.h"

namespace itk
{
template <typename TInputMesh, typename TOutputMesh>
NormalQuadEdgeMeshFilter<TInputMesh, TOutputMesh>::NormalQuadEdgeMeshFilter()
  : m_Weight(WeightEnum::THURMER)
{}

template <typename TInputMesh, typename TOutputMesh>
auto
NormalQuadEdgeMeshFilter<TInputMesh, TOutputMesh>::ComputeFaceNormal(OutputPolygonType * iPoly) -> OutputFaceNormalType
{
  const OutputMeshPointer output = this->GetOutput();

  OutputPointType pt[3];
  int             k(0);

  OutputQEType * edge = iPoly->GetEdgeRingEntry();
  OutputQEType * temp = edge;

  do
  {
    pt[k++] = output->GetPoint(temp->GetOrigin());
    temp = temp->GetLnext();
  } while (temp != edge);

  return TriangleType::ComputeNormal(pt[0], pt[1], pt[2]);
}

template <typename TInputMesh, typename TOutputMesh>
void
NormalQuadEdgeMeshFilter<TInputMesh, TOutputMesh>::ComputeAllFaceNormals()
{
  const OutputMeshPointer output = this->GetOutput();

  for (OutputCellsContainerConstIterator cell_it = output->GetCells()->Begin(); cell_it != output->GetCells()->End();
       ++cell_it)
  {
    auto * poly = dynamic_cast<OutputPolygonType *>(cell_it.Value());

    if (poly != nullptr)
    {
      if (poly->GetNumberOfPoints() == 3)
      {
        output->SetCellData(cell_it->Index(), ComputeFaceNormal(poly));
      }
    }
  }
}

template <typename TInputMesh, typename TOutputMesh>
void
NormalQuadEdgeMeshFilter<TInputMesh, TOutputMesh>::ComputeAllVertexNormals()
{
  const OutputMeshPointer            output = this->GetOutput();
  const OutputPointsContainerPointer points = output->GetPoints();

  OutputMeshType * outputMesh = this->GetOutput();

  for (OutputPointsContainerIterator it = points->Begin(); it != points->End(); ++it)
  {
    OutputPointIdentifier id = it->Index();
    output->SetPointData(id, ComputeVertexNormal(id, outputMesh));
  }
}

template <typename TInputMesh, typename TOutputMesh>
auto
NormalQuadEdgeMeshFilter<TInputMesh, TOutputMesh>::ComputeVertexNormal(const OutputPointIdentifier & iId,
                                                                       OutputMeshType *              outputMesh)
  -> OutputVertexNormalType
{

  OutputQEType *       edge = outputMesh->FindEdge(iId);
  OutputQEType *       temp = edge;
  OutputCellIdentifier cell_id(0);

  OutputVertexNormalType n(0.);
  OutputFaceNormalType   face_normal(0.);

  do
  {
    cell_id = temp->GetLeft();
    if (cell_id != OutputMeshType::m_NoFace)
    {
      outputMesh->GetCellData(cell_id, &face_normal);
      n += face_normal * Weight(iId, cell_id, outputMesh);
    }
    temp = temp->GetOnext();
  } while (temp != edge);

  n.Normalize();
  return n;
}

template <typename TInputMesh, typename TOutputMesh>
auto
NormalQuadEdgeMeshFilter<TInputMesh, TOutputMesh>::Weight(const OutputPointIdentifier & iPId,
                                                          const OutputCellIdentifier &  iCId,
                                                          OutputMeshType *              outputMesh)
  -> OutputVertexNormalComponentType
{
  if (m_Weight == WeightEnum::GOURAUD)
  {
    return static_cast<OutputVertexNormalComponentType>(1.0);
  }


  auto * poly = dynamic_cast<OutputPolygonType *>(outputMesh->GetCells()->GetElement(iCId));
  if (poly != nullptr) // this test should be removed...
  {
    // this test should be removed...
    if (poly->GetNumberOfPoints() == 3)
    {
      OutputQEType *  edge = poly->GetEdgeRingEntry();
      OutputQEType *  temp = edge;
      OutputPointType pt[3];
      int             internal_id(0);
      int             k(0);
      do
      {
        pt[k] = outputMesh->GetPoint(temp->GetOrigin());
        if (temp->GetOrigin() == iPId)
        {
          internal_id = k;
        }

        temp = temp->GetLnext();
        ++k;
      } while (temp != edge);

      switch (m_Weight)
      {
        default:
        case WeightEnum::GOURAUD:
        {
          return static_cast<OutputVertexNormalComponentType>(1.);
        }
        case WeightEnum::THURMER:
        {
          // this implementation may be included inside itkTriangle
          OutputVectorType u;
          OutputVectorType v;
          switch (internal_id)
          {
            case 0:
              u = pt[1] - pt[0];
              v = pt[2] - pt[0];
              break;
            case 1:
              u = pt[0] - pt[1];
              v = pt[2] - pt[1];
              break;
            case 2:
              u = pt[0] - pt[2];
              v = pt[1] - pt[2];
              break;
          }
          typename OutputVectorType::RealValueType norm_u = u.GetNorm();
          if (norm_u > itk::Math::eps)
          {
            norm_u = 1. / norm_u;
            u *= norm_u;
          }

          typename OutputVectorType::RealValueType norm_v = v.GetNorm();
          if (norm_v > itk::Math::eps)
          {
            norm_v = 1. / norm_v;
            v *= norm_v;
          }
          return static_cast<OutputVertexNormalComponentType>(std::acos(u * v));
        }
        case WeightEnum::AREA:
        {
          return static_cast<OutputVertexNormalComponentType>(TriangleType::ComputeArea(pt[0], pt[1], pt[2]));
        }
      }
    }
    else
    {
      std::cout << "Input should be a triangular mesh!!!" << std::endl;
      return static_cast<OutputVertexNormalComponentType>(0.);
    }
  }
  else
  {
    return static_cast<OutputVertexNormalComponentType>(0.);
  }
}

template <typename TInputMesh, typename TOutputMesh>
void
NormalQuadEdgeMeshFilter<TInputMesh, TOutputMesh>::GenerateData()
{
  this->CopyInputMeshToOutputMesh();
  this->ComputeAllFaceNormals();
  this->ComputeAllVertexNormals();
}

template <typename TInputMesh, typename TOutputMesh>
void
NormalQuadEdgeMeshFilter<TInputMesh, TOutputMesh>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  std::cout << indent << "Weight: " << m_Weight << std::endl;
}
} // namespace itk

#endif
