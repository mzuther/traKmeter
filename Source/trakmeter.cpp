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

TraKmeter::TraKmeter(const String &componentName, int posX, int posY, int nCrestFactor, int nNumChannels, int segment_height, int meter_type)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;
    nMeterType = meter_type;
    bShowSplitMeters = (nMeterType == TraKmeterPluginParameters::selSplitMeters);

    AverageMeters = new MeterBarAverage*[nInputChannels];
    PeakMeters = new MeterBarPeak*[nInputChannels];
    MeterSegmentOverloads = new MeterSegmentOverload*[nInputChannels];

    PeakMeterSignals = nullptr;

    int nThreshold = -90;
    nThreshold += nCrestFactor * 10;

    // register all hot signals, even up to +100 dB FS!
    float fRange = ((nCrestFactor * 10) - nThreshold) * 0.1f + 100.0f;

    int nNumberOfBarsPeak;
    int nNumberOfBarsAverage;
    int nSegmentHeight;

    if (bShowSplitMeters)
    {
        nNumberOfBarsPeak = 9;
        nNumberOfBarsAverage = nNumberOfBarsPeak - 1;
        nSegmentHeight = segment_height;
    }
    else
    {
        nNumberOfBarsPeak = 26;
        nNumberOfBarsAverage = nNumberOfBarsPeak;
        nSegmentHeight = segment_height - 1;
    }

    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        PeakMeters[nChannel] = new MeterBarPeak("Level Meter Peak #" + String(nChannel), nNumberOfBarsPeak, nCrestFactor, nSegmentHeight, true, !bShowSplitMeters);
        addAndMakeVisible(PeakMeters[nChannel]);

        AverageMeters[nChannel] = new MeterBarAverage("Level Meter Average #" + String(nChannel), nNumberOfBarsAverage, nCrestFactor, nSegmentHeight, true, !bShowSplitMeters);
        addAndMakeVisible(AverageMeters[nChannel]);

        MeterSegmentOverloads[nChannel] = new MeterSegmentOverload("MeterSegmentOverload (" + componentName + ")", nThreshold * 0.1f, fRange, nCrestFactor, false, 0);
        addAndMakeVisible(MeterSegmentOverloads[nChannel]);
    }

    if (bShowSplitMeters)
    {
        PeakMeterSignals = new MeterSignalLed*[nInputChannels];
        String strLabel;

        // signals are detected at -60 dB FS and above (40 dB meter range)
        int nThreshold = (-60 + nCrestFactor) * 10;
        float fRange = 40.0f;

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

            PeakMeterSignals[nChannel] = new MeterSignalLed("Peak Meter Signal #" + String(nChannel), strLabel, nThreshold * 0.1f, fRange);

            PeakMeterSignals[nChannel]->setBounds(0, 1, TraKmeter::TRAKMETER_SEGMENT_WIDTH, segment_height);
            addAndMakeVisible(PeakMeterSignals[nChannel]);
        }
    }
}


TraKmeter::~TraKmeter()
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        delete AverageMeters[nChannel];
        AverageMeters[nChannel] = nullptr;

        delete PeakMeters[nChannel];
        PeakMeters[nChannel] = nullptr;

        delete MeterSegmentOverloads[nChannel];
        MeterSegmentOverloads[nChannel] = nullptr;

        if (bShowSplitMeters)
        {
            delete PeakMeterSignals[nChannel];
            PeakMeterSignals[nChannel] = nullptr;
        }
    }

    delete [] AverageMeters;
    AverageMeters = nullptr;

    delete [] PeakMeters;
    PeakMeters = nullptr;

    delete [] MeterSegmentOverloads;
    MeterSegmentOverloads = nullptr;

    if (bShowSplitMeters)
    {
        delete [] PeakMeterSignals;
        PeakMeterSignals = nullptr;
    }

    deleteAllChildren();
}


void TraKmeter::applySkin(Skin *pSkin)
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        pSkin->placeComponent(AverageMeters[nChannel], "meter_average_" + String(nChannel + 1));
        pSkin->placeComponent(PeakMeters[nChannel], "meter_peak_" + String(nChannel + 1));

        // pSkin->placeAndSkinStateLabel(MeterSegmentOverloads[nChannel], "label_over_left");
        pSkin->placeComponent(MeterSegmentOverloads[nChannel], "label_over_" + String(nChannel + 1));

        if (bShowSplitMeters)
        {
            // pSkin->placeAndSkinStateLabel(PeakMeterSignals[nChannel], "label_over_left");
            pSkin->placeComponent(PeakMeterSignals[nChannel], "label_signal_" + String(nChannel + 1));
        }
    }

    Component *parent = getParentComponent();

    if (parent != nullptr)
    {
        setBounds(0, 0, parent->getWidth(), parent->getHeight());
    }
}


void TraKmeter::resized()
{
}


void TraKmeter::setLevels(MeterBallistics *pMeterBallistics)
{
    for (int nChannel = 0; nChannel < nInputChannels; nChannel++)
    {
        AverageMeters[nChannel]->setLevels(pMeterBallistics->getAverageMeterLevel(nChannel), pMeterBallistics->getAverageMeterPeakLevel(nChannel));
        PeakMeters[nChannel]->setLevels(pMeterBallistics->getPeakMeterLevel(nChannel), pMeterBallistics->getPeakMeterPeakLevel(nChannel));
        MeterSegmentOverloads[nChannel]->setLevels(pMeterBallistics->getPeakMeterLevel(nChannel), pMeterBallistics->getPeakMeterPeakLevel(nChannel), pMeterBallistics->getMaximumPeakLevel(nChannel));

        if (bShowSplitMeters)
        {
            PeakMeterSignals[nChannel]->setLevel(pMeterBallistics->getPeakMeterSignal(nChannel));
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
