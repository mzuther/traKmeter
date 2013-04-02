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

#include "signal_meter.h"

SignalMeter::SignalMeter(const String& componentName, int posX, int posY, int width, int CrestFactor, int nNumChannels)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;
    nCrestFactor = CrestFactor;

    nSegmentHeight = 12;
    nMeterHeight = nSegmentHeight + 1;

    nPosX = posX;
    nPosY = posY;
    nWidth = width;
    nHeight = nMeterHeight;

    int nPositionX = 0;
    PeakMeterSignals = new MeterSignalLed*[nInputChannels];

    // signals are detected at -60 dB and above
    int nThreshold = (-60 + nCrestFactor) * 10;
    float fRange = 40.0f;
    String strLabel;

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        if (nInputChannels == 2)
        {
            if (nChannel == 0)
            {
                strLabel = "L";
            }
            else
            {
                strLabel = "R";
            }
        }
        else
        {
            strLabel = String(nChannel + 1);
        }

        nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;

        PeakMeterSignals[nChannel] = new MeterSignalLed("Peak Meter Signal #" + String(nChannel), strLabel, nThreshold * 0.1f, fRange);

        PeakMeterSignals[nChannel]->setBounds(nPositionX, 0, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nSegmentHeight + 1);
        addAndMakeVisible(PeakMeterSignals[nChannel]);
    }
}


SignalMeter::~SignalMeter()
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        delete PeakMeterSignals[nChannel];
        PeakMeterSignals[nChannel] = NULL;
    }

    delete [] PeakMeterSignals;
    PeakMeterSignals = NULL;

    deleteAllChildren();
}


int SignalMeter::getPreferredHeight()
{
    return nHeight;
}


void SignalMeter::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);
}


void SignalMeter::paint(Graphics& g)
{
    int x = 0;
    int y = -1;
    int width = TraKmeter::TRAKMETER_LABEL_WIDTH - 14;
    int height = 13;

    String strMarker = "SIG";

    g.setColour(Colours::white);
    g.setFont(12.0f);
    drawMarkers(g, strMarker, x + 1, y + 2, width, height);
}


void SignalMeter::resized()
{
}


void SignalMeter::setLevels(MeterBallistics* pMeterBallistics)
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        PeakMeterSignals[nChannel]->setLevel(pMeterBallistics->getPeakMeterSignal(nChannel));
    }
}


void SignalMeter::drawMarkers(Graphics& g, String& strMarker, int x, int y, int width, int height)
{
    int meter_width = nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 6;
    int x_2 = TraKmeter::TRAKMETER_LABEL_WIDTH + nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 2;

    g.saveState();

    g.setColour(Colours::grey.brighter(0.6f));
    g.fillRect(x, y - 1, width + 3, height);

    g.setColour(Colours::black);
    g.drawRect(x, y - 1, width + 3, height);

    g.setColour(Colours::grey.brighter(0.6f));
    g.fillRect(x_2 + x, y - 1, width + 3, height);

    g.setColour(Colours::black);
    g.drawRect(x_2 + x, y - 1, width + 3, height);

    x_2 = x + TraKmeter::TRAKMETER_LABEL_WIDTH + meter_width + 5;
    g.setColour(Colours::black);

    g.drawFittedText(strMarker, x + 1, y - 1, width, height, Justification::centred, 1, 1.0f);
    g.drawFittedText(strMarker, x_2 + 1, y - 1, width, height, Justification::centred, 1, 1.0f);

    g.setColour(Colours::grey);

    int nMarkerY = y + 5;
    int nStart = x + width + 3;
    int nEnd = x + x_2 - 2;

    for (int nMarkerX = nStart; nMarkerX < nEnd; nMarkerX++)
    {
        g.setPixel(nMarkerX, nMarkerY);
    }

    g.restoreState();
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
