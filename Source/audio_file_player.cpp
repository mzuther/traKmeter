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

#include "audio_file_player.h"


AudioFilePlayer::AudioFilePlayer(const File audioFile, int sample_rate, MeterBallistics *meter_ballistics, int crest_factor)
{
    nReportChannel = -1;
    bReports = false;
    bReportCSV = false;
    bReportAverageMeterLevel = false;
    bReportPeakMeterLevel = false;

    bHeaderIsWritten = false;
    setCrestFactor(crest_factor);

    pMeterBallistics = meter_ballistics;

    // try "300" for uncorrelated band-limited pink noise
    nSamplesMovingAverage = 50;
    nNumberOfChannels = pMeterBallistics->getNumberOfChannels();

    for (int nChannel = 0; nChannel < nNumberOfChannels; nChannel++)
    {
        arrAverager_AverageMeterLevels.add(Averager(nSamplesMovingAverage, fMeterMinimumDecibel));
        arrAverager_PeakMeterLevels.add(Averager(nSamplesMovingAverage, fMeterMinimumDecibel));
    }

    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    // will be deleted by "audioFileSource"
    AudioFormatReader *formatReader = formatManager.createReaderFor(audioFile);

    if (formatReader)
    {
        audioFileSource = new AudioFormatReaderSource(formatReader, true);
        bIsPlaying = true;

        nNumberOfSamples = audioFileSource->getTotalLength();
        // pause for ten seconds after playback
        nNumberOfSamples += 10 * sample_rate;

        outputMessage("Audio file: \"" + audioFile.getFullPathName() + "\"");
        outputMessage(String(formatReader->numChannels) + " channel(s), " + String(formatReader->sampleRate) + " Hz, " + String(formatReader->bitsPerSample) + " bit");

        fSampleRate = (float) formatReader->sampleRate;

        if (formatReader->sampleRate != sample_rate)
        {
            outputMessage(String::empty);
            outputMessage("WARNING: sample rate mismatch (host: " + String(sample_rate) + " Hz)!");
            outputMessage(String::empty);
        }

        outputMessage(String::empty);
        outputMessage("Starting validation ...");
        outputMessage(String::empty);
    }
    else
    {
        bIsPlaying = false;
        bReports = false;
    }
}


AudioFilePlayer::~AudioFilePlayer()
{
    if (isPlaying())
    {
        outputMessage("Stopping validation ...");
    }
}


void AudioFilePlayer::setCrestFactor(int crest_factor)
{
    fCrestFactor = float(crest_factor);
    fMeterMinimumDecibel = MeterBallistics::getMeterMinimumDecibel() + fCrestFactor;
}


void AudioFilePlayer::setReporters(int nChannel, bool ReportCSV, bool bAverageMeterLevel, bool bPeakMeterLevel)
{
    bReportCSV = ReportCSV;

    nReportChannel = nChannel;
    bReportAverageMeterLevel = bAverageMeterLevel;
    bReportPeakMeterLevel = bPeakMeterLevel;

    bReports = bReportAverageMeterLevel || bReportPeakMeterLevel;
}


bool AudioFilePlayer::isPlaying()
{
    if (bIsPlaying)
    {
        if (audioFileSource->getNextReadPosition() < nNumberOfSamples)
        {
            return true;
        }
        else
        {
            outputMessage("Stopping validation ...");

            bIsPlaying = false;
            bReports = false;
            return false;
        }
    }
    else
    {
        return false;
    }
}


void AudioFilePlayer::fillBufferChunk(AudioSampleBuffer *buffer)
{
    // report old meter readings
    if (bReports)
    {
        if (bReportCSV)
        {
            outputReportCSVLine();
        }
        else
        {
            outputReportPlain();
        }
    }

    if (isPlaying())
    {
        AudioSourceChannelInfo channelInfo;
        channelInfo.buffer = buffer;
        channelInfo.startSample = 0;
        channelInfo.numSamples = buffer->getNumSamples();

        channelInfo.clearActiveBufferRegion();
        audioFileSource->getNextAudioBlock(channelInfo);
    }
}


void AudioFilePlayer::outputReportPlain(void)
{
    if (bReportAverageMeterLevel)
    {
        if (nReportChannel < 0)
        {
            for (int nChannel = 0; nChannel < nNumberOfChannels; nChannel++)
            {
                float fAverageMeterLevel = pMeterBallistics->getAverageMeterLevel(nChannel);
                String strPrefix = "average level (ch. " + String(nChannel + 1) + "):  ";
                String strSuffix = " dB";
                outputValue(fAverageMeterLevel, arrAverager_AverageMeterLevels.getReference(nChannel), fCrestFactor, strPrefix, strSuffix);
            }
        }
        else
        {
            float fAverageMeterLevel = pMeterBallistics->getAverageMeterLevel(nReportChannel);
            String strPrefix = "average level (ch. " + String(nReportChannel + 1) + "):  ";
            String strSuffix = " dB";
            outputValue(fAverageMeterLevel, arrAverager_AverageMeterLevels.getReference(nReportChannel), fCrestFactor, strPrefix, strSuffix);
        }
    }

    if (bReportPeakMeterLevel)
    {
        if (nReportChannel < 0)
        {
            for (int nChannel = 0; nChannel < nNumberOfChannels; nChannel++)
            {
                float fPeakMeterLevel = pMeterBallistics->getPeakMeterLevel(nChannel);
                String strPrefix = "peak level (ch. " + String(nChannel + 1) + "):     ";
                String strSuffix = " dB";
                outputValue(fPeakMeterLevel, arrAverager_PeakMeterLevels.getReference(nChannel), fCrestFactor, strPrefix, strSuffix);
            }
        }
        else
        {
            float fPeakMeterLevel = pMeterBallistics->getPeakMeterLevel(nReportChannel);
            String strPrefix = "peak level (ch. " + String(nReportChannel + 1) + "):     ";
            String strSuffix = " dB";
            outputValue(fPeakMeterLevel, arrAverager_PeakMeterLevels.getReference(nReportChannel), fCrestFactor, strPrefix, strSuffix);
        }
    }

    outputMessage(String::empty);
}


