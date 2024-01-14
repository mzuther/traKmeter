/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2024 Martin Zuther (http://www.mzuther.de/)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Thank you for using free software!

---------------------------------------------------------------------------- */

#ifndef TRAKMETER_METER_BAR_AVERAGE_H
#define TRAKMETER_METER_BAR_AVERAGE_H

#include "FrutHeader.h"


class MeterBarAverage :
   public frut::widgets::MeterBar
{
public:
   using frut::widgets::MeterBar::create;
   void create( float retainSignalFactor,
                float newSignalFactor,
                frut::widgets::Orientation orientation,
                bool discreteMeter,
                int targetRecordingLevel,
                int mainSegmentHeight,
                const Array<Colour>& segmentColours );

protected:
   /// Colour ID selector for meter segments.
   enum colourSelector { // protected namespace
      /// overload (usually red)
      overload = 0,

      /// warning (usually yellow)
      warning,

      /// fine (usually green)
      fine,

      /// signal (usually blue)
      signal
   };

private:
   JUCE_LEAK_DETECTOR( MeterBarAverage );
};

#endif  // TRAKMETER_METER_BAR_AVERAGE_H
