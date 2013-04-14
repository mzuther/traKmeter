/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2013 Martin Zuther (http://www.mzuther.de/)

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

#include "meter_ballistics.h"


// RMS peak-to-average gain correction; this is simply the difference
// between peak and average meter readings during validation, measured
// using pure sines
float MeterBallistics::fPeakToAverageCorrection = MeterBallistics::level2decibel(sqrtf(2.0f));

// logarithmic levels have no minimum level, so let's define one:
// (70 dB meter range + 0.01 to make sure that the minimum level is
// below the meter's threshold + 20 dB maximum crest factor +
// peak-to-average gain correction)
float MeterBallistics::fMeterMinimumDecibel = -(70.01f + 20.0f + fPeakToAverageCorrection);


MeterBallistics::MeterBallistics(int nChannels, int CrestFactor, bool bPeakMeterInfiniteHold, bool bAverageMeterInfiniteHold, bool transient_mode)
/*  Constructor.

    nChannels (integer): number of audio input channels

    bPeakMeterInfiniteHold (Boolean): selects "infinite peak hold"
    (true) or "falling peaks" mode (false) for peak meter

    bAverageMeterInfiniteHold (Boolean): selects "infinite peak hold"
    (true) or "falling peaks" mode (false) for average meter

    transient_mode (Boolean): selects "transient mode" (true) or
    "averaging mode" (false) for average meter

    return value: none
*/
{
    // store the number of audio input channels
    nNumberOfChannels = nChannels;

    // store meter's crest factor
    setCrestFactor(CrestFactor);

    // store setting for transient mode
    bTransientMode = transient_mode;

    // allocate variables for peak meter's level and peak mark (all
    // audio input channels)
    fPeakMeterLevels = new float[nNumberOfChannels];
    fPeakMeterPeakLevels = new float[nNumberOfChannels];

    // allocate variables for average meter's level and peak mark (all
    // audio input channels)
    fAverageMeterLevels = new float[nNumberOfChannels];
    fAverageMeterPeakLevels = new float[nNumberOfChannels];

    // allocate variables for the time since the peak mark was last
    // changed (all audio input channels)
    fPeakMeterPeakLastChanged = new float[nNumberOfChannels];
    fAverageMeterPeakLastChanged = new float[nNumberOfChannels];

    // allocate variables for peak meter signal level
    fPeakMeterSignals = new float[nNumberOfChannels];

    // allocate variables for overall maximum peak level and number of
    // registered overflows (all audio input channels)
    fMaximumPeakLevels = new float[nNumberOfChannels];
    nNumberOfOverflows = new int[nNumberOfChannels];

    // select "infinite peak hold" or "falling peaks" mode
    setPeakMeterInfiniteHold(bPeakMeterInfiniteHold);
    setAverageMeterInfiniteHold(bAverageMeterInfiniteHold);

    // reset (i.e. initialise) all meter readings
    reset();
}


MeterBallistics::~MeterBallistics()
/*  Destructor.

    return value: none
*/
{
    // delete all allocated variables
    delete [] fPeakMeterLevels;
    fPeakMeterLevels = NULL;

    delete [] fPeakMeterPeakLevels;
    fPeakMeterPeakLevels = NULL;

    delete [] fAverageMeterLevels;
    fAverageMeterLevels = NULL;

    delete [] fAverageMeterPeakLevels;
    fAverageMeterPeakLevels = NULL;

    delete [] fPeakMeterPeakLastChanged;
    fPeakMeterPeakLastChanged = NULL;

    delete [] fAverageMeterPeakLastChanged;
    fAverageMeterPeakLastChanged = NULL;

    delete [] fPeakMeterSignals;
    fPeakMeterSignals = NULL;

    delete [] fMaximumPeakLevels;
    fMaximumPeakLevels = NULL;

    delete [] nNumberOfOverflows;
    nNumberOfOverflows = NULL;
}


void MeterBallistics::reset()
/*  Reset all meter readings

    return value: none
*/
{
    // loop through all audio channels
    for (int nChannel = 0; nChannel < nNumberOfChannels; nChannel++)
    {
        // set peak meter's level and peak mark to meter's minimum
        fPeakMeterLevels[nChannel] = fMeterMinimumDecibel;
        fPeakMeterPeakLevels[nChannel] = fMeterMinimumDecibel;

        // set average meter's level and peak mark to meter's minimum
        fAverageMeterLevels[nChannel] = fMeterMinimumDecibel;
        fAverageMeterPeakLevels[nChannel] = fMeterMinimumDecibel;

        // set peak meter signal levels to meter's minimum
        fPeakMeterSignals[nChannel] = fMeterMinimumDecibel;

        // set overall maximum peak level to meter's minimum
        fMaximumPeakLevels[nChannel] = fMeterMinimumDecibel;

        // reset number of registered overflows
        nNumberOfOverflows[nChannel] = 0;
    }
}


void MeterBallistics::setPeakMeterInfiniteHold(bool bInfiniteHold)
/*  Set peak meter to "infinite peak hold" or "falling peaks" mode.

    bInfiniteHold (Boolean): selects "infinite peak hold" (true) or
    "falling peaks" mode (false)

    return value: none
*/
{
    // loop through all audio channels
    for (int nChannel = 0; nChannel < nNumberOfChannels; nChannel++)
    {
        // negative times will not be processed by meter ballistics,
        // so this effectively selects "infinite peak hold" mode
        if (bInfiniteHold)
        {
            fPeakMeterPeakLastChanged[nChannel] = -1.0f;
        }
        // select "falling peaks" mode by resetting time since peak
        // mark was last changed
        else
        {
            fPeakMeterPeakLastChanged[nChannel] = 0.0f;
        }
    }
}


void MeterBallistics::setAverageMeterInfiniteHold(bool bInfiniteHold)
/*  Set average meter to "infinite peak hold" or "falling peaks" mode.

    bInfiniteHold (Boolean): selects "infinite peak hold" (true) or
    "falling peaks" mode (false)

    return value: none
*/
{
    // loop through all audio channels
    for (int nChannel = 0; nChannel < nNumberOfChannels; nChannel++)
    {
        // negative times will not be processed by meter ballistics,
        // so this effectively selects "infinite peak hold" mode
        if (bInfiniteHold)
        {
            fAverageMeterPeakLastChanged[nChannel] = -1.0f;
        }
        // select "falling peaks" mode by resetting time since peak mark
        // was last changed
        else
        {
            fAverageMeterPeakLastChanged[nChannel] = 0.0f;
        }
    }
}


int MeterBallistics::getNumberOfChannels()
/*  Get number of audio channels.

    return value (integer): returns the current number of processed
    audio channels
*/
{
    return nNumberOfChannels;
}


int MeterBallistics::getCrestFactor()
/*  Get current crest factor.

    return value (integer): returns the current crest factor in decibel
*/
{
    return nCrestFactor;
}


void MeterBallistics::setCrestFactor(int CrestFactor)
/*  Set current crest factor.

    CrestFactor (integer): current crest factor in decibel

    return value: none
*/
{
    nCrestFactor = CrestFactor;
}


float MeterBallistics::getPeakMeterLevel(int nChannel)
/*  Get current level of an audio channel's peak level meter.

    nChannel (integer): selected audio channel

    return value (float): returns the current level in decibel of the
    given audio channel's peak level meter
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nNumberOfChannels);

    return fPeakMeterLevels[nChannel] + nCrestFactor;
}


float MeterBallistics::getPeakMeterPeakLevel(int nChannel)
/*  Get peak level of an audio channel's peak level meter.

    nChannel (integer): selected audio channel

    return value (float): returns the (changing) peak level in decibel
    of the given audio channel's peak level meter
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nNumberOfChannels);

    return fPeakMeterPeakLevels[nChannel] + nCrestFactor;
}


float MeterBallistics::getAverageMeterLevel(int nChannel)
/*  Get current level of an audio channel's average level meter.

    nChannel (integer): selected audio channel

    return value (float): returns the current level in decibel of the
    given audio channel's average level meter
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nNumberOfChannels);

    return fAverageMeterLevels[nChannel] + nCrestFactor;
}


float MeterBallistics::getAverageMeterPeakLevel(int nChannel)
/*  Get peak level of an audio channel's average level meter.

    nChannel (integer): selected audio channel

    return value (float): returns the (changing) peak level in decibel
    of the given audio channel's average level meter
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nNumberOfChannels);

    return fAverageMeterPeakLevels[nChannel] + nCrestFactor;
}


float MeterBallistics::getPeakMeterSignal(int nChannel)
/*  Get peak meter signal level of an audio channel.

    nChannel (integer): selected audio channel

    return value (float): returns the peak meter signal level in
    decibel that has been registered on the given audio channel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nNumberOfChannels);

    return fPeakMeterSignals[nChannel] + nCrestFactor;
}


float MeterBallistics::getMaximumPeakLevel(int nChannel)
/*  Get overall maximum peak level of an audio channel.

    nChannel (integer): selected audio channel

    return value (float): returns the overall maximum peak level in
    decibel that has been registered on the given audio channel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nNumberOfChannels);

    return fMaximumPeakLevels[nChannel] + nCrestFactor;
}


int MeterBallistics::getNumberOfOverflows(int nChannel)
/*  Get number of overflows of an audio channel.

    nChannel (integer): selected audio channel

    return value (integer): returns the number of overflows that has
    been registered on the given audio channel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nNumberOfChannels);

    return nNumberOfOverflows[nChannel];
}


void MeterBallistics::updateChannel(int nChannel, float fTimePassed, float fPeak, float fRms, int nOverflows)
/*  Update audio levels, overflows and apply meter ballistics.

    nChannel (integer): audio input channel to update

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fPeak (float): current peak meter level (linear scale)

    fRms (float): current RMS level (linear scale)

    nOverflows (integer): number of overflows in buffer chunk

    return value: none
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nNumberOfChannels);

    // convert current peak meter level from linear scale to decibels
    fPeak = level2decibel(fPeak);

    // convert current RMS level from linear scale to decibels and
    // apply peak-to-average correction
    fRms = level2decibel(fRms) + fPeakToAverageCorrection;

    // if current peak meter level exceeds overall maximum peak level,
    // store it as new overall maximum peak level
    if (fPeak > fMaximumPeakLevels[nChannel])
    {
        fMaximumPeakLevels[nChannel] = fPeak;
    }

    // apply peak meter's ballistics and store resulting level and
    // peak mark
    fPeakMeterLevels[nChannel] = PeakMeterBallistics(fTimePassed, fPeak, fPeakMeterLevels[nChannel]);
    fPeakMeterPeakLevels[nChannel] = PeakMeterPeakBallistics(fTimePassed, &fPeakMeterPeakLastChanged[nChannel], fPeak, fPeakMeterPeakLevels[nChannel]);

    PeakMeterSignalBallistics(nChannel, fTimePassed, fPeak);

    // apply average meter's ballistics and store resulting level and
    // peak mark
    AverageMeterBallistics(nChannel, fTimePassed, fRms);
    fAverageMeterPeakLevels[nChannel] = AverageMeterPeakBallistics(fTimePassed, &fAverageMeterPeakLastChanged[nChannel], fAverageMeterLevels[nChannel], fAverageMeterPeakLevels[nChannel]);

    // update registered number of overflows
    nNumberOfOverflows[nChannel] += nOverflows;
}


float MeterBallistics::level2decibel(float fLevel)
/*  Convert level from linear scale to decibels (dB).

    fLevel (float): audio level

    return value (float): returns given level in decibels (dB) when
    above "fMeterMinimumDecibel", otherwise "fMeterMinimumDecibel"
*/
{
    // log(0) is not defined, so return "fMeterMinimumDecibel"
    if (fLevel == 0.0f)
    {
        return fMeterMinimumDecibel;
    }
    else
    {
        // calculate decibels from audio level (a factor of 20.0 is
        // needed to calculate *level* ratios, whereas 10.0 is needed
        // for *power* ratios!)
        float fDecibels = 20.0f * log10f(fLevel);

        // to make meter ballistics look nice for low levels, do not
        // return levels below "fMeterMinimumDecibel"
        if (fDecibels < fMeterMinimumDecibel)
        {
            return fMeterMinimumDecibel;
        }
        else
        {
            return fDecibels;
        }
    }
}


float MeterBallistics::decibel2level(float fDecibels)
/*  Convert level from decibels (dB) to linear scale.

    fLevel (float): audio level in decibels (dB)

    return value (float): given level in linear scale
*/
{
    // calculate audio level from decibels (a divisor of 20.0 is
    // needed to calculate *level* ratios, whereas 10.0 is needed for
    // *power* ratios!)
    float fLevel = powf(10.0f, fDecibels / 20.0f);
    return fLevel;
}


float MeterBallistics::getMeterMinimumDecibel()
{
    return fMeterMinimumDecibel;
}


float MeterBallistics::PeakMeterBallistics(float fTimePassed, float fPeakLevelCurrent, float fPeakLevelOld)
/*  Calculate ballistics for peak meter levels.

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fPeakLevelCurrent (float): current peak meter level in decibel

    fPeakLevelOld (float): old peak meter reading in decibel

    return value (float): new peak meter reading in decibel
*/
{
    // apply rise time if peak level is above old level
    if (fPeakLevelCurrent >= fPeakLevelOld)
    {
        // immediate rise time, so return current peak level as new
        // peak meter reading
        return fPeakLevelCurrent;
    }
    // otherwise, apply fall time
    else
    {
        // fall time: 26 dB in 3 seconds (linear)
        float fReleaseCoef = 26.0f * fTimePassed / 3.0f;

        // apply fall time and return new peak meter reading
        return fPeakLevelOld - fReleaseCoef;
    }
}


float MeterBallistics::PeakMeterPeakBallistics(float fTimePassed, float* fLastChanged, float fPeakCurrent, float fPeakOld)
/*  Calculate ballistics for peak meter peak marks.

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fLastChanged (float pointer): time since peak mark was last
    changed in fractional seconds

    fPeakCurrent (float): current peak level mark in decibel

    fPeakOld (float): old peak level mark in decibel

    return value (float): new peak level mark in decibel
*/
{
    float fOutput;

    // prevent meter overshoot on overflows by limiting peak levels to
    // 0.0 dBFS
    if (fPeakCurrent >= 0.0f)
    {
        fPeakCurrent = 0.0f;
    }

    // apply rise time if peak level is above old level
    if (fPeakCurrent >= fPeakOld)
    {
        // if peak meter is set to "falling peaks" mode (non-negative
        // values), reset hold time (time that peaks are held before
        // starting to fall back down)
        if (*fLastChanged >= 0.0f)
        {
            *fLastChanged = 0.0f;
        }

        // immediate rise time, so set current peak level mark as new
        // peak level mark
        fOutput = fPeakCurrent;
    }
    // otherwise, test whether to apply fall time
    else
    {
        // if peak meter is set to "falling peaks" mode (non-negative
        // values), update hold time (time that peaks are held before
        // starting to fall back down)
        if (*fLastChanged >= 0.0f)
        {
            *fLastChanged += fTimePassed;
        }

        // peak meter is EITHER set to "infinite peak hold" mode
        // (negative values) OR the peak meter's hold time of 10
        // seconds has not yet been exceeded, so retain old peak level
        // mark
        if (*fLastChanged < 10.0f)
        {
            fOutput = fPeakOld;
        }
        // peak meter's hold time (10 seconds) has been reached or
        // exceeded
        else
        {
            // fall time: 26 dB in 3 seconds (linear)
            float fReleaseCoef = 26.0f * fTimePassed / 3.0f;

            // apply fall time
            fOutput = fPeakOld - fReleaseCoef;
        }
    }

    // finally, return new peak level mark
    return fOutput;
}


void MeterBallistics::AverageMeterBallistics(int nChannel, float fTimePassed, float fAverageLevelCurrent)
/*  Calculate ballistics for average meter levels and update readout.

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fAverageLevelCurrent (float): current average meter level in
    decibel

    return value: none
*/
{
    // in "transient mode", the meter rises to 99% of the final
    // reading in 10 ms (logarithmic) and has a fall time of 6 dB per
    // second (linear)
    if (bTransientMode)
    {
        // meter is falling
        if (fAverageLevelCurrent < fAverageMeterLevels[nChannel])
        {
            // fall time: 6 dB per second (linear)
            float fReleaseCoef = 6.0f * fTimePassed;

            // apply fall time
            fAverageMeterLevels[nChannel] -= fReleaseCoef;

            // make sure that meter doesn't fall below current level
            if (fAverageLevelCurrent > fAverageMeterLevels[nChannel])
            {
                fAverageMeterLevels[nChannel] = fAverageLevelCurrent;
            }
        }
        // meter is rising
        else
        {
            // meter reaches 99% of the final reading in 10 ms
            LogMeterBallistics(0.010f, fTimePassed, fAverageLevelCurrent, fAverageMeterLevels[nChannel]);
        }
    }
    // in "classic mode", the meter reaches 99% of the final reading
    // in 300 ms (logarithmic)
    else
    {
        LogMeterBallistics(0.300f, fTimePassed, fAverageLevelCurrent, fAverageMeterLevels[nChannel]);
    }
}


float MeterBallistics::AverageMeterPeakBallistics(float fTimePassed, float* fLastChanged, float fPeakCurrent, float fPeakOld)
/*  Calculate ballistics for average meter peak marks.

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fLastChanged (float pointer): time since peak mark was last
    changed in fractional seconds

    fPeakCurrent (float): current peak level mark in decibel

    fPeakOld (float): old peak level mark in decibel

    return value (float): new peak level mark in decibel
*/
{
    // the peak marks ballistics of peak meter and average meter are
    // identical, so let's reuse the peak meter code
    return PeakMeterPeakBallistics(fTimePassed, fLastChanged, fPeakCurrent, fPeakOld);
}


void MeterBallistics::PeakMeterSignalBallistics(int nChannel, float fTimePassed, float fPeakMeterSignalCurrent)
/*  Calculate ballistics for peak meter signal levels and update readout.

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fPeakMeterSignalCurrent (float): current peak meter signal level in
    decibel

    return value: none
*/
{
    // meter is rising
    if (fPeakMeterSignalCurrent >= fPeakMeterSignals[nChannel])
    {
        // immediate rise time, so return current peak meter signal
        // level as new reading
        fPeakMeterSignals[nChannel] = fPeakMeterSignalCurrent;
    }
    // meter is falling
    else
    {
        // meter reaches 99% of the final reading in 1200 ms
        LogMeterBallistics(1.200f, fTimePassed, fPeakMeterSignalCurrent, fPeakMeterSignals[nChannel]);
    }
}


void MeterBallistics::LogMeterBallistics(float fMeterInertia, float fTimePassed, float fLevel, float& fReadout)
/*  Calculate logarithmic meter ballistics.

    fMeterInertia (float): time needed to reach 99% of the final
    readout (in fractional seconds)

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fLevel (float): new meter level

    fReadout (reference to float): old meter readout; this variable
    will be updated by this function

    return value: none
*/
{
    // we only have to calculate meter ballistics if meter level and
    // meter readout are not equal
    if (fLevel != fReadout)
    {
        // Thanks to Bram from Smartelectronix for the code snippet!
        // (http://www.musicdsp.org/showone.php?id=136)
        //
        // rise and fall: 99% of final reading in "fMeterInertia" seconds
        float fAttackReleaseCoef = powf(0.01f, fTimePassed / fMeterInertia);
        fReadout = fAttackReleaseCoef * (fReadout - fLevel) + fLevel;
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
