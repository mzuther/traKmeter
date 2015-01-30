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

#include "trakmeter.h"

TraKmeter::TraKmeter(const String &componentName, int posX, int posY, int nCrestFactor, int nNumChannels, int nSegmentHeight, int meter_type)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;
    nMeterType = meter_type;

    nPosX = posX;
    nPosY = posY;
    nWidth = 2 * TRAKMETER_LABEL_WIDTH + nInputChannels * (TRAKMETER_SEGMENT_WIDTH + 6) - 3;
    nHeight = 0;

    combined_meter = nullptr;

    peak_meter = nullptr;
    average_meter = nullptr;
    signal_meter = nullptr;

    if (nMeterType == TraKmeterPluginParameters::selSeparateMeters)
    {
        peak_meter = new PeakMeter("Peak Meter", 4, 4, nWidth - 8, nCrestFactor, nInputChannels, nSegmentHeight);
        addAndMakeVisible(peak_meter);

        int nHeightPeakMeter = peak_meter->getPreferredHeight();
        int nHeightSeparator = 29;

        signal_meter = new SignalMeter("Signal Meter", 4, nHeightPeakMeter + (nHeightSeparator - 14) / 2 + 4, nWidth - 8, nCrestFactor, nInputChannels);
        addAndMakeVisible(signal_meter);

        average_meter = new AverageMeter("Average Meter", 4, nHeightPeakMeter + nHeightSeparator + 4, nWidth - 8, nCrestFactor, nInputChannels, nSegmentHeight);
        addAndMakeVisible(average_meter);

        int nHeightAverageMeter = average_meter->getPreferredHeight();

        nHeight = nHeightPeakMeter + nHeightAverageMeter + nHeightSeparator + 8;
    }
    else
    {
        combined_meter = new CombinedMeter("Combined Meter", 4, 4, nWidth - 8, nCrestFactor, nInputChannels, nSegmentHeight - 1);
        addAndMakeVisible(combined_meter);

        nHeight = combined_meter->getPreferredHeight() + 8;
    }
}


TraKmeter::~TraKmeter()
{
    delete combined_meter;
    combined_meter = nullptr;

    delete peak_meter;
    peak_meter = nullptr;

    delete average_meter;
    average_meter = nullptr;

    delete signal_meter;
    signal_meter = nullptr;

    deleteAllChildren();
}


void TraKmeter::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);
}


void TraKmeter::paint(Graphics &g)
{
    int x = 4;
    int y = 0;

    g.setColour(Colours::grey.withAlpha(0.1f));
    g.fillRect(x - 4, y, nWidth, nHeight + 1);

    g.setColour(Colours::darkgrey);
    g.drawRect(x - 4, y, nWidth - 1, nHeight - 1);

    g.setColour(Colours::darkgrey.darker(0.7f));
    g.drawRect(x - 3, y + 1, nWidth - 1, nHeight - 1);

    g.setColour(Colours::darkgrey.darker(0.4f));
    g.drawRect(x - 3, y + 1, nWidth - 2, nHeight - 2);
}


void TraKmeter::resized()
{
}


void TraKmeter::setLevels(MeterBallistics *pMeterBallistics)
{
    if (nMeterType == TraKmeterPluginParameters::selSeparateMeters)
    {
        peak_meter->setLevels(pMeterBallistics);
        average_meter->setLevels(pMeterBallistics);
        signal_meter->setLevels(pMeterBallistics);
    }
    else
    {
        combined_meter->setLevels(pMeterBallistics);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
