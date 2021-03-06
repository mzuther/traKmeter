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

#ifndef TRAKMETER_AUDIO_FILE_PLAYER_H
#define TRAKMETER_AUDIO_FILE_PLAYER_H

#include "FrutHeader.h"
#include "meter_ballistics.h"


class AudioFilePlayer
{
public:
    AudioFilePlayer(const File audioFile,
                    int sample_rate,
                    std::shared_ptr<MeterBallistics> meter_ballistics);

    ~AudioFilePlayer();

    bool isPlaying();
    bool matchingSampleRates();

    void copyTo(AudioBuffer<float> &buffer);

    void setReporters(int nChannel, bool ReportCSV,
                      bool bAverageMeterLevel, bool bPeakMeterLevel);

private:
    JUCE_LEAK_DETECTOR(AudioFilePlayer);

    bool bIsPlaying;
    bool bSampleRatesMatch;
    bool bHeaderIsWritten;

    int nSamplesMovingAverage;
    int64 nNumberOfSamples;
    float fSampleRate;

    int nNumberOfChannels;
    int nReportChannel;
    bool bReports;
    bool bReportCSV;
    bool bReportAverageMeterLevel;
    bool bReportPeakMeterLevel;

    Array<frut::math::Averager> arrAverager_AverageMeterLevels;
    Array<frut::math::Averager> arrAverager_PeakMeterLevels;

    frut::dsp::Dither dither_;

    std::unique_ptr<AudioFormatReaderSource> audioFileSource;
    std::shared_ptr<MeterBallistics> pMeterBallistics;

    void outputReportPlain(void);
    void outputReportCSVHeader(void);
    void outputReportCSVLine(void);

    String formatTime(void);
    String formatValue(const float fValue);

    void outputValue(const float fValue,
                     frut::math::Averager &averager,
                     const String &strPrefix,
                     const String &strSuffix);

    void outputMessage(const String &strMessage);
};

#endif  // TRAKMETER_AUDIO_FILE_PLAYER_H
