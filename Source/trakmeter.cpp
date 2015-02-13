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

    int nWidth = 2 * TRAKMETER_LABEL_WIDTH + nInputChannels * (TRAKMETER_SEGMENT_WIDTH + 6) - 3;

    combined_meter = nullptr;

    peak_meter = nullptr;
    average_meter = nullptr;
    signal_meter = nullptr;

    if (nMeterType == TraKmeterPluginParameters::selSplitMeters)
    {
        peak_meter = new PeakMeter("Peak Meter", 4, 4, nWidth - 8, nCrestFactor, nInputChannels, nSegmentHeight);
        addAndMakeVisible(peak_meter);

        int nHeightPeakMeter = peak_meter->getPreferredHeight();
        int nHeightSeparator = 29;

        signal_meter = new SignalMeter("Signal Meter", 4, nHeightPeakMeter + (nHeightSeparator - 14) / 2 + 4, nWidth - 8, nCrestFactor, nInputChannels);
        addAndMakeVisible(signal_meter);

        average_meter = new AverageMeter("Average Meter", 4, nHeightPeakMeter + nHeightSeparator + 4, nWidth - 8, nCrestFactor, nInputChannels, nSegmentHeight);
        addAndMakeVisible(average_meter);
    }
    else
    {
        combined_meter = new CombinedMeter("Combined Meter", 4, 4, nWidth - 8, nCrestFactor, nInputChannels, nSegmentHeight - 1);
        addAndMakeVisible(combined_meter);
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


void TraKmeter::applySkin(Skin *pSkin)
{
    // if (nInputChannels == 2)
    // {
    //     pSkin->placeComponent(peak_meter[0], "meter_kmeter_left");
    //     pSkin->placeAndSkinStateLabel(OverflowMeters[0], "label_over_left");
    //     pSkin->placeAndSkinStateLabel(MaximumPeakLabels[0], "label_peak_left");

    //     pSkin->placeComponent(LevelMeters[1], "meter_kmeter_right");
    //     pSkin->placeAndSkinStateLabel(OverflowMeters[1], "label_over_right");
    //     pSkin->placeAndSkinStateLabel(MaximumPeakLabels[1], "label_peak_right");
    // }
    // else if (nInputChannels == 6)
    // {
    //     pSkin->placeComponent(LevelMeters[0], "meter_kmeter_left");
    //     pSkin->placeAndSkinStateLabel(OverflowMeters[0], "label_over_left");
    //     pSkin->placeAndSkinStateLabel(MaximumPeakLabels[0], "label_peak_left");

    //     pSkin->placeComponent(LevelMeters[1], "meter_kmeter_right");
    //     pSkin->placeAndSkinStateLabel(OverflowMeters[1], "label_over_right");
    //     pSkin->placeAndSkinStateLabel(MaximumPeakLabels[1], "label_peak_right");

    //     pSkin->placeComponent(LevelMeters[2], "meter_kmeter_center");
    //     pSkin->placeAndSkinStateLabel(OverflowMeters[2], "label_over_center");
    //     pSkin->placeAndSkinStateLabel(MaximumPeakLabels[2], "label_peak_center");

    //     pSkin->placeComponent(LevelMeters[3], "meter_kmeter_lfe");
    //     pSkin->placeAndSkinStateLabel(OverflowMeters[3], "label_over_lfe");
    //     pSkin->placeAndSkinStateLabel(MaximumPeakLabels[3], "label_peak_lfe");

    //     pSkin->placeComponent(LevelMeters[4], "meter_kmeter_ls");
    //     pSkin->placeAndSkinStateLabel(OverflowMeters[4], "label_over_ls");
    //     pSkin->placeAndSkinStateLabel(MaximumPeakLabels[4], "label_peak_ls");

    //     pSkin->placeComponent(LevelMeters[5], "meter_kmeter_rs");
    //     pSkin->placeAndSkinStateLabel(OverflowMeters[5], "label_over_rs");
    //     pSkin->placeAndSkinStateLabel(MaximumPeakLabels[5], "label_peak_rs");
    // }
    // else
    // {
    //     DBG("[K-Meter] channel configuration (" + String(nInputChannels) + " channels) not supported");
    // }

    // Component *parent = getParentComponent();

    // if (parent != nullptr)
    // {
    //     setBounds(0, 0, parent->getWidth(), parent->getHeight());
    // }

    pSkin->placeComponent(this, "meter_graduation");
}


void TraKmeter::resized()
{
}


void TraKmeter::setLevels(MeterBallistics *pMeterBallistics)
{
    if (nMeterType == TraKmeterPluginParameters::selSplitMeters)
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
