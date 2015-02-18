/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2015 Martin Zuther (http://www.mzuther.de/)

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

MeterBarAverage::MeterBarAverage(const String &componentName, int number_of_bars, int crest_factor, int segment_height, bool display_peaks, bool show_combined_meters)
{
    setName(componentName);

    // this component does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    nNumberOfBars = number_of_bars;
    nSegmentHeight = segment_height;

    int nCrestFactor = 10 * crest_factor;
    fAverageLevel = 0.0f;
    fAverageLevelPeak = 0.0f;

    int n = 0;
    int nThreshold = show_combined_meters ? -100 : -170;
    nThreshold += nCrestFactor;
    int nTrueThreshold = nThreshold - nCrestFactor;

    // register all hot signals, even up to +100 dB FS!
    float fRange = (nCrestFactor - nThreshold) * 0.1f + 100.0f;
    int nColor = 0;
    bool bDiscreteLevels = true;

    MeterSegment *pMeterSegment = p_arrMeterSegments.add(new MeterSegment("MeterSegment #" + String(n) + " (" + componentName + ")", nThreshold * 0.1f, fRange, bDiscreteLevels, display_peaks, nColor));
    addAndMakeVisible(pMeterSegment);

    for (n = 1; n < nNumberOfBars; n++)
    {
        int nThresholdDifference;

        if (nTrueThreshold > -260)
        {
            nThresholdDifference = 10;
        }
        else if (nTrueThreshold > -300)
        {
            nThresholdDifference = 40;
        }
        else
        {
            nThresholdDifference = 100;
        }

        nThreshold -= nThresholdDifference;
        fRange = nThresholdDifference / 10.0f;
        bDiscreteLevels = false;

        nTrueThreshold = nThreshold - nCrestFactor;

        if (nTrueThreshold >= -170)
        {
            nColor = 0;
        }
        else if (nTrueThreshold >= -180)
        {
            nColor = 1;
        }
        else if (nTrueThreshold >= -220)
        {
            nColor = 2;
        }
        else
        {
            nColor = 3;
        }

        pMeterSegment = p_arrMeterSegments.add(new MeterSegment("MeterSegment #" + String(n) + " (" + componentName + ")", nThreshold * 0.1f, fRange, bDiscreteLevels, display_peaks, nColor));
        addAndMakeVisible(pMeterSegment);
    }
}


MeterBarAverage::~MeterBarAverage()
{
}


void MeterBarAverage::paint(Graphics &g)
{
    g.fillAll(Colours::black);
}


void MeterBarAverage::resized()
{
    int y = 0;

    for (int n = 0; n < nNumberOfBars; n++)
    {
        p_arrMeterSegments[n]->setBounds(0, y, getWidth(), nSegmentHeight + 1);
        y += nSegmentHeight;
    }
}


void MeterBarAverage::setLevels(float averageLevel, float averageLevelPeak)
{
    if ((averageLevel != fAverageLevel) || (averageLevelPeak != fAverageLevelPeak))
    {
        fAverageLevel = averageLevel;
        fAverageLevelPeak = averageLevelPeak;

        for (int n = 0; n < nNumberOfBars; n++)
        {
            p_arrMeterSegments[n]->setLevels(fAverageLevel, fAverageLevelPeak);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