void AudioFilePlayer::outputReportCSVHeader(void)
{
    bHeaderIsWritten = true;
    String strOutput = "\"timecode\"\t";

    if (bReportAverageMeterLevel)
    {
        if (nReportChannel < 0)
        {
            for (int nChannel = 0; nChannel < nNumberOfChannels; nChannel++)
            {
                strOutput += "\"avg_" + String(nChannel + 1) + "\"\t";
            }
        }
        else
        {
            strOutput += "\"avg_" + String(nReportChannel + 1) + "\"\t";
        }
    }

    if (bReportPeakMeterLevel)
    {
        if (nReportChannel < 0)
        {
            for (int nChannel = 0; nChannel < nNumberOfChannels; nChannel++)
            {
                strOutput += "\"pk_" + String(nChannel + 1) + "\"\t";
            }
        }
        else
        {
            strOutput += "\"pk_" + String(nReportChannel + 1) + "\"\t";
        }
    }

    Logger::outputDebugString(strOutput);
}


void AudioFilePlayer::outputReportCSVLine(void)
{
    String strOutput;

    if (!bHeaderIsWritten)
    {
        outputReportCSVHeader();
    }

    if (bReportAverageMeterLevel)
    {
        if (nReportChannel < 0)
        {
            for (int nChannel = 0; nChannel < nNumberOfChannels; nChannel++)
            {
                float fAverageMeterLevel = pMeterBallistics->getAverageMeterLevel(nChannel);
                strOutput += formatValue(fAverageMeterLevel);
            }
        }
        else
        {
            float fAverageMeterLevel = pMeterBallistics->getAverageMeterLevel(nReportChannel);
            strOutput += formatValue(fAverageMeterLevel);
        }
    }

    if (bReportPeakMeterLevel)
    {
        if (nReportChannel < 0)
        {
            for (int nChannel = 0; nChannel < nNumberOfChannels; nChannel++)
            {
                float fPeakMeterLevel = pMeterBallistics->getPeakMeterLevel(nChannel);
                strOutput += formatValue(fPeakMeterLevel);
            }
        }
        else
        {
            float fPeakMeterLevel = pMeterBallistics->getPeakMeterLevel(nReportChannel);
            strOutput += formatValue(fPeakMeterLevel);
        }
    }

    Logger::outputDebugString("\"" + formatTime() + "\"\t" + strOutput);
}


String AudioFilePlayer::formatTime(void)
{
    float fTime = audioFileSource->getNextReadPosition() / fSampleRate;

    // check for NaN
    if (fTime != fTime)
    {
        fTime = 0.0f;
    }

    int nTime = int(fTime);
    int nMilliSeconds = int(1000.0f * (fTime - nTime) + 0.5f);

    String strMinutes = String(nTime / 60).paddedLeft('0', 2);
    String strSeconds = String(nTime % 60).paddedLeft('0', 2);
    String strMilliSeconds = String(nMilliSeconds).paddedLeft('0', 3);

    return strMinutes + ":" + strSeconds + "." + strMilliSeconds;
}


String AudioFilePlayer::formatValue(const float fValue)
{
    String strValue;

    if (fValue < 0.0f)
    {
        strValue = String(fValue, 2);
    }
    else
    {
        strValue = "+" + String(fValue, 2);
    }

    return (strValue + "\t");
}


void AudioFilePlayer::outputValue(const float fValue, Averager &averager, const float fCorrectionFactor, const String &strPrefix, const String &strSuffix)
{
    String strValue;

    if (fValue < 0.0f)
    {
        strValue = String(fValue, 2) + strSuffix;
    }
    else
    {
        strValue = "+" + String(fValue, 2) + strSuffix;
    }

    String strSimpleMovingAverage;

    averager.addSample(fValue - fCorrectionFactor);

    if (averager.isValid())
    {
        float fSimpleMovingAverage = averager.getSimpleMovingAverage() + fCorrectionFactor;

        if (fSimpleMovingAverage < 0.0f)
        {
            strSimpleMovingAverage = "   SMA(" + String(nSamplesMovingAverage) + "): " + String(fSimpleMovingAverage, 2) + strSuffix;
        }
        else
        {
            strSimpleMovingAverage = "   SMA(" + String(nSamplesMovingAverage) + "): +" + String(fSimpleMovingAverage, 2) + strSuffix;
        }
    }

    outputMessage(strPrefix + strValue + strSimpleMovingAverage);
}


void AudioFilePlayer::outputMessage(const String &strMessage)
{

    Logger::outputDebugString("[Validation - " + formatTime() + "] " + strMessage);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
