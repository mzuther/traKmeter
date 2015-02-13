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

#include "signal_meter.h"

SignalMeter::SignalMeter(const String &componentName, int posX, int posY, int width, int nCrestFactor, int nNumChannels)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;

    int nSegmentHeight = 12;
    int nMeterHeight = nSegmentHeight + 1;

    nPosX = posX;
    nPosY = posY;
    nWidth = width;
    nHeight = nMeterHeight + 2;

    int nPositionX = 0;
    PeakMeterSignals = new MeterSignalLed*[nInputChannels];

    // signals are detected at -60 dB FS and above (40 dB meter range)
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
        nPositionX += (nChannel % 2) ? -2 : 2;

        PeakMeterSignals[nChannel] = new MeterSignalLed("Peak Meter Signal #" + String(nChannel), strLabel, nThreshold * 0.1f, fRange);

        PeakMeterSignals[nChannel]->setBounds(nPositionX, 1, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nSegmentHeight + 1);
        addAndMakeVisible(PeakMeterSignals[nChannel]);
    }
}


SignalMeter::~SignalMeter()
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        delete PeakMeterSignals[nChannel];
        PeakMeterSignals[nChannel] = nullptr;
    }

    delete [] PeakMeterSignals;
    PeakMeterSignals = nullptr;

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


void SignalMeter::resized()
{
}


void SignalMeter::setLevels(MeterBallistics *pMeterBallistics)
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        PeakMeterSignals[nChannel]->setLevel(pMeterBallistics->getPeakMeterSignal(nChannel));
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
