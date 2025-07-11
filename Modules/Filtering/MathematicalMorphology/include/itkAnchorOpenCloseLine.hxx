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
#ifndef itkAnchorOpenCloseLine_hxx
#define itkAnchorOpenCloseLine_hxx


namespace itk
{
template <typename TInputPix, typename TCompare>
AnchorOpenCloseLine<TInputPix, TCompare>::AnchorOpenCloseLine() = default;

template <typename TInputPix, typename TCompare>
void
AnchorOpenCloseLine<TInputPix, TCompare>::DoLine(std::vector<InputImagePixelType> & buffer, unsigned int bufflength)
{
  // TFunction1 will be >= for openings
  // TFunction2 will be <=
  // TFunction3 will be >

  // the initial version will adopt the methodology of loading a line
  // at a time into a buffer vector, carrying out the opening or
  // closing, and then copy the result to the output. Hopefully this
  // will improve cache performance when working along non raster
  // directions.
  if (bufflength <= m_Size / 2)
  {
    // No point doing anything fancy - just look for the extreme value
    // This is important for angled structuring elements
    InputImagePixelType Extreme = buffer[0];
    for (unsigned int i = 0; i < bufflength; ++i)
    {
      if (Compare1(Extreme, buffer[i]))
      {
        Extreme = buffer[i];
      }
    }

    for (unsigned int i = 0; i < bufflength; ++i)
    {
      buffer[i] = Extreme;
    }
    return;
  }

  // start the real work - everything here will be done with index
  // arithmetic rather than pointer arithmetic
  unsigned int outLeftP = 0;
  unsigned int outRightP = bufflength - 1;
  // left side
  while ((outLeftP < outRightP) && Compare1(buffer[outLeftP], buffer[outLeftP + 1]))
  {
    ++outLeftP;
  }
  while ((outLeftP < outRightP) && Compare2(buffer[outRightP - 1], buffer[outRightP]))
  {
    --outRightP;
  }
  InputImagePixelType Extreme;
  while (StartLine(buffer, Extreme, outLeftP, outRightP))
  {
  }

  FinishLine(buffer, Extreme, outLeftP, outRightP);
  // this section if to make the edge behaviour the same as the more
  // traditional approaches. It isn't part of the core anchor method.
  // Note that the index calculations include some extra factors that
  // relate to the padding at either end to allow users to set
  // borders.
  // compat
  // fix left border
  Extreme = buffer[m_Size / 2 + 1];
  for (int i = m_Size / 2; i >= 0; i--)
  {
    if (Compare1(Extreme, buffer[i]))
    {
      Extreme = buffer[i];
    }
    //    std::cout << i << ' ' << static_cast<int>(Extreme) << ' ' << static_cast<int>(buffer[i]) <<
    // std::endl;
    buffer[i] = Extreme;
  }
  // fix right border
  Extreme = buffer[bufflength - m_Size / 2 - 2];
  for (int i = static_cast<int>(bufflength) - m_Size / 2 - 1; i < static_cast<int>(bufflength); ++i)
  {
    if (Compare1(Extreme, buffer[i]))
    {
      Extreme = buffer[i];
    }
    //    std::cout << static_cast<int>(Extreme) << ' ' << static_cast<int>(buffer[i]) << std::endl;
    buffer[i] = Extreme;
  }
}

template <typename TInputPix, typename TCompare>
bool
AnchorOpenCloseLine<TInputPix, TCompare>::StartLine(std::vector<InputImagePixelType> & buffer,
                                                    InputImagePixelType &              Extreme,
                                                    unsigned int &                     outLeftP,
                                                    unsigned int &                     outRightP)
{
  // This returns true to indicate return to startLine label in pseudo
  // code, and false to indicate finishLine
  Extreme = buffer[outLeftP];
  unsigned int currentP = outLeftP + 1;


  while ((currentP < outRightP) && Compare2(buffer[currentP], Extreme))
  {
    Extreme = buffer[currentP];
    ++outLeftP;
    ++currentP;
  }
  const unsigned int sentinel = outLeftP + m_Size;
  if (sentinel > outRightP)
  {
    // finish
    return (false);
  }
  ++currentP;
  // ran m_Size pixels ahead
  while (currentP < sentinel)
  {
    if (Compare2(buffer[currentP], Extreme))
    {
      const unsigned int endP = currentP;
      for (unsigned int PP = outLeftP + 1; PP < endP; ++PP)
      {
        buffer[PP] = Extreme;
      }
      outLeftP = currentP;
      return (true);
    }
    ++currentP;
  }
  // We didn't find a smaller (for opening) value in the segment of
  // reach of outLeftP. currentP is the first position outside the
  // reach of outLeftP
  HistogramType histo;
  if (Compare2(buffer[currentP], Extreme))
  {
    const unsigned int endP = currentP;
    for (unsigned int PP = outLeftP + 1; PP < endP; ++PP)
    {
      buffer[PP] = Extreme;
    }
    outLeftP = currentP;
    return (true);
  }

  // Now we need a histogram
  // Initialise it
  ++outLeftP;
  for (unsigned int aux = outLeftP; aux <= currentP; ++aux)
  {
    histo.AddPixel(buffer[aux]);
  }
  // find the minimum value. The version
  // in the paper assumes integer pixel types and initializes the
  // search to the current extreme. Hopefully the latter is an
  // optimization step.
  Extreme = histo.GetValue();
  histo.RemovePixel(buffer[outLeftP]);
  buffer[outLeftP] = Extreme;
  histo.AddPixel(Extreme);


  while (currentP < outRightP)
  {
    ++currentP;
    if (Compare2(buffer[currentP], Extreme))
    {
      // Found a new extreme
      const unsigned int endP = currentP;
      for (unsigned int PP = outLeftP + 1; PP < endP; ++PP)
      {
        buffer[PP] = Extreme;
      }
      outLeftP = currentP;
      return (true);
    }

    /* histogram update */
    histo.AddPixel(buffer[currentP]);
    histo.RemovePixel(buffer[outLeftP]);
    Extreme = histo.GetValue();
    ++outLeftP;
    histo.RemovePixel(buffer[outLeftP]);
    buffer[outLeftP] = Extreme;
    histo.AddPixel(Extreme);
  }
  // Finish the line
  while (outLeftP < outRightP)
  {
    histo.RemovePixel(buffer[outLeftP]);
    Extreme = histo.GetValue();
    ++outLeftP;
    histo.RemovePixel(buffer[outLeftP]);
    buffer[outLeftP] = Extreme;
    histo.AddPixel(Extreme);
  }
  return (false);
}

template <typename TInputPix, typename TCompare>
void
AnchorOpenCloseLine<TInputPix, TCompare>::FinishLine(std::vector<InputImagePixelType> & buffer,
                                                     InputImagePixelType &              Extreme,
                                                     unsigned int &                     outLeftP,
                                                     unsigned int &                     outRightP)
{
  while (outLeftP < outRightP)
  {
    if (Compare2(buffer[outLeftP], buffer[outRightP]))
    {
      Extreme = buffer[outRightP];
      --outRightP;
      if (!Compare2(buffer[outRightP], Extreme))
      {
        buffer[outRightP] = Extreme;
      }
    }
    else
    {
      Extreme = buffer[outLeftP];
      ++outLeftP;
      if (!Compare2(buffer[outLeftP], Extreme))
      {
        buffer[outLeftP] = Extreme;
      }
    }
  }
}

template <typename TInputPix, typename TCompare>
void
AnchorOpenCloseLine<TInputPix, TCompare>::PrintSelf(std::ostream & os, Indent indent) const
{
  os << indent << "Size: " << m_Size << std::endl;
}
} // end namespace itk

#endif
