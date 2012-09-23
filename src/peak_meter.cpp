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

#include "peak_meter.h"

PeakMeter::PeakMeter(const String& componentName, int posX, int posY, int width, int nCrestFactor, int nNumChannels, int segment_height)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;
    nMeterCrestFactor = nCrestFactor;

    nNumberOfBars = 10;
    nMeterPositionTop = 19;
    nSegmentHeight = segment_height;

    nPosX = posX;
    nPosY = posY;
    nWidth = width;
    nHeight = nMeterPositionTop + nNumberOfBars * nSegmentHeight + 1;

    int nPositionX = 0;
    LevelMeters = new MeterBarPeak*[nInputChannels];

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 4) - 2;

        LevelMeters[nChannel] = new MeterBarPeak("Level Meter Peak #" + String(nChannel), nPositionX, nMeterPositionTop, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nNumberOfBars, nMeterCrestFactor, nSegmentHeight, true);
        addAndMakeVisible(LevelMeters[nChannel]);
    }
}


PeakMeter::~PeakMeter()
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        delete LevelMeters[nChannel];
        LevelMeters[nChannel] = NULL;
    }

    delete [] LevelMeters;
    LevelMeters = NULL;

    deleteAllChildren();
}


int PeakMeter::getPreferredHeight()
{
    return nHeight;
}


void PeakMeter::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);
}


void PeakMeter::paint(Graphics& g)
{
    int x = 0;
    int y = 0;
    int width = 20;
    int height = 11;

    g.setColour(Colours::grey.brighter(0.6f));
    g.fillRect(x, y, TraKmeter::TRAKMETER_LABEL_WIDTH - 9, 12);

    g.setColour(Colours::grey);
    g.drawRect(x, y, TraKmeter::TRAKMETER_LABEL_WIDTH - 9, 12);

    int x_2 = TraKmeter::TRAKMETER_LABEL_WIDTH + nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 4) + 1;
    g.setColour(Colours::grey.brighter(0.6f));
    g.fillRect(x_2 + x, y, TraKmeter::TRAKMETER_LABEL_WIDTH - 9, 12);

    g.setColour(Colours::grey);
    g.drawRect(x_2 + x, y, TraKmeter::TRAKMETER_LABEL_WIDTH - 9, 12);

    String strMarker = "dB";
    g.setColour(Colours::black);
    g.setFont(12.0f);
    drawMarkers(g, strMarker, x + 1, y + 1, width, height);

    g.setColour(Colours::grey.brighter(0.6f));
    g.fillRect((nWidth - 32) / 2, y, 32, 12);

    g.setColour(Colours::grey);
    g.drawRect((nWidth - 32) / 2, y, 32, 12);

    g.setColour(Colours::black);
    g.drawFittedText("Peak", (nWidth - 32) / 2, y, 32, 12, Justification::centred, 1, 1.0f);

    y = nMeterPositionTop + nSegmentHeight / 2 - 1;
    strMarker = "OVR";

    g.setFont(11.0f);
    g.setColour(Colours::red);
    drawMarkers(g, strMarker, x + 1, y, width, height);

    for (int n = nNumberOfBars; n > 4; n -= 2)
    {
        int nLevel = n;

        if (nLevel > 0)
        {
            strMarker = "+" + String(nLevel);
        }
        else
        {
            strMarker = String(nLevel);
        }

        y += 2 * nSegmentHeight;

        if (nLevel == 10)
        {
            g.setColour(Colours::white);
        }
        else
        {
            g.setColour(Colours::white.darker(0.4f));
        }

        drawMarkers(g, strMarker, x + 1, y, width, height);
    }

    strMarker = "SIG";

    g.setColour(Colours::yellow);
    drawMarkers(g, strMarker, x + 1, y + 2 * nSegmentHeight, width, height);
}


void PeakMeter::resized()
{
}


void PeakMeter::setLevels(MeterBallistics* pMeterBallistics)
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        LevelMeters[nChannel]->setLevels(pMeterBallistics->getPeakMeterLevel(nChannel), pMeterBallistics->getPeakMeterPeakLevel(nChannel));
    }
}


void PeakMeter::drawMarkers(Graphics& g, String& strMarker, int x, int y, int width, int height)
{
    g.saveState();

    int meter_width = nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 4) - 2;

    g.drawFittedText(strMarker, x, y, width, height, Justification::centred, 1, 1.0f);
    g.drawFittedText(strMarker, x + TraKmeter::TRAKMETER_LABEL_WIDTH + meter_width + 4, y, width, height, Justification::centred, 1, 1.0f);

    g.setColour(Colours::grey);

    int nMarkerY = y + 5;
    int nStart = x + width + 2;
    int nEnd = x + TraKmeter::TRAKMETER_LABEL_WIDTH + meter_width + 2;

    for (int nMarkerX = nStart; nMarkerX < nEnd; nMarkerX++)
    {
        g.setPixel(nMarkerX, nMarkerY);
    }

    g.restoreState();
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
