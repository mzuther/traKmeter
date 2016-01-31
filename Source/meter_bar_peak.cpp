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
                          int mainSegmentHeight)

{
    GenericMeterBar::create();

    Array<Colour> segmentColours;

    segmentColours.add(Colour(0.00f, 1.0f, 1.0f, 1.0f));  // red
    segmentColours.add(Colour(0.18f, 1.0f, 1.0f, 1.0f));  // yellow
    segmentColours.add(Colour(0.30f, 1.0f, 1.0f, 1.0f));  // green
    segmentColours.add(Colour(0.58f, 1.0f, 1.0f, 1.0f));  // blue

    crestFactor *= 10;
    int trueLowerThreshold;
    int numberOfBars;

    if (showCombinedMeters)
    {
        trueLowerThreshold = -90;
        numberOfBars = 26;
    }
    else
    {
        trueLowerThreshold = -90;
        numberOfBars = 9;
    }

    int lowerThreshold = trueLowerThreshold + crestFactor;

    for (int n = 0; n < numberOfBars; ++n)
    {
        int thresholdDifference;

        if (trueLowerThreshold > -260)
        {
            thresholdDifference = 10;
        }
        else if (trueLowerThreshold > -300)
        {
            thresholdDifference = 40;
        }
        else
        {
            thresholdDifference = 100;
        }

        trueLowerThreshold -= thresholdDifference;
        lowerThreshold = trueLowerThreshold + crestFactor;

        int colourId;

        if (trueLowerThreshold >= -90)
        {
            colourId = 0;
        }
        else if (trueLowerThreshold >= -100)
        {
            colourId = 1;
        }
        else if (trueLowerThreshold >= -160)
        {
            colourId = 2;
        }
        else
        {
            colourId = 3;
        }

        int segmentHeight = mainSegmentHeight;
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
            // meter segment outlines overlap
            int spacingBefore = -1;
            segmentHeight += 1;

            addDiscreteSegment(
                lowerThreshold * 0.1f,
                thresholdDifference * 0.1f,
                hasHighestLevel,
                segmentHeight,
                spacingBefore,
                segmentColours[colourId],
                segmentColours[colourId].withMultipliedBrightness(0.7f));
        }
        else
        {
            // meter segment outlines must not overlap
            int spacingBefore = 0;

            addContinuousSegment(
                lowerThreshold * 0.1f,
                thresholdDifference * 0.1f,
                hasHighestLevel,
                segmentHeight,
                spacingBefore,
                segmentColours[colourId],
                segmentColours[colourId].withMultipliedBrightness(0.7f));
        }
    }

    // set orientation here to save some processing power
    setOrientation(orientation);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
