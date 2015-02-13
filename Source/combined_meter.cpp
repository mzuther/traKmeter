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

#include "combined_meter.h"

CombinedMeter::CombinedMeter(const String &componentName, int posX, int posY, int width, int nCrestFactor, int nNumChannels, int nSegmentHeight)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;

    int nNumberOfBars = 27;
    nPeakLabelHeight = nSegmentHeight + 2;
    nMeterPositionTop = 4;
    int nMeterHeight = (nNumberOfBars - 1) * nSegmentHeight + nPeakLabelHeight + 3;

    nPosX = posX;
    nPosY = posY;
    nWidth = width;
    nHeight = 2 * nMeterPositionTop + nMeterHeight;

    int nPositionX = 0;
    int nPeakMeterSegmentWidth = 6;

    AverageMeters = new MeterBarAverage*[nInputChannels];
    PeakMeters = new MeterBarPeak*[nInputChannels];
    MeterSegmentOverloads = new MeterSegmentOverload*[nInputChannels];

    int nThreshold = -90;
    nThreshold += nCrestFactor * 10;

    // register all hot signals, even up to +100 dB FS!
    float fRange = ((nCrestFactor * 10) - nThreshold) * 0.1f + 100.0f;

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;
        nPositionX += (nChannel % 2) ? -2 : 2;

        int nPositionXAverage = (nChannel % 2) ? 0 : nPeakMeterSegmentWidth + 1;
        int nPositionXPeakMeters = (nChannel % 2) ? TraKmeter::TRAKMETER_SEGMENT_WIDTH - nPeakMeterSegmentWidth : 0;

        AverageMeters[nChannel] = new MeterBarAverage("Level Meter Average #" + String(nChannel), nPositionX + nPositionXAverage, nMeterPositionTop + nPeakLabelHeight + 2, TraKmeter::TRAKMETER_SEGMENT_WIDTH - nPeakMeterSegmentWidth - 1, nNumberOfBars - 1, nCrestFactor, nSegmentHeight, true, true);
        addAndMakeVisible(AverageMeters[nChannel]);

        PeakMeters[nChannel] = new MeterBarPeak("Level Meter Peak #" + String(nChannel), nPositionX + nPositionXPeakMeters, nMeterPositionTop + nPeakLabelHeight + 2, nPeakMeterSegmentWidth, nNumberOfBars - 1, nCrestFactor, nSegmentHeight, true, true);
        addAndMakeVisible(PeakMeters[nChannel]);

        MeterSegmentOverloads[nChannel] = new MeterSegmentOverload("MeterSegmentOverload (" + componentName + ")", nThreshold * 0.1f, fRange, nCrestFactor, false, 0);
        addAndMakeVisible(MeterSegmentOverloads[nChannel]);
    }
}


CombinedMeter::~CombinedMeter()
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {

        delete AverageMeters[nChannel];
        AverageMeters[nChannel] = nullptr;

        delete PeakMeters[nChannel];
        PeakMeters[nChannel] = nullptr;

        delete MeterSegmentOverloads[nChannel];
        MeterSegmentOverloads[nChannel] = nullptr;
    }

    delete [] AverageMeters;
    AverageMeters = nullptr;

    delete [] PeakMeters;
    PeakMeters = nullptr;

    delete [] MeterSegmentOverloads;
    MeterSegmentOverloads = nullptr;

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
        nPositionX += (nChannel % 2) ? -2 : 2;

        MeterSegmentOverloads[nChannel]->setBounds(nPositionX, nMeterPositionTop, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nPeakLabelHeight);
    }
}


void CombinedMeter::resized()
{
}


void CombinedMeter::setLevels(MeterBallistics *pMeterBallistics)
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        AverageMeters[nChannel]->setLevels(pMeterBallistics->getAverageMeterLevel(nChannel), pMeterBallistics->getAverageMeterPeakLevel(nChannel));
        PeakMeters[nChannel]->setLevels(pMeterBallistics->getPeakMeterLevel(nChannel), pMeterBallistics->getPeakMeterPeakLevel(nChannel));
        MeterSegmentOverloads[nChannel]->setLevels(pMeterBallistics->getPeakMeterLevel(nChannel), pMeterBallistics->getPeakMeterPeakLevel(nChannel), pMeterBallistics->getMaximumPeakLevel(nChannel));
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
