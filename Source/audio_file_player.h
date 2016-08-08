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

#ifndef __AUDIO_FILE_PLAYER__
#define __AUDIO_FILE_PLAYER__

#include "FrutHeader.h"
#include "meter_ballistics.h"


class AudioFilePlayer
{
public:
    AudioFilePlayer(const File audioFile, int sample_rate, MeterBallistics *meter_ballistics, int crest_factor);
    ~AudioFilePlayer();

    bool isPlaying();
    bool matchingSampleRates();

    void fillBufferChunk(AudioBuffer<float> *buffer);
    void setCrestFactor(int crest_factor);
    void setReporters(int nChannel, bool ReportCSV, bool bAverageMeterLevel, bool bPeakMeterLevel);

private:
    JUCE_LEAK_DETECTOR(AudioFilePlayer);

    bool bIsPlaying;
    bool bSampleRatesMatch;
    bool bHeaderIsWritten;

    int nSamplesMovingAverage;
    int64 nNumberOfSamples;
    float fSampleRate;
    float fCrestFactor;
    float fMeterMinimumDecibel;

    int nNumberOfChannels;
    int nReportChannel;
    bool bReports;
    bool bReportCSV;
    bool bReportAverageMeterLevel;
    bool bReportPeakMeterLevel;

    Array<frut::math::Averager> arrAverager_AverageMeterLevels;
    Array<frut::math::Averager> arrAverager_PeakMeterLevels;

    ScopedPointer<AudioFormatReaderSource> audioFileSource;
    MeterBallistics *pMeterBallistics;

    void outputReportPlain(void);
    void outputReportCSVHeader(void);
    void outputReportCSVLine(void);

    String formatTime(void);
    String formatValue(const float fValue);

    void outputValue(const float fValue, frut::math::Averager &averager, const float fCorrectionFactor, const String &strPrefix, const String &strSuffix);
    void outputMessage(const String &strMessage);
};

#endif   // __AUDIO_FILE_PLAYER__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
