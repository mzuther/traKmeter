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

#include "average_meter.h"

AverageMeter::AverageMeter(const String &componentName, int posX, int posY, int width, int nCrestFactor, int nNumChannels, int nSegmentHeight)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;

    int nNumberOfBars = 8;
    int nMeterPositionBottom = 22;
    int nMeterHeight = nNumberOfBars * nSegmentHeight + 2;

    nPosX = posX;
    nPosY = posY;
    nWidth = width;
    nHeight = nMeterHeight + nMeterPositionBottom;

    int nPositionX = 0;
    LevelMeters = new MeterBarAverage*[nInputChannels];

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;
        nPositionX += (nChannel % 2) ? -2 : 2;

        LevelMeters[nChannel] = new MeterBarAverage("Level Meter Average #" + String(nChannel), nPositionX, 1, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nNumberOfBars, nCrestFactor, nSegmentHeight, true, false);
        addAndMakeVisible(LevelMeters[nChannel]);
    }
}


AverageMeter::~AverageMeter()
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        delete LevelMeters[nChannel];
        LevelMeters[nChannel] = nullptr;
    }

    delete [] LevelMeters;
    LevelMeters = nullptr;

    deleteAllChildren();
}


int AverageMeter::getPreferredHeight()
{
    return nHeight;
}


void AverageMeter::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);
}


void AverageMeter::resized()
{
}


void AverageMeter::setLevels(MeterBallistics *pMeterBallistics)
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        LevelMeters[nChannel]->setLevels(pMeterBallistics->getAverageMeterLevel(nChannel), pMeterBallistics->getAverageMeterPeakLevel(nChannel));
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
