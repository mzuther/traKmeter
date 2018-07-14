/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2018 Martin Zuther (http://www.mzuther.de/)

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

#ifndef TRAKMETER_METER_BALLISTICS_H
#define TRAKMETER_METER_BALLISTICS_H

#include "FrutHeader.h"


class MeterBallistics
{
public:
    MeterBallistics(int nChannels,
                    bool bPeakMeterInfiniteHold,
                    bool bAverageMeterInfiniteHold);

    void setPeakMeterInfiniteHold(bool bInfiniteHold);
    void setAverageMeterInfiniteHold(bool bInfiniteHold);
    void reset();

    int getNumberOfChannels();

    float getPeakMeterLevel(int nChannel);
    float getPeakMeterPeakLevel(int nChannel);

    float getAverageMeterLevel(int nChannel);
    float getAverageMeterPeakLevel(int nChannel);

    float getSignalMeterReadout(int nChannel);
    float getMaximumPeakLevel(int nChannel);

    void updateChannel(int nChannel,
                       float fTimePassed,
                       float fPeak,
                       float fRms);

    static float level2decibel(float fLevel);
    static double level2decibel_double(double dLevel);

    static float decibel2level(float fDecibels);
    static double decibel2level_double(double dDecibels);

    static float getMeterMinimumDecibel();
private:
    JUCE_LEAK_DETECTOR(MeterBallistics);

    int nNumberOfChannels;

    static float fMeterMinimumDecibel;
    static float fPeakToAverageCorrection;

    Array<float> arrPeakMeterLevels;
    Array<float> arrPeakMeterPeakLevels;

    Array<float> arrAverageMeterLevels;
    Array<float> arrAverageMeterPeakLevels;

    Array<float> arrSignalMeterReadouts;
    Array<float> arrMaximumPeakLevels;

    Array<float> arrPeakMeterPeakLastChanged;
    Array<float> arrAverageMeterPeakLastChanged;

    float PeakMeterBallistics(float fTimePassed,
                              float fPeakLevelCurrent,
                              float fPeakLevelOld);

    float PeakMeterPeakBallistics(float fTimePassed,
                                  float &fLastChanged,
                                  float fPeakLevelCurrent,
                                  float fPeakLevelOld);

    void AverageMeterBallistics(int nChannel,
                                float fTimePassed,
                                float fAverageLevelCurrent);

    float AverageMeterPeakBallistics(float fTimePassed,
                                     float &fLastChanged,
                                     float fAverageLevelCurrent,
                                     float fAverageLevelOld);

    void SignalMeterBallistics(int nChannel,
                               float fTimePassed,
                               float fPeakMeterSignalCurrent);

    void LogMeterBallistics(float fMeterInertia,
                            float fTimePassed,
                            float fLevel,
                            float &fReadout);
};

#endif  // TRAKMETER_METER_BALLISTICS_H
