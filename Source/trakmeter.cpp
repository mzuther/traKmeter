/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2019 Martin Zuther (http://www.mzuther.de/)

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

TraKmeter::TraKmeter(int nNumChannels, int segment_height,
                     float retainSignalFactor, float newSignalFactor,
                     bool discreteMeter, int targetRecordingLevel,
                     const Array<Colour> &averageMeterColours,
                     const Array<Colour> &peakMeterColours)

{
    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;
    int nSegmentHeight = segment_height;

    int nThreshold = (targetRecordingLevel * 10) + 10;

    for (int nChannel = 0; nChannel < nInputChannels; ++nChannel)
    {
        MeterBarPeak *pMeterBarPeak = p_arrPeakMeters.add(
                                          new MeterBarPeak());

        pMeterBarPeak->create(retainSignalFactor,
                              newSignalFactor,
                              frut::widgets::Orientation::vertical,
                              discreteMeter,
                              targetRecordingLevel,
                              nSegmentHeight,
                              peakMeterColours);

        addAndMakeVisible(pMeterBarPeak);

        MeterBarAverage *pMeterBarAverage = p_arrAverageMeters.add(
                                                new MeterBarAverage());

        pMeterBarAverage->create(retainSignalFactor,
                                 newSignalFactor,
                                 frut::widgets::Orientation::vertical,
                                 discreteMeter,
                                 targetRecordingLevel,
                                 nSegmentHeight,
                                 averageMeterColours);

        addAndMakeVisible(pMeterBarAverage);

        OverloadMeter *overloadMeter = new OverloadMeter(
            nThreshold * 0.1f);
        addAndMakeVisible(overloadMeter);

        p_arrOverloadMeters.add(overloadMeter);
    }

    String strLabel;

    for (int nChannel = 0; nChannel < nInputChannels; ++nChannel)
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

        frut::widgets::SignalLed *signalLed = new frut::widgets::SignalLed();
        addAndMakeVisible(signalLed);

        p_arrSignalLeds.add(signalLed);
    }
}


void TraKmeter::applySkin(Skin *pSkin)
{
    for (int nChannel = 0; nChannel < nInputChannels; ++nChannel)
    {
        pSkin->placeMeterBar("meter_average_" + String(nChannel + 1),
                             p_arrAverageMeters[nChannel]);
        pSkin->placeMeterBar("meter_peak_" + String(nChannel + 1),
                             p_arrPeakMeters[nChannel]);

        pSkin->placeAndSkinStateLabel("label_over_" + String(nChannel + 1),
                                      p_arrOverloadMeters[nChannel]);

        pSkin->placeAndSkinSignalLed("label_signal_" + String(nChannel + 1),
                                     p_arrSignalLeds[nChannel]);
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
    for (int nChannel = 0; nChannel < nInputChannels; ++nChannel)
    {
        p_arrAverageMeters[nChannel]->setNormalLevels(
            pMeterBallistics->getAverageMeterLevel(nChannel),
            pMeterBallistics->getAverageMeterPeakLevel(nChannel));

        p_arrPeakMeters[nChannel]->setNormalLevels(
            pMeterBallistics->getPeakMeterLevel(nChannel),
            pMeterBallistics->getPeakMeterPeakLevel(nChannel));

        p_arrOverloadMeters[nChannel]->setLevels(
            pMeterBallistics->getPeakMeterLevel(nChannel),
            pMeterBallistics->getMaximumPeakLevel(nChannel));

        p_arrSignalLeds[nChannel]->setLevel(
            pMeterBallistics->getSignalMeterReadout(nChannel));
    }
}
