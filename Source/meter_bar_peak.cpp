/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2020 Martin Zuther (http://www.mzuther.de/)

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


void MeterBarPeak::create(
    float retainSignalFactor,
    float newSignalFactor,
    frut::widgets::Orientation orientation,
    bool discreteMeter,
    int targetRecordingLevel,
    int mainSegmentHeight,
    const Array<Colour> &segmentColours)
{
    frut::widgets::MeterBar::create();

    int numberOfBars = 15;
    bool hasHighestLevel = false;

    targetRecordingLevel *= 10;

    int trueLowerThreshold = targetRecordingLevel + 10;
    int overloadLevel = targetRecordingLevel;
    int warningLevel = targetRecordingLevel - 20;
    int fineLevel = targetRecordingLevel - 80;

    int lowerThreshold = trueLowerThreshold;
    int thresholdDifference = 10;

    for (int n = 0; n < numberOfBars; ++n)
    {
        trueLowerThreshold -= thresholdDifference;
        lowerThreshold = trueLowerThreshold;

        int colourId;

        if (trueLowerThreshold >= overloadLevel)
        {
            colourId = colourSelector::overload;
        }
        else if (trueLowerThreshold >= warningLevel)
        {
            colourId = colourSelector::warning;
        }
        else if (trueLowerThreshold >= fineLevel)
        {
            colourId = colourSelector::fine;
        }
        else
        {
            colourId = colourSelector::signal;
        }

        if (discreteMeter)
        {
            // meter segment outlines overlap
            int spacingBefore = -1;
            int segmentHeight = mainSegmentHeight - spacingBefore;

            addDiscreteSegment(
                lowerThreshold * 0.1f,
                thresholdDifference * 0.1f,
                retainSignalFactor,
                newSignalFactor,
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
            int segmentHeight = mainSegmentHeight - spacingBefore;

            addContinuousSegment(
                lowerThreshold * 0.1f,
                thresholdDifference * 0.1f,
                (thresholdDifference * 0.1f) / segmentHeight,
                hasHighestLevel,
                segmentHeight,
                spacingBefore,
                segmentColours[colourId],
                segmentColours[colourId]);
        }
    }

    // set orientation here to save some processing
    setOrientation(orientation);
}
