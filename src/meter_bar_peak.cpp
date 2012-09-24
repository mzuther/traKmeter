/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012 Martin Zuther (http://www.mzuther.de/)

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

MeterBarPeak::MeterBarPeak(const String& componentName, int pos_x, int pos_y, int width, int number_of_bars, int crest_factor, int segment_height, bool display_peaks)
{
    setName(componentName);

    // this component does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    nNumberOfBars = number_of_bars;
    nSegmentHeight = segment_height;

    nPosX = pos_x;
    nPosY = pos_y;
    nWidth = width;
    nHeight = nNumberOfBars * nSegmentHeight + 1;

    int nCrestFactor = 10 * crest_factor;
    fPeakLevel = 0.0f;
    fPeakLevelPeak = 0.0f;

    MeterArray = new MeterSegment*[nNumberOfBars];

    int n = 0;
    int nThreshold = 110;
    float fRange = (nCrestFactor - nThreshold) * 0.1f;
    int nColor = 0;
    bool bDiscreteLevels = true;

    MeterArray[n] = new MeterSegment("MeterSegment #" + String(n) + " (" + componentName + ")", (nThreshold - nCrestFactor) * 0.1f, fRange, bDiscreteLevels, display_peaks, nColor);
    addAndMakeVisible(MeterArray[n]);

    for (int n = 1; n < (nNumberOfBars - 1); n++)
    {
        int nThresholdDifference = 10;
        nThreshold -= nThresholdDifference;
        fRange = nThresholdDifference / 10.0f;
        bDiscreteLevels = false;

        if (nThreshold >= 100)
        {
            nColor = 0;
        }
        else if ((nThreshold >= 80) && (nThreshold < 100))
        {
            nColor = 1;
        }
        else
        {
            nColor = 2;
        }

        MeterArray[n] = new MeterSegment("MeterSegment #" + String(n) + " (" + componentName + ")", (nThreshold - nCrestFactor) * 0.1f, fRange, bDiscreteLevels, display_peaks, nColor);
        addAndMakeVisible(MeterArray[n]);
    }

    n = nNumberOfBars - 1;
    // signals are detected at -60 dB and above
    fRange = -(-600 + nCrestFactor - nThreshold) * 0.1f;
    nThreshold = -600 + nCrestFactor;
    nColor = 1;
    bDiscreteLevels = false;

    MeterArray[n] = new MeterSegment("MeterSegment #" + String(n) + " (" + componentName + ")", (nThreshold - nCrestFactor) * 0.1f, fRange, bDiscreteLevels, display_peaks, nColor);
    addAndMakeVisible(MeterArray[n]);
}


MeterBarPeak::~MeterBarPeak()
{
    for (int n = 0; n < nNumberOfBars; n++)
    {
        removeChildComponent(MeterArray[n]);
        delete MeterArray[n];
        MeterArray[n] = NULL;
    }

    delete [] MeterArray;
    MeterArray = NULL;

    deleteAllChildren();
}


void MeterBarPeak::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);

    int x = 0;
    int y = 0;

    for (int n = 0; n < nNumberOfBars; n++)
    {
        MeterArray[n]->setBounds(x, y, nWidth, nSegmentHeight + 1);
        y += nSegmentHeight;
    }
}


void MeterBarPeak::paint(Graphics& g)
{
    g.fillAll(Colours::black);
}


void MeterBarPeak::resized()
{
}


void MeterBarPeak::setLevels(float peakLevel, float peakLevelPeak)
{
    if ((peakLevel != fPeakLevel) || (peakLevelPeak != fPeakLevelPeak))
    {
        fPeakLevel = peakLevel;
        fPeakLevelPeak = peakLevelPeak;

        for (int n = 0; n < nNumberOfBars; n++)
        {
            MeterArray[n]->setLevels(fPeakLevel, fPeakLevelPeak);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
