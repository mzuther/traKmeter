/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2013 Martin Zuther (http://www.mzuther.de/)

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

MeterBarPeak::MeterBarPeak(const String& componentName, int pos_x, int pos_y, int width, int number_of_bars, int crest_factor, int segment_height, bool display_peaks, bool show_combined_meters)
{
    setName(componentName);

    // this component does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    // the upper bar is an overload meter
    nNumberOfBars = number_of_bars - 1;
    nSegmentHeight = segment_height;

    nPosX = pos_x;
    nPosY = pos_y;
    nWidth = width;
    nHeight = (nNumberOfBars + 1) * nSegmentHeight + 1;

    int nCrestFactor = 10 * crest_factor;
    fPeakLevel = -9999.8f;
    fPeakLevelPeak = -9999.8f;
    fPeakLevelMaximum = -9999.8f;

    pMeterSegments = new MeterSegment*[nNumberOfBars];

    int nThreshold = show_combined_meters ? -50 : -90;
    nThreshold += nCrestFactor;
    int nTrueThreshold = nThreshold - nCrestFactor;

    // register all hot signals, even up to +100 dB FS!
    float fRange = (nCrestFactor + 100.0f - nThreshold) * 0.1f;

    int nColor = 0;
    bool bDiscreteLevels = true;

    pMeterSegmentOverload = new MeterSegmentOverload("MeterSegmentOverload (" + componentName + ")", nThreshold * 0.1f, fRange, bDiscreteLevels, display_peaks, nColor);
    addAndMakeVisible(pMeterSegmentOverload);

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

        nThreshold -= nThresholdDifference;
        fRange = nThresholdDifference / 10.0f;
        bDiscreteLevels = false;

        nTrueThreshold = nThreshold - nCrestFactor;

        if (nTrueThreshold >= -90)
        {
            nColor = 0;
        }
        else if ((nTrueThreshold < -160) || (nTrueThreshold >= -100))
        {
            nColor = 1;
        }
        else
        {
            nColor = 2;
        }

        pMeterSegments[n] = new MeterSegment("MeterSegment #" + String(n) + " (" + componentName + ")", nThreshold * 0.1f, fRange, bDiscreteLevels, display_peaks, nColor);
        addAndMakeVisible(pMeterSegments[n]);
    }
}


MeterBarPeak::~MeterBarPeak()
{
    for (int n = 0; n < nNumberOfBars; n++)
    {
        removeChildComponent(pMeterSegments[n]);
        delete pMeterSegments[n];
        pMeterSegments[n] = NULL;
    }

    delete [] pMeterSegments;
    pMeterSegments = NULL;

    delete pMeterSegmentOverload;
    pMeterSegmentOverload = NULL;

    deleteAllChildren();
}


void MeterBarPeak::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);

    int x = 0;
    int y = 0;

    pMeterSegmentOverload->setBounds(x, y, nWidth, nSegmentHeight + 1);
    y += nSegmentHeight;

    for (int n = 0; n < nNumberOfBars; n++)
    {
        pMeterSegments[n]->setBounds(x, y, nWidth, nSegmentHeight + 1);
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


void MeterBarPeak::setLevels(float peakLevel, float peakLevelPeak, float peakLevelMaximum)
{
    if ((peakLevel != fPeakLevel) || (peakLevelPeak != fPeakLevelPeak) || (fPeakLevelMaximum != peakLevelMaximum))
    {
        fPeakLevel = peakLevel;
        fPeakLevelPeak = peakLevelPeak;
        fPeakLevelMaximum = peakLevelMaximum;

        pMeterSegmentOverload->setLevels(fPeakLevel, fPeakLevelPeak, fPeakLevelMaximum);

        for (int n = 0; n < nNumberOfBars; n++)
        {
            pMeterSegments[n]->setLevels(fPeakLevel, fPeakLevelPeak);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
