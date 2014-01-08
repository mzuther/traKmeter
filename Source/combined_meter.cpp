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

    nNumberOfBars = 27;
    nSegmentHeight = segment_height;
    nPeakLabelHeight = nSegmentHeight + 2;
    nMeterPositionTop = 0;
    nMeterHeight = (nNumberOfBars - 1) * nSegmentHeight + nPeakLabelHeight + 2;

    nPosX = posX;
    nPosY = posY;
    nWidth = width;
    nHeight = nMeterPositionTop + nMeterHeight;

    int nPositionX = 0;
    nPeakMeterSegmentWidth = 6;

    AverageMeters = new MeterBarAverage*[nInputChannels];
    PeakMeters = new MeterBarPeak*[nInputChannels];
    PeakLabels = new PeakLabel*[nInputChannels];

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;

        int nPositionXAverage = (nChannel % 2) ? 0 : nPeakMeterSegmentWidth + 1;
        int nPositionXPeakMeters = (nChannel % 2) ? TraKmeter::TRAKMETER_SEGMENT_WIDTH - nPeakMeterSegmentWidth : 0;

        AverageMeters[nChannel] = new MeterBarAverage("Level Meter Average #" + String(nChannel), nPositionX + nPositionXAverage, nMeterPositionTop + nPeakLabelHeight + 1, TraKmeter::TRAKMETER_SEGMENT_WIDTH - nPeakMeterSegmentWidth - 1, nNumberOfBars - 1, nCrestFactor, nSegmentHeight, true, true);
        addAndMakeVisible(AverageMeters[nChannel]);

        PeakMeters[nChannel] = new MeterBarPeak("Level Meter Peak #" + String(nChannel), nPositionX + nPositionXPeakMeters, nMeterPositionTop + nPeakLabelHeight + 1, nPeakMeterSegmentWidth, nNumberOfBars - 1, nCrestFactor, nSegmentHeight, true, true);
        addAndMakeVisible(PeakMeters[nChannel]);

        PeakLabels[nChannel] = new PeakLabel("PeakLabel (" + componentName + ")", nCrestFactor);
        addAndMakeVisible(PeakLabels[nChannel]);
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

        delete PeakLabels[nChannel];
        PeakLabels[nChannel] = NULL;
    }

    delete [] AverageMeters;
    AverageMeters = NULL;

    delete [] PeakMeters;
    PeakMeters = NULL;

    delete [] PeakLabels;
    PeakLabels = NULL;

    deleteAllChildren();
}


int CombinedMeter::getPreferredHeight()
{
    return nHeight;
}


void CombinedMeter::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        int nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;

        PeakLabels[nChannel]->setBounds(nPositionX, nMeterPositionTop, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nPeakLabelHeight);
    }
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
    g.setColour(Colour(0.00f, 1.0f, 1.0f, 1.0f));
    drawMarkers(g, strMarker, x + 1, y, width, height, Colour(0.00f, 1.0f, 1.0f, 1.0f));

    y -= roundf(nSegmentHeight / 2.0f);
    y += nPeakLabelHeight - nSegmentHeight + 1;

    int nTrueLevel = -8;

    for (int n = nNumberOfBars; n > 2; n -= 2)
    {
        if (nTrueLevel > -26)
        {
            nTrueLevel -= 2;
        }
        else if (nTrueLevel > -40)
        {
            nTrueLevel -= 14;
        }
        else
        {
            nTrueLevel -= 20;
        }

        int nLevel = nTrueLevel + nCrestFactor;

        if (nLevel > 0)
        {
            strMarker = "+" + String(nLevel);
        }
        else
        {
            strMarker = String(nLevel);
        }

        y += 2 * nSegmentHeight;

        if (nTrueLevel == -10)
        {
            g.setColour(Colours::white);
            drawMarkers(g, strMarker, x + 1, y, width, height, Colours::white.darker(0.5f));
        }
        else if (nTrueLevel == -20)
        {
            g.setColour(Colours::white);
            drawMarkers(g, strMarker, x + 1, y, width, height, Colours::white.darker(0.5f));
        }
        else if (nTrueLevel == -40)
        {
            g.setColour(Colour(0.58f, 1.0f, 1.0f, 1.0f));
            drawMarkers(g, strMarker, x + 1, y, width, height, Colour(0.58f, 1.0f, 1.0f, 1.0f));
        }
        else
        {
            g.setColour(Colours::grey.brighter(0.1f));
            drawMarkers(g, strMarker, x + 1, y, width, height, Colours::grey.darker(0.1f));
        }
    }

    g.setColour(Colours::black.brighter(0.15f));

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        int nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;

        g.fillRect(nPositionX, nMeterPositionTop + nPeakLabelHeight, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nHeight - nPeakLabelHeight);
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
        PeakMeters[nChannel]->setLevels(pMeterBallistics->getPeakMeterLevel(nChannel), pMeterBallistics->getPeakMeterPeakLevel(nChannel));
        PeakLabels[nChannel]->updateLevel(pMeterBallistics->getMaximumPeakLevel(nChannel));
    }
}


void CombinedMeter::drawMarkers(Graphics& g, String& strMarker, int x, int y, int width, int height, const Colour& colour)
{
    g.saveState();

    int meter_width = nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 4;
    int x_2 = x + TraKmeter::TRAKMETER_LABEL_WIDTH + meter_width + 4;

    g.drawFittedText(strMarker, x, y - 1, width, height, Justification::centred, 1, 1.0f);
    g.drawFittedText(strMarker, x_2, y - 1, width, height, Justification::centred, 1, 1.0f);

    g.setColour(colour);

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
