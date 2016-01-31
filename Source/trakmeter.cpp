/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2016 Martin Zuther (http://www.mzuther.de/)

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

TraKmeter::TraKmeter(int posX, int posY, int nCrestFactor, int nNumChannels,
                     int segment_height, bool discreteMeter, int meter_type,
                     const Array<Colour> &averageMeterColours,
                     const Array<Colour> &peakMeterColours)

{
    // this component blends in with the background
    setOpaque(false);

    nInputChannels = nNumChannels;
    nMeterType = meter_type;
    bShowSplitMeters = (nMeterType == TraKmeterPluginParameters::selSplitMeters);

    int nThreshold = -90;
    nThreshold += nCrestFactor * 10;

    int nSegmentHeight;

    if (bShowSplitMeters)
    {
        nSegmentHeight = segment_height;
    }
    else
    {
        nSegmentHeight = segment_height - 1;
    }

    for (int nChannel = 0; nChannel < nInputChannels; ++nChannel)
    {
        MeterBarPeak *pMeterBarPeak = p_arrPeakMeters.add(
                                          new MeterBarPeak());

        pMeterBarPeak->create(nCrestFactor,
                              MeterBarPeak::orientationVertical,
                              discreteMeter,
                              !bShowSplitMeters,
                              nSegmentHeight,
                              peakMeterColours);

        addAndMakeVisible(pMeterBarPeak);

        MeterBarAverage *pMeterBarAverage = p_arrAverageMeters.add(
                                                new MeterBarAverage());

        pMeterBarAverage->create(nCrestFactor,
                                 MeterBarAverage::orientationVertical,
                                 discreteMeter,
                                 !bShowSplitMeters,
                                 nSegmentHeight,
                                 averageMeterColours);

        addAndMakeVisible(pMeterBarAverage);

        OverloadMeter *pOverloadMeter = p_arrOverloadMeters.add(new OverloadMeter("OverloadMeter", nThreshold * 0.1f, nCrestFactor));
        addAndMakeVisible(pOverloadMeter);
    }

    if (bShowSplitMeters)
    {
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

            GenericSignalLed *pGenericSignalLed = p_arrSignalLeds.add(new GenericSignalLed("Peak Meter Signal #" + String(nChannel)));
            addAndMakeVisible(pGenericSignalLed);
        }
    }
}


void TraKmeter::applySkin(Skin *pSkin)
{
    for (int nChannel = 0; nChannel < nInputChannels; ++nChannel)
    {
        pSkin->placeMeterBar(p_arrAverageMeters[nChannel], "meter_average_" + String(nChannel + 1));
        pSkin->placeMeterBar(p_arrPeakMeters[nChannel], "meter_peak_" + String(nChannel + 1));

        pSkin->placeAndSkinStateLabel(p_arrOverloadMeters[nChannel], "label_over_" + String(nChannel + 1));

        if (bShowSplitMeters)
        {
            pSkin->placeAndSkinSignalLed(p_arrSignalLeds[nChannel], "label_signal_" + String(nChannel + 1));
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

        if (bShowSplitMeters)
        {
            p_arrSignalLeds[nChannel]->setLevel(
                pMeterBallistics->getSignalMeterReadout(nChannel));
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
