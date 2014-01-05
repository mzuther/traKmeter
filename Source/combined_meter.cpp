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

#include "combined_meter.h"

CombinedMeter::CombinedMeter(const String& componentName, int posX, int posY, int width, int CrestFactor, int nNumChannels, int segment_height)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;
    nCrestFactor = CrestFactor;

    nNumberOfBars = 29;
    nSegmentHeight = segment_height;
    nMeterPositionTop = 0;
    nMeterHeight = nNumberOfBars * nSegmentHeight + 2;

    nPosX = posX;
    nPosY = posY;
    nWidth = width;
    nHeight = nMeterPositionTop + nMeterHeight;

    int nPositionX = 0;
    nMeterSegmentWidth = 6;

    AverageMeters = new MeterBarAverage*[nInputChannels];
    PeakMeters = new MeterBarPeak*[nInputChannels];

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;

        int nPositionXAverage = (nChannel % 2) ? 0 : nMeterSegmentWidth + 1;
        int nPositionXPeakMeters = (nChannel % 2) ? TraKmeter::TRAKMETER_SEGMENT_WIDTH - nMeterSegmentWidth : 0;

        AverageMeters[nChannel] = new MeterBarAverage("Level Meter Average #" + String(nChannel), nPositionX + nPositionXAverage, nMeterPositionTop, TraKmeter::TRAKMETER_SEGMENT_WIDTH - nMeterSegmentWidth - 1, nNumberOfBars, nCrestFactor, nSegmentHeight, true, true);
        addAndMakeVisible(AverageMeters[nChannel]);

        PeakMeters[nChannel] = new MeterBarPeak("Level Meter Peak #" + String(nChannel), nPositionX + nPositionXPeakMeters, nMeterPositionTop, nMeterSegmentWidth, nNumberOfBars, nCrestFactor, nSegmentHeight, true, true);
        addAndMakeVisible(PeakMeters[nChannel]);
    }
}


CombinedMeter::~CombinedMeter()
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {

        delete AverageMeters[nChannel];
        AverageMeters[nChannel] = NULL;

        delete PeakMeters[nChannel];
        PeakMeters[nChannel] = NULL;
    }

    delete [] AverageMeters;
    AverageMeters = NULL;

    delete [] PeakMeters;
    PeakMeters = NULL;

    deleteAllChildren();
}


int CombinedMeter::getPreferredHeight()
{
    return nHeight;
}


void CombinedMeter::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);
}


void CombinedMeter::paint(Graphics& g)
{
    int x = 0;
    int y = 0;
    int width = TraKmeter::TRAKMETER_LABEL_WIDTH - 14;
    int height = 13;

    y = nMeterPositionTop;
    String strMarker = "OVR";

    g.setFont(12.0f);
    g.setColour(Colours::red);
    drawMarkers(g, strMarker, x + 1, y, width, height);

    y -= roundf(nSegmentHeight / 2.0f);

    int nLevel = -4 + nCrestFactor;

    for (int n = nNumberOfBars; n > 2; n -= 2)
    {
        if (n > 7)
        {
            nLevel -= 2;
        }
        else if (n == 7)
        {
            nLevel -= 14;
        }
        else
        {
            nLevel -= 20;
        }

        if (nLevel > 0)
        {
            strMarker = "+" + String(nLevel);
        }
        else
        {
            strMarker = String(nLevel);
        }

        y += 2 * nSegmentHeight;

        if (nLevel == nCrestFactor - 10)
        {
            g.setColour(Colours::white);
        }
        else if (nLevel == nCrestFactor - 20)
        {
            g.setColour(Colours::white);
        }
        else if (nLevel == nCrestFactor - 40)
        {
            g.setColour(Colours::white);
        }
        else
        {
            g.setColour(Colours::white.darker(0.4f));
        }

        drawMarkers(g, strMarker, x + 1, y, width, height);
    }

    g.setColour(Colours::black.brighter(0.15f));

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        int nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;
        nPositionX += (nChannel % 2) ? TraKmeter::TRAKMETER_SEGMENT_WIDTH - nMeterSegmentWidth - 1 : nMeterSegmentWidth;

        g.fillRect(nPositionX, nMeterPositionTop, 1, nHeight - 1);
    }
}


void CombinedMeter::resized()
{
}


void CombinedMeter::setLevels(MeterBallistics* pMeterBallistics)
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        AverageMeters[nChannel]->setLevels(pMeterBallistics->getAverageMeterLevel(nChannel), pMeterBallistics->getAverageMeterPeakLevel(nChannel));
        PeakMeters[nChannel]->setLevels(pMeterBallistics->getPeakMeterLevel(nChannel), pMeterBallistics->getPeakMeterPeakLevel(nChannel), pMeterBallistics->getMaximumPeakLevel(nChannel));
    }
}


void CombinedMeter::drawMarkers(Graphics& g, String& strMarker, int x, int y, int width, int height)
{
    g.saveState();

    int meter_width = nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 4;
    int x_2 = x + TraKmeter::TRAKMETER_LABEL_WIDTH + meter_width + 4;

    g.drawFittedText(strMarker, x, y - 1, width, height, Justification::centred, 1, 1.0f);
    g.drawFittedText(strMarker, x_2, y - 1, width, height, Justification::centred, 1, 1.0f);

    g.setColour(Colours::grey);

    int nMarkerY = y + 5;
    int nStart = x + width + 2;
    int nEnd = x + x_2 - 3;

    for (int nMarkerX = nStart; nMarkerX < nEnd; nMarkerX++)
    {
        g.setPixel(nMarkerX, nMarkerY);
    }

    g.restoreState();
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
