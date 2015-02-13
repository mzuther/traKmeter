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

#include "peak_meter.h"

PeakMeter::PeakMeter(const String &componentName, int posX, int posY, int width, int nCrestFactor, int nNumChannels, int segment_height)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;

    int nNumberOfBars = 9;
    nSegmentHeight = segment_height;
    nMeterPositionTop = 21;
    int nMeterHeight = (nNumberOfBars + 1) * nSegmentHeight + 2;

    nPosX = posX;
    nPosY = posY;
    nWidth = width;
    nHeight = nMeterPositionTop + nMeterHeight;

    int nPositionX = 0;
    LevelMeters = new MeterBarPeak*[nInputChannels];
    MeterSegmentOverloads = new MeterSegmentOverload*[nInputChannels];

    int nThreshold = -90;
    nThreshold += nCrestFactor * 10;

    // register all hot signals, even up to +100 dB FS!
    float fRange = ((nCrestFactor * 10) - nThreshold) * 0.1f + 100.0f;

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;
        nPositionX += (nChannel % 2) ? -2 : 2;

        LevelMeters[nChannel] = new MeterBarPeak("Level Meter Peak #" + String(nChannel), nPositionX, nMeterPositionTop + nSegmentHeight, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nNumberOfBars, nCrestFactor, nSegmentHeight, true, false);
        addAndMakeVisible(LevelMeters[nChannel]);

        MeterSegmentOverloads[nChannel] = new MeterSegmentOverload("MeterSegmentOverload (" + componentName + ")", nThreshold * 0.1f, fRange, nCrestFactor, false, 0);
        addAndMakeVisible(MeterSegmentOverloads[nChannel]);
    }
}


PeakMeter::~PeakMeter()
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        delete LevelMeters[nChannel];
        LevelMeters[nChannel] = nullptr;

        delete MeterSegmentOverloads[nChannel];
        MeterSegmentOverloads[nChannel] = nullptr;
    }

    delete [] LevelMeters;
    LevelMeters = nullptr;

    delete [] MeterSegmentOverloads;
    MeterSegmentOverloads = nullptr;

    deleteAllChildren();
}


int PeakMeter::getPreferredHeight()
{
    return nHeight;
}


void PeakMeter::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        int nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;
        nPositionX += (nChannel % 2) ? -2 : 2;

        MeterSegmentOverloads[nChannel]->setBounds(nPositionX, nMeterPositionTop, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nSegmentHeight + 1);
    }
}


void PeakMeter::resized()
{
}


void PeakMeter::setLevels(MeterBallistics *pMeterBallistics)
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        LevelMeters[nChannel]->setLevels(pMeterBallistics->getPeakMeterLevel(nChannel), pMeterBallistics->getPeakMeterPeakLevel(nChannel));
        MeterSegmentOverloads[nChannel]->setLevels(pMeterBallistics->getPeakMeterLevel(nChannel), pMeterBallistics->getPeakMeterPeakLevel(nChannel), pMeterBallistics->getMaximumPeakLevel(nChannel));
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
