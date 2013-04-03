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

#include "meter_bar_average.h"

MeterBarAverage::MeterBarAverage(const String& componentName, int pos_x, int pos_y, int width, int number_of_bars, int crest_factor, int segment_height, bool display_peaks)
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
    fAverageLevel = 0.0f;
    fAverageLevelPeak = 0.0f;

    MeterArray = new MeterSegment*[nNumberOfBars];

    int n = 0;
    int nThreshold = -170 + nCrestFactor;

    // register all hot signals, even up to +100 dB FS!
    float fRange = (nCrestFactor + 100.0f - nThreshold) * 0.1f;
    int nColor = 0;
    bool bDiscreteLevels = true;

    MeterArray[n] = new MeterSegment("MeterSegment #" + String(n) + " (" + componentName + ")", nThreshold * 0.1f, fRange, bDiscreteLevels, display_peaks, nColor);
    addAndMakeVisible(MeterArray[n]);

    for (int n = 1; n < nNumberOfBars; n++)
    {
        int nThresholdDifference = 10;
        nThreshold -= nThresholdDifference;
        fRange = nThresholdDifference / 10.0f;
        bDiscreteLevels = false;

        int nTrueThreshold = nThreshold - nCrestFactor;

        if ((nTrueThreshold < -230) || (nTrueThreshold >= -170))
        {
            nColor = 0;
        }
        else if ((nTrueThreshold < -220) || (nTrueThreshold >= -180))
        {
            nColor = 1;
        }
        else
        {
            nColor = 2;
        }

        MeterArray[n] = new MeterSegment("MeterSegment #" + String(n) + " (" + componentName + ")", nThreshold * 0.1f, fRange, bDiscreteLevels, display_peaks, nColor);
        addAndMakeVisible(MeterArray[n]);
    }
}


MeterBarAverage::~MeterBarAverage()
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


void MeterBarAverage::visibilityChanged()
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


void MeterBarAverage::paint(Graphics& g)
{
    g.fillAll(Colours::black);
}


void MeterBarAverage::resized()
{
}


void MeterBarAverage::setLevels(float averageLevel, float averageLevelPeak)
{
    if ((averageLevel != fAverageLevel) || (averageLevelPeak != fAverageLevelPeak))
    {
        fAverageLevel = averageLevel;
        fAverageLevelPeak = averageLevelPeak;

        for (int n = 0; n < nNumberOfBars; n++)
        {
            MeterArray[n]->setLevels(fAverageLevel, fAverageLevelPeak);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
