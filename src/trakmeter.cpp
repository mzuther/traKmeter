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

#include "trakmeter.h"

TraKmeter::TraKmeter(const String& componentName, int posX, int posY, int nCrestFactor, int nNumChannels)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;

    nPosX = posX;
    nPosY = posY;
    nWidth = 2 * TRAKMETER_LABEL_WIDTH + nInputChannels * (TRAKMETER_SEGMENT_WIDTH + 6) - 3;
    nHeight = 0;

    peak_meter = new PeakMeter("Peak Meter", 4, 4, nWidth - 8, nCrestFactor, nInputChannels, TRAKMETER_SEGMENT_HEIGHT);
    addAndMakeVisible(peak_meter);

    nHeightPeakMeter = peak_meter->getPreferredHeight();
    nHeightSeparator = 31;

    average_meter = new AverageMeter("Average Meter", 4, nHeightPeakMeter + nHeightSeparator + 4, nWidth - 8, nCrestFactor, nInputChannels, TRAKMETER_SEGMENT_HEIGHT);
    addAndMakeVisible(average_meter);

    nHeightAverageMeter = average_meter->getPreferredHeight();
    nHeight = nHeightPeakMeter + nHeightAverageMeter + nHeightSeparator + 8;
}


TraKmeter::~TraKmeter()
{
    delete peak_meter;
    peak_meter = NULL;

    delete average_meter;
    average_meter = NULL;

    deleteAllChildren();
}


void TraKmeter::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);
}


void TraKmeter::paint(Graphics& g)
{
    int x = 4;
    int y = 0;

    g.setColour(Colours::grey.withAlpha(0.1f));
    g.fillRect(x - 4, 0, nWidth, nHeight + 1);

    g.setColour(Colours::darkgrey);
    g.drawRect(x - 4, 0, nWidth - 1, nHeight - 1);

    g.setColour(Colours::darkgrey.darker(0.7f));
    g.drawRect(x - 3, 0 + 1, nWidth - 1, nHeight - 1);

    g.setColour(Colours::darkgrey.darker(0.4f));
    g.drawRect(x - 3, 0 + 1, nWidth - 2, nHeight - 2);

    y = nHeightPeakMeter + (nHeightSeparator - 12) / 2 + 4;
    int width = TraKmeter::TRAKMETER_LABEL_WIDTH - 14;
    int height = 13;

    g.setColour(Colours::grey.brighter(0.6f));
    g.fillRect(x, y, width + 3, height);

    g.setColour(Colours::grey);
    g.drawRect(x, y, width + 3, height);

    int x_2 = TraKmeter::TRAKMETER_LABEL_WIDTH + nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 1;
    g.setColour(Colours::grey.brighter(0.6f));
    g.fillRect(x_2 + x, y, width + 3, height);

    g.setColour(Colours::grey);
    g.drawRect(x_2 + x, y, width + 3, height);

    String strMarker = "CH";
    g.setColour(Colours::black);
    g.setFont(12.0f);
    drawMarkers(g, strMarker, x - 1, y + 1, width + 2, height);

    x_2 = TraKmeter::TRAKMETER_LABEL_WIDTH + 1;

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        g.setColour(Colours::grey.brighter(0.6f));
        g.fillRect(x_2, y, TraKmeter::TRAKMETER_SEGMENT_WIDTH, height);

        g.setColour(Colours::grey);
        g.drawRect(x_2, y, TraKmeter::TRAKMETER_SEGMENT_WIDTH, height);

        if (nInputChannels == 2)
        {
            if (nChannel == 0)
            {
                strMarker = "L";
            }
            else
            {
                strMarker = "R";
            }
        }
        else
        {
            strMarker = String(nChannel + 1);
        }

        g.setColour(Colours::black);
        g.drawFittedText(strMarker, x_2, y + 1, TraKmeter::TRAKMETER_SEGMENT_WIDTH, 10, Justification::centred, 1, 1.0f);

        x_2 += TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6;
    }
}


void TraKmeter::resized()
{
}


void TraKmeter::drawMarkers(Graphics& g, String& strMarker, int x, int y, int width, int height)
{
    g.saveState();

    int meter_width = nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) - 6;
    int x_2 = x + TraKmeter::TRAKMETER_LABEL_WIDTH + meter_width + 6;

    g.drawFittedText(strMarker, x + 1, y - 1, width, height, Justification::centred, 1, 1.0f);
    g.drawFittedText(strMarker, x_2 + 1, y - 1, width, height, Justification::centred, 1, 1.0f);

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


void TraKmeter::setLevels(MeterBallistics* pMeterBallistics)
{
    peak_meter->setLevels(pMeterBallistics);
    average_meter->setLevels(pMeterBallistics);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
