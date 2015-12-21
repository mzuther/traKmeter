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

#ifndef __METER_BALLISTICS_H__
#define __METER_BALLISTICS_H__

#include "JuceHeader.h"


//==============================================================================
/**
*/
class MeterBallistics
{
public:
    MeterBallistics(int nChannels, int CrestFactor, bool bPeakMeterInfiniteHold, bool bAverageMeterInfiniteHold, bool transient_mode);
    ~MeterBallistics();

    void setPeakMeterInfiniteHold(bool bInfiniteHold);
    void setAverageMeterInfiniteHold(bool bInfiniteHold);
    void reset();

    int getNumberOfChannels();

    int getCrestFactor();
    void setCrestFactor(int CrestFactor);

    float getPeakMeterLevel(int nChannel);
    float getPeakMeterPeakLevel(int nChannel);

    float getAverageMeterLevel(int nChannel);
    float getAverageMeterPeakLevel(int nChannel);

    float getSignalMeterReadout(int nChannel);
    float getMaximumPeakLevel(int nChannel);
    int getNumberOfOverflows(int nChannel);

    void updateChannel(int nChannel, float fTimePassed, float fPeak, float fRms, int nOverflows);

    static float level2decibel(float fLevel);
    static float decibel2level(float fDecibels);
    static double level2decibel_double(double dLevel);
    static double decibel2level_double(double dDecibels);
    static float getMeterMinimumDecibel();
private:
    JUCE_LEAK_DETECTOR(MeterBallistics);

    int nNumberOfChannels;
    int nCrestFactor;
    bool bTransientMode;

    static float fMeterMinimumDecibel;
    static float fPeakToAverageCorrection;

    Array<float> arrPeakMeterLevels;
    Array<float> arrPeakMeterPeakLevels;

    Array<float> arrAverageMeterLevels;
    Array<float> arrAverageMeterPeakLevels;

    Array<float> arrSignalMeterReadouts;
    Array<float> arrMaximumPeakLevels;
    Array<int> arrNumberOfOverflows;

    Array<float> arrPeakMeterPeakLastChanged;
    Array<float> arrAverageMeterPeakLastChanged;

    float PeakMeterBallistics(float fTimePassed, float fPeakLevelCurrent, float fPeakLevelOld);
    float PeakMeterPeakBallistics(float fTimePassed, float &fLastChanged, float fPeakLevelCurrent, float fPeakLevelOld);

    void AverageMeterBallistics(int nChannel, float fTimePassed, float fAverageLevelCurrent);
    float AverageMeterPeakBallistics(float fTimePassed, float &fLastChanged, float fAverageLevelCurrent, float fAverageLevelOld);

    void SignalMeterBallistics(int nChannel, float fTimePassed, float fPeakMeterSignalCurrent);

    void LogMeterBallistics(float fMeterInertia, float fTimePassed, float fLevel, float &fReadout);
};


#endif  // __METER_BALLISTICS_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
