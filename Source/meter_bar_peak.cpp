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

#include "meter_bar_peak.h"

MeterBarPeak::MeterBarPeak(int number_of_bars, int crest_factor, int segment_height, bool show_combined_meters)
{
    // this component does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    nNumberOfBars = number_of_bars;
    nSegmentHeight = segment_height;

    Array<float> arrHues;

    arrHues.add(0.00f);  // red
    arrHues.add(0.18f);  // yellow
    arrHues.add(0.30f);  // green
    arrHues.add(0.58f);  // blue

    fPeakLevel = -9999.8f;
    fPeakLevelPeak = -9999.8f;
    fPeakLevelMaximum = -9999.8f;

    int nCrestFactor = 10 * crest_factor;
    int nTrueThreshold = show_combined_meters ? -90 : -90;
    int nThreshold = nTrueThreshold + nCrestFactor;

    for (int n = 0; n < nNumberOfBars; n++)
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

        nTrueThreshold -= nThresholdDifference;
        nThreshold = nTrueThreshold + nCrestFactor;
        float fRange = nThresholdDifference / 10.0f;

        int nColor;

        if (nTrueThreshold >= -90)
        {
            nColor = 0;
        }
        else if (nTrueThreshold >= -100)
        {
            nColor = 1;
        }
        else if (nTrueThreshold >= -160)
        {
            nColor = 2;
        }
        else
        {
            nColor = 3;
        }

        GenericMeterSegment *pMeterSegment = p_arrMeterSegments.add(new GenericMeterSegment());
        pMeterSegment->setThresholds(nThreshold * 0.1f, fRange);
        pMeterSegment->setColour(arrHues[nColor], Colour(arrHues[nColor], 1.0f, 1.0f, 0.7f));

        addAndMakeVisible(pMeterSegment);
    }
}


MeterBarPeak::~MeterBarPeak()
{
}


void MeterBarPeak::paint(Graphics &g)
{
    g.fillAll(Colours::black);
}


void MeterBarPeak::resized()
{
    int y = 0;

    for (int n = 0; n < nNumberOfBars; n++)
    {
        p_arrMeterSegments[n]->setBounds(0, y, getWidth(), nSegmentHeight + 1);
        y += nSegmentHeight;
    }
}


void MeterBarPeak::setLevels(float peakLevel, float peakLevelPeak)
{
    if ((peakLevel != fPeakLevel) || (peakLevelPeak != fPeakLevelPeak))
    {
        fPeakLevel = peakLevel;
        fPeakLevelPeak = peakLevelPeak;

        for (int n = 0; n < nNumberOfBars; n++)
        {
            p_arrMeterSegments[n]->setNormalLevels(fPeakLevel, fPeakLevelPeak);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
