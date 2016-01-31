/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2016 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __METER_BAR_PEAK_H__
#define __METER_BAR_PEAK_H__

#include "JuceHeader.h"
#include "common/widgets/generic_meter_bar.h"


class MeterBarPeak :
    public GenericMeterBar
{
public:
    void create(int crestFactor,
                Orientation orientation,
                bool discreteMeter,
                bool showCombinedMeters,
                int mainSegmentHeight,
                const Array<Colour> &segmentColours);

protected:
    /// Colour ID selector for meter segments.
    enum colourSelector  // protected namespace
    {
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
    JUCE_LEAK_DETECTOR(MeterBarPeak);
};


#endif  // __METER_BAR_PEAK_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
