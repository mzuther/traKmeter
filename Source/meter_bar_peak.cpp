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

#include "meter_bar_peak.h"


void MeterBarPeak::create(int crestFactor, Orientation orientation,
                          bool discreteMeter, bool showCombinedMeters,
                          int segmentHeight)

{
    GenericMeterBar::create();

    segmentColours_.clear();

    segmentColours_.add(Colour(0.00f, 1.0f, 1.0f, 1.0f));  // red
    segmentColours_.add(Colour(0.18f, 1.0f, 1.0f, 1.0f));  // yellow
    segmentColours_.add(Colour(0.30f, 1.0f, 1.0f, 1.0f));  // green
    segmentColours_.add(Colour(0.58f, 1.0f, 1.0f, 1.0f));  // blue

    crestFactor *= 10;
    int nTrueLowerThreshold;
    int nNumberOfBars;

    if (showCombinedMeters)
    {
        nTrueLowerThreshold = -90;
        nNumberOfBars = 26;
    }
    else
    {
        nTrueLowerThreshold = -90;
        nNumberOfBars = 9;
    }

    int nLowerThreshold = nTrueLowerThreshold + crestFactor;

    for (int n = 0; n < nNumberOfBars; ++n)
    {
        int nThresholdDifference;

        if (nTrueLowerThreshold > -260)
        {
            nThresholdDifference = 10;
        }
        else if (nTrueLowerThreshold > -300)
        {
            nThresholdDifference = 40;
        }
        else
        {
            nThresholdDifference = 100;
        }

        nTrueLowerThreshold -= nThresholdDifference;
        nLowerThreshold = nTrueLowerThreshold + crestFactor;

        int nColour;

        if (nTrueLowerThreshold >= -90)
        {
            nColour = 0;
        }
        else if (nTrueLowerThreshold >= -100)
        {
            nColour = 1;
        }
        else if (nTrueLowerThreshold >= -160)
        {
            nColour = 2;
        }
        else
        {
            nColour = 3;
        }

        float fLowerThreshold = nLowerThreshold * 0.1f;
        float fRange = nThresholdDifference * 0.1f;

        int nSpacingBefore = 0;
        bool hasHighestLevel;

        if (showCombinedMeters)
        {
            hasHighestLevel = (n == 0) ? true : false;
        }
        else
        {
            hasHighestLevel = false;
        }

        if (discreteMeter)
        {
            addDiscreteSegment(
                fLowerThreshold,
                fRange,
                hasHighestLevel,
                segmentHeight,
                nSpacingBefore,
                segmentColours_[nColour],
                segmentColours_[nColour].withAlpha(0.7f));
        }
        else
        {
            addContinuousSegment(
                fLowerThreshold,
                fRange,
                hasHighestLevel,
                segmentHeight,
                nSpacingBefore,
                segmentColours_[nColour],
                segmentColours_[nColour].withAlpha(0.7f));
        }
    }

    // set orientation here to save some processing power
    setOrientation(orientation);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
