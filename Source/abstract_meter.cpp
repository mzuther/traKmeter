/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2014 Martin Zuther (http://www.mzuther.de/)

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

#include "abstract_meter.h"

AbstractMeter::AbstractMeter(const String& componentName, int posX, int posY, int width, int CrestFactor, int nNumChannels, int segment_height)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;
    nCrestFactor = CrestFactor;

    nSegmentHeight = segment_height;
    nMeterPositionTop = 5;
    nSegmentSpace = 10;
    nMeterHeight = 3 * nSegmentHeight + 2 * nSegmentSpace;

    nPosX = posX;
    nPosY = posY;
    nWidth = width;
    nHeight = 2 * nMeterPositionTop + nMeterHeight;

    int nPositionX = 0;
    nPeakMeterSegmentWidth = 6;

    OverloadMeters = new MeterSegmentOverload*[nInputChannels];
    PeakMeters = new MeterSegmentMulti*[nInputChannels];
    AverageMeters = new MeterSegmentMulti*[nInputChannels];

    int nOverloadThreshold = -90;
    nOverloadThreshold += nCrestFactor * 10;

    // overload: register all hot signals, even up to +100 dB FS!
    float fOverloadRange = ((nCrestFactor * 10) - nOverloadThreshold) * 0.1f + 100.0f;

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;
        nPositionX += (nChannel % 2) ? -2 : 2;

        OverloadMeters[nChannel] = new MeterSegmentOverload("OverloadMeters (" + componentName + ")", nOverloadThreshold * 0.1f, fOverloadRange, nCrestFactor, true, true, 0);
        addAndMakeVisible(OverloadMeters[nChannel]);

        PeakMeters[nChannel] = new MeterSegmentMulti("PeakMeters (" + componentName + ")", nCrestFactor, false, true);
        addAndMakeVisible(PeakMeters[nChannel]);

        AverageMeters[nChannel] = new MeterSegmentMulti("AverageMeters (" + componentName + ")", nCrestFactor, true, true);
        addAndMakeVisible(AverageMeters[nChannel]);
    }
}


AbstractMeter::~AbstractMeter()
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        delete OverloadMeters[nChannel];
        OverloadMeters[nChannel] = NULL;

        delete PeakMeters[nChannel];
        PeakMeters[nChannel] = NULL;

        delete AverageMeters[nChannel];
        AverageMeters[nChannel] = NULL;
    }

    delete [] OverloadMeters;
    OverloadMeters = NULL;

    delete [] PeakMeters;
    PeakMeters = NULL;

    delete [] AverageMeters;
    AverageMeters = NULL;

    deleteAllChildren();
}


int AbstractMeter::getPreferredHeight()
{
    return nHeight;
}


void AbstractMeter::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        int nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;
        nPositionX += (nChannel % 2) ? -2 : 2;
        int nPositionY = nMeterPositionTop;
        OverloadMeters[nChannel]->setBounds(nPositionX, nPositionY, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nSegmentHeight);

        nPositionY += nSegmentHeight + nSegmentSpace;
        PeakMeters[nChannel]->setBounds(nPositionX, nPositionY, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nSegmentHeight);

        nPositionY += nSegmentHeight + nSegmentSpace;
        AverageMeters[nChannel]->setBounds(nPositionX, nPositionY, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nSegmentHeight);
    }
}


void AbstractMeter::paint(Graphics& g)
{
    int x = 0;
    int y = 0;
    int width = TraKmeter::TRAKMETER_LABEL_WIDTH - 12;
    int height = 13;

    y = nMeterPositionTop;
    String strMarker = "OVR";

    g.setFont(12.0f);
    g.setColour(Colour(0.00f, 1.0f, 1.0f, 1.0f));
    drawMarkers(g, strMarker, x + 1, y, width, height, Colour(0.00f, 1.0f, 1.0f, 1.0f));

    y += nSegmentHeight + nSegmentSpace;
    strMarker = "Peak";

    g.setColour(Colours::white);
    drawMarkers(g, strMarker, x + 1, y, width, height, Colours::white.darker(0.5f));

    y += nSegmentHeight + nSegmentSpace;
    strMarker = "RMS";

    g.setColour(Colours::yellow);
    drawMarkers(g, strMarker, x + 1, y, width, height, Colours::white.darker(0.5f));

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        int nPositionX = TraKmeter::TRAKMETER_LABEL_WIDTH + nChannel * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 3;
        nPositionX += (nChannel % 2) ? -2 : 2;

        int nPositionY = nMeterPositionTop - 2;
        g.setColour(Colours::black.brighter(0.20f));
        g.fillRect(nPositionX - 1, nPositionY, TraKmeter::TRAKMETER_SEGMENT_WIDTH + 2, nSegmentHeight + 4);

        nPositionY += nSegmentHeight + nSegmentSpace;
        g.fillRect(nPositionX - 1, nPositionY, TraKmeter::TRAKMETER_SEGMENT_WIDTH + 2, nSegmentHeight + 4);

        nPositionY += nSegmentHeight + nSegmentSpace;
        g.fillRect(nPositionX - 1, nPositionY, TraKmeter::TRAKMETER_SEGMENT_WIDTH + 2, nSegmentHeight + 4);

        g.setColour(Colours::black);
        g.fillRect(nPositionX, nMeterPositionTop, TraKmeter::TRAKMETER_SEGMENT_WIDTH, nSegmentHeight);
    }
}


void AbstractMeter::resized()
{
}


void AbstractMeter::setLevels(MeterBallistics* pMeterBallistics)
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        OverloadMeters[nChannel]->setLevels(pMeterBallistics->getPeakMeterLevel(nChannel), pMeterBallistics->getPeakMeterPeakLevel(nChannel), pMeterBallistics->getMaximumPeakLevel(nChannel));
        PeakMeters[nChannel]->setLevels(pMeterBallistics->getPeakMeterLevel(nChannel), pMeterBallistics->getPeakMeterPeakLevel(nChannel));
        AverageMeters[nChannel]->setLevels(pMeterBallistics->getAverageMeterLevel(nChannel), pMeterBallistics->getAverageMeterPeakLevel(nChannel));
    }
}


void AbstractMeter::drawMarkers(Graphics& g, String& strMarker, int x, int y, int width, int height, const Colour& colour)
{
    g.saveState();

    int meter_width = nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 4;
    int x_2 = x + TraKmeter::TRAKMETER_LABEL_WIDTH + meter_width + 4;

    g.drawFittedText(strMarker, x, y, width, height, Justification::centred, 1, 1.0f);
    g.drawFittedText(strMarker, x_2, y, width, height, Justification::centred, 1, 1.0f);

    g.setColour(colour);

    int nMarkerY = y + 6;
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
