/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2020 Martin Zuther (http://www.mzuther.de/)

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
float MeterBallistics::fPeakToAverageCorrection = MeterBallistics::level2decibel(
                                                     sqrtf( 2.0f ) );

// logarithmic levels have no minimum level, so let's define one:
// (90 dB meter range + 0.01 to make sure that the minimum level is
// below the meter's threshold + 20 dB maximum crest factor +
// peak-to-average gain correction)
float MeterBallistics::fMeterMinimumDecibel = -( 90.01f + 20.0f +
                                                 fPeakToAverageCorrection );


MeterBallistics::MeterBallistics( int nChannels,
                                  bool bPeakMeterInfiniteHold,
                                  bool bAverageMeterInfiniteHold )
/*  Constructor.

    nChannels (integer): number of audio input channels

    bPeakMeterInfiniteHold (Boolean): selects "infinite peak hold"
    (true) or "falling peaks" mode (false) for peak meter

    bAverageMeterInfiniteHold (Boolean): selects "infinite peak hold"
    (true) or "falling peaks" mode (false) for average meter

    return value: none
*/
{
   // store the number of audio input channels
   nNumberOfChannels = nChannels;

   // select "infinite peak hold" or "falling peaks" mode
   setPeakMeterInfiniteHold( bPeakMeterInfiniteHold );
   setAverageMeterInfiniteHold( bAverageMeterInfiniteHold );

   // reset and initialise all meter readings
   reset();
}


void MeterBallistics::reset()
/*  Reset all meter readings

    return value: none
*/
{
   // loop through all audio channels
   for ( int nChannel = 0; nChannel < nNumberOfChannels; ++nChannel ) {
      // set peak meter's level and peak mark to meter's minimum
      arrPeakMeterLevels.set( nChannel, fMeterMinimumDecibel );
      arrPeakMeterPeakLevels.set( nChannel, fMeterMinimumDecibel );

      // set average meter's level and peak mark to meter's minimum
      arrAverageMeterLevels.set( nChannel, fMeterMinimumDecibel );
      arrAverageMeterPeakLevels.set( nChannel, fMeterMinimumDecibel );

      // set signal meter readout to minimum
      arrSignalMeterReadouts.set( nChannel, 0.0f );

      // set overall maximum peak level to meter's minimum
      arrMaximumPeakLevels.set( nChannel, fMeterMinimumDecibel );
   }
}


void MeterBallistics::setPeakMeterInfiniteHold( bool bInfiniteHold )
/*  Set peak meter to "infinite peak hold" or "falling peaks" mode.

    bInfiniteHold (Boolean): selects "infinite peak hold" (true) or
    "falling peaks" mode (false)

    return value: none
*/
{
   // loop through all audio channels
   for ( int nChannel = 0; nChannel < nNumberOfChannels; ++nChannel ) {
      // negative times will not be processed by meter ballistics,
      // so this effectively selects "infinite peak hold" mode
      if ( bInfiniteHold ) {
         arrPeakMeterPeakLastChanged.set( nChannel, -1.0f );
         // select "falling peaks" mode by resetting time since peak
         // mark was last changed
      } else {
         arrPeakMeterPeakLastChanged.set( nChannel, 0.0f );
      }
   }
}


void MeterBallistics::setAverageMeterInfiniteHold( bool bInfiniteHold )
/*  Set average meter to "infinite peak hold" or "falling peaks" mode.

    bInfiniteHold (Boolean): selects "infinite peak hold" (true) or
    "falling peaks" mode (false)

    return value: none
*/
{
   // loop through all audio channels
   for ( int nChannel = 0; nChannel < nNumberOfChannels; ++nChannel ) {
      // negative times will not be processed by meter ballistics,
      // so this effectively selects "infinite peak hold" mode
      if ( bInfiniteHold ) {
         arrAverageMeterPeakLastChanged.set( nChannel, -1.0f );
         // select "falling peaks" mode by resetting time since peak mark
         // was last changed
      } else {
         arrAverageMeterPeakLastChanged.set( nChannel, 0.0f );
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


float MeterBallistics::getPeakMeterLevel( int nChannel )
/*  Get current level of an audio channel's peak level meter.

    nChannel (integer): selected audio channel

    return value (float): returns the current level in decibel of the
    given audio channel's peak level meter
*/
{
   jassert( nChannel >= 0 );
   jassert( nChannel < nNumberOfChannels );

   return arrPeakMeterLevels[nChannel];
}


float MeterBallistics::getPeakMeterPeakLevel( int nChannel )
/*  Get peak level of an audio channel's peak level meter.

    nChannel (integer): selected audio channel

    return value (float): returns the (changing) peak level in decibel
    of the given audio channel's peak level meter
*/
{
   jassert( nChannel >= 0 );
   jassert( nChannel < nNumberOfChannels );

   return arrPeakMeterPeakLevels[nChannel];
}


float MeterBallistics::getAverageMeterLevel( int nChannel )
/*  Get current level of an audio channel's average level meter.

    nChannel (integer): selected audio channel

    return value (float): returns the current level in decibel of the
    given audio channel's average level meter
*/
{
   jassert( nChannel >= 0 );
   jassert( nChannel < nNumberOfChannels );

   return arrAverageMeterLevels[nChannel];
}


float MeterBallistics::getAverageMeterPeakLevel( int nChannel )
/*  Get peak level of an audio channel's average level meter.

    nChannel (integer): selected audio channel

    return value (float): returns the (changing) peak level in decibel
    of the given audio channel's average level meter
*/
{
   jassert( nChannel >= 0 );
   jassert( nChannel < nNumberOfChannels );

   return arrAverageMeterPeakLevels[nChannel];
}


float MeterBallistics::getSignalMeterReadout( int nChannel )
/*  Get signal meter readout of an audio channel.

    nChannel (integer): selected audio channel

    return value (float): returns the signal meter readout (between
    0.0 and 1.0) that has been registered on the given audio channel
*/
{
   jassert( nChannel >= 0 );
   jassert( nChannel < nNumberOfChannels );

   return arrSignalMeterReadouts[nChannel];
}


float MeterBallistics::getMaximumPeakLevel( int nChannel )
/*  Get overall maximum peak level of an audio channel.

    nChannel (integer): selected audio channel

    return value (float): returns the overall maximum peak level in
    decibel that has been registered on the given audio channel
*/
{
   jassert( nChannel >= 0 );
   jassert( nChannel < nNumberOfChannels );

   return arrMaximumPeakLevels[nChannel];
}


void MeterBallistics::updateChannel( int nChannel,
                                     float fTimePassed,
                                     float fPeak,
                                     float fRms )
/*  Update audio levels and apply meter ballistics.

    nChannel (integer): audio input channel to update

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fPeak (float): current peak meter level (linear scale)

    fRms (float): current RMS level (linear scale)

    return value: none
*/
{
   jassert( nChannel >= 0 );
   jassert( nChannel < nNumberOfChannels );

   // convert current peak meter level from linear scale to decibels
   fPeak = level2decibel( fPeak );

   // convert current RMS level from linear scale to decibels and
   // apply peak-to-average correction
   fRms = level2decibel( fRms ) + fPeakToAverageCorrection;

   // if current peak meter level exceeds overall maximum peak level,
   // store it as new overall maximum peak level
   if ( fPeak > arrMaximumPeakLevels[nChannel] ) {
      arrMaximumPeakLevels.set( nChannel, fPeak );
   }

   // apply peak meter's ballistics and store resulting level and
   // peak mark
   arrPeakMeterLevels.set( nChannel, PeakMeterBallistics( fTimePassed, fPeak, arrPeakMeterLevels[nChannel] ) );
   arrPeakMeterPeakLevels.set( nChannel, PeakMeterPeakBallistics( fTimePassed, arrPeakMeterPeakLastChanged.getReference( nChannel ), fPeak, arrPeakMeterPeakLevels[nChannel] ) );

   SignalMeterBallistics( nChannel, fTimePassed, fPeak );

   // apply average meter's ballistics and store resulting level and
   // peak mark
   AverageMeterBallistics( nChannel, fTimePassed, fRms );
   arrAverageMeterPeakLevels.set( nChannel, AverageMeterPeakBallistics( fTimePassed, arrAverageMeterPeakLastChanged.getReference( nChannel ), arrAverageMeterLevels[nChannel], arrAverageMeterPeakLevels[nChannel] ) );
}


float MeterBallistics::level2decibel( float fLevel )
/*  Convert level from linear scale to decibels (dB).

    fLevel (float): audio level

    return value (float): returns given level in decibels (dB) when
    above "fMeterMinimumDecibel", otherwise "fMeterMinimumDecibel"
*/
{
   // log(0) is not defined, so return "fMeterMinimumDecibel"
   if ( fLevel == 0.0f ) {
      return fMeterMinimumDecibel;
   } else {
      // calculate decibels from audio level (a factor of 20.0 is
      // needed to calculate *level* ratios, whereas 10.0 is needed
      // for *power* ratios!)
      float fDecibels = 20.0f * log10f( fLevel );

      // to make meter ballistics look nice for low levels, do not
      // return levels below "fMeterMinimumDecibel"
      if ( fDecibels < fMeterMinimumDecibel ) {
         return fMeterMinimumDecibel;
      } else {
         return fDecibels;
      }
   }
}


double MeterBallistics::level2decibel_double( double dLevel )
/*  Convert level from linear scale to decibels (dB).

    dLevel (double): audio level

    return value (double): returns given level in decibels (dB) when
    above "fMeterMinimumDecibel", otherwise "fMeterMinimumDecibel"
*/
{
   // log(0) is not defined, so return "fMeterMinimumDecibel"
   if ( dLevel == 0.0 ) {
      return static_cast<double>( fMeterMinimumDecibel );
   } else {
      // calculate decibels from audio level (a factor of 20.0 is
      // needed to calculate *level* ratios, whereas 10.0 is needed
      // for *power* ratios!)
      double dDecibels = 20.0 * log10( dLevel );

      // to make meter ballistics look nice for low levels, do not
      // return levels below "fMeterMinimumDecibel"
      if ( dDecibels < fMeterMinimumDecibel ) {
         return static_cast<double>( fMeterMinimumDecibel );
      } else {
         return dDecibels;
      }
   }
}


float MeterBallistics::decibel2level( float fDecibels )
/*  Convert level from decibels (dB) to linear scale.

    fLevel (float): audio level in decibels (dB)

    return value (float): given level in linear scale
*/
{
   // calculate audio level from decibels (a divisor of 20.0 is
   // needed to calculate *level* ratios, whereas 10.0 is needed for
   // *power* ratios!)
   float fLevel = powf( 10.0f, fDecibels / 20.0f );
   return fLevel;
}


double MeterBallistics::decibel2level_double( double dDecibels )
/*  Convert level from decibels (dB) to linear scale.

    dLevel (double): audio level in decibels (dB)

    return value (double): given level in linear scale
*/
{
   // calculate audio level from decibels (a divisor of 20.0 is
   // needed to calculate *level* ratios, whereas 10.0 is needed for
   // *power* ratios!)
   double dLevel = pow( 10.0, dDecibels / 20.0 );
   return dLevel;
}


float MeterBallistics::getMeterMinimumDecibel()
{
   return fMeterMinimumDecibel;
}


float MeterBallistics::PeakMeterBallistics( float fTimePassed,
                                            float fPeakLevelCurrent,
                                            float fPeakLevelOld )
/*  Calculate ballistics for peak meter levels.

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fPeakLevelCurrent (float): current peak meter level in decibel

    fPeakLevelOld (float): old peak meter reading in decibel

    return value (float): new peak meter reading in decibel
*/
{
   // apply rise time if peak level is above old level
   if ( fPeakLevelCurrent >= fPeakLevelOld ) {
      // immediate rise time, so return current peak level as new
      // peak meter reading
      return fPeakLevelCurrent;
      // otherwise, apply fall time
   } else {
      // fall time: 26 dB in 3 seconds (linear)
      float fReleaseCoef = 26.0f * fTimePassed / 3.0f;

      // apply fall time and return new peak meter reading
      return fPeakLevelOld - fReleaseCoef;
   }
}


float MeterBallistics::PeakMeterPeakBallistics( float fTimePassed,
                                                float& fLastChanged,
                                                float fPeakCurrent,
                                                float fPeakOld )
/*  Calculate ballistics for peak meter peak marks.

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fLastChanged (reference to float): time since peak mark was last
    changed in fractional seconds

    fPeakCurrent (float): current peak level mark in decibel

    fPeakOld (float): old peak level mark in decibel

    return value (float): new peak level mark in decibel
*/
{
   float fOutput;

   // prevent meter overshoot on overflows by limiting peak levels to
   // 0.0 dBFS
   if ( fPeakCurrent >= 0.0f ) {
      fPeakCurrent = 0.0f;
   }

   // apply rise time if peak level is above old level
   if ( fPeakCurrent >= fPeakOld ) {
      // if peak meter is set to "falling peaks" mode (non-negative
      // values), reset hold time (time that peaks are held before
      // starting to fall back down)
      if ( fLastChanged >= 0.0f ) {
         fLastChanged = 0.0f;
      }

      // immediate rise time, so set current peak level mark as new
      // peak level mark
      fOutput = fPeakCurrent;
      // otherwise, test whether to apply fall time
   } else {
      // if peak meter is set to "falling peaks" mode (non-negative
      // values), update hold time (time that peaks are held before
      // starting to fall back down)
      if ( fLastChanged >= 0.0f ) {
         fLastChanged += fTimePassed;
      }

      // peak meter is EITHER set to "infinite peak hold" mode
      // (negative values) OR the peak meter's hold time of 10
      // seconds has not yet been exceeded, so retain old peak level
      // mark
      if ( fLastChanged < 10.0f ) {
         fOutput = fPeakOld;
         // peak meter's hold time (10 seconds) has been reached or
         // exceeded
      } else {
         // fall time: 26 dB in 3 seconds (linear)
         float fReleaseCoef = 26.0f * fTimePassed / 3.0f;

         // apply fall time
         fOutput = fPeakOld - fReleaseCoef;
      }
   }

   // finally, return new peak level mark
   return fOutput;
}


void MeterBallistics::AverageMeterBallistics( int nChannel,
                                              float fTimePassed,
                                              float fAverageLevelCurrent )
/*  Calculate ballistics for average meter levels and update readout.

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fAverageLevelCurrent (float): current average meter level in
    decibel

    return value: none
*/
{
   // the meter reaches 99% of the final reading in 300 ms
   // (logarithmic)
   LogMeterBallistics( 0.300f, fTimePassed, fAverageLevelCurrent, arrAverageMeterLevels.getReference( nChannel ) );
}


float MeterBallistics::AverageMeterPeakBallistics( float fTimePassed,
                                                   float& fLastChanged,
                                                   float fPeakCurrent,
                                                   float fPeakOld )
/*  Calculate ballistics for average meter peak marks.

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fLastChanged (reference to float): time since peak mark was last
    changed in fractional seconds

    fPeakCurrent (float): current peak level mark in decibel

    fPeakOld (float): old peak level mark in decibel

    return value (float): new peak level mark in decibel
*/
{
   // the peak marks ballistics of peak meter and average meter are
   // identical, so let's reuse the peak meter code
   return PeakMeterPeakBallistics( fTimePassed, fLastChanged, fPeakCurrent, fPeakOld );
}


void MeterBallistics::SignalMeterBallistics( int nChannel,
                                             float fTimePassed,
                                             float fPeakMeterSignalCurrent )
/*  Calculate ballistics for signal meter and update readout.

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fPeakMeterSignalCurrent (float): current signal meter readout
    (between 0.0 and 1.0)

    return value: none
*/
{
   // the signal meter has a threshold of -60 dB FS; meter is rising
   if ( fPeakMeterSignalCurrent >= -60 ) {
      // immediate rise time, so set to full readout
      arrSignalMeterReadouts.set( nChannel, 1.0f );
      // meter is falling back to zero (i.e. fading out)
   } else {
      // shorten release phase
      if ( arrSignalMeterReadouts[nChannel] < 0.1f ) {
         arrSignalMeterReadouts.set( nChannel, 0.0f );
         // meter reaches 99% of the final reading in 1200 ms
      } else {
         LogMeterBallistics( 1.200f, fTimePassed, 0.0f, arrSignalMeterReadouts.getReference( nChannel ) );
      }
   }
}


void MeterBallistics::LogMeterBallistics( float fMeterInertia,
                                          float fTimePassed,
                                          float fLevel,
                                          float& fReadout )
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
   if ( fLevel != fReadout ) {
      // Thanks to Bram de Jong for the code snippet!
      // (http://www.musicdsp.org/showone.php?id=136)
      //
      // rise and fall: 99% of final reading in "fMeterInertia" seconds
      float fAttackReleaseCoef = powf( 0.01f, fTimePassed / fMeterInertia );
      fReadout = fAttackReleaseCoef * ( fReadout - fLevel ) + fLevel;
   }
}
