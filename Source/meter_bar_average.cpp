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

#include "meter_bar_average.h"

MeterBarAverage::MeterBarAverage()
{
    arrHues.add(0.00f);  // red
    arrHues.add(0.18f);  // yellow
    arrHues.add(0.30f);  // green
    arrHues.add(0.58f);  // blue

    for (int n = 0; n < arrHues.size(); ++n)
    {
        arrPeakColours.add(Colour(arrHues[n], 1.0f, 1.0f, 0.7f));
    }
}


void MeterBarAverage::create(int crestFactor, int nMainSegmentHeight, Orientation orientation, bool bShowCombinedMeters)
{
    GenericMeterBar::create();

    int nCrestFactor = crestFactor * 10;
    int nTrueLowerThreshold;
    int nNumberOfBars;

    if (bShowCombinedMeters)
    {
        nTrueLowerThreshold = -90;
        nNumberOfBars = 26;
    }
    else
    {
        nTrueLowerThreshold = -160;
        nNumberOfBars = 8;
    }

    int nLowerThreshold = nTrueLowerThreshold + nCrestFactor;

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

        int nColour;

        nTrueLowerThreshold -= nThresholdDifference;
        nLowerThreshold = nTrueLowerThreshold + nCrestFactor;

        if (nTrueLowerThreshold >= -170)
        {
            nColour = 0;
        }
        else if (nTrueLowerThreshold >= -180)
        {
            nColour = 1;
        }
        else if (nTrueLowerThreshold >= -220)
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
        bool bHasHighestLevel = (n == 0) ? true : false;

        addSegment(fLowerThreshold, fRange, bHasHighestLevel, nMainSegmentHeight, nSpacingBefore, arrHues[nColour], arrPeakColours[nColour]);
    }

    // set orientation here to save some processing power
    setOrientation(orientation);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
