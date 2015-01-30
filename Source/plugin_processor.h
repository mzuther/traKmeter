/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2014 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __TRAKMETER_PLUGINPROCESSOR_H__
#define __TRAKMETER_PLUGINPROCESSOR_H__

#define TRAKMETER_BUFFER_SIZE 1024

class TraKmeterAudioProcessor;
class MeterBallistics;

#include "../JuceLibraryCode/JuceHeader.h"
#include "audio_file_player.h"
#include "audio_ring_buffer.h"
#include "dither.h"
#include "meter_ballistics.h"
#include "plugin_parameters.h"


//============================================================================
class TraKmeterAudioProcessor  : public AudioProcessor, public ActionBroadcaster
{
public:
    //==========================================================================

    TraKmeterAudioProcessor();
    ~TraKmeterAudioProcessor();

    void addActionListenerParameters(ActionListener *listener) throw();
    void removeActionListenerParameters(ActionListener *listener) throw();

    //==========================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages);

    void startValidation(File fileAudio, int nSelectedChannel, bool bReportCSV, bool bAverageMeterLevel, bool bPeakMeterLevel);
    void stopValidation();
    bool isValidating();

    //==========================================================================
    AudioProcessorEditor *createEditor();
    bool hasEditor() const;

    //==========================================================================
    int getNumParameters();

    float getParameter(int index);
    bool getParameterAsBool(int nIndex);

    void setParameter(int index, float newValue);
    void updateParameters(bool bIncludeHiddenParameters);

    File getParameterValidationFile();
    void setParameterValidationFile(File &fileValidation);

    const String getParameterName(int index);
    const String getParameterText(int index);

    void changeParameter(int index, float fValue);
    int getParameterAsInt(int index);

    void MarkParameter(int nIndex);
    void UnmarkParameter(int nIndex);
    bool isParameterMarked(int nIndex);

    //==========================================================================
    const String getName() const;

    const String getInputChannelName(int channelIndex) const;
    const String getOutputChannelName(int channelIndex) const;
    bool isInputChannelStereoPair(int index) const;
    bool isOutputChannelStereoPair(int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;

    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;

    MeterBallistics *getLevels();
    void processBufferChunk(AudioSampleBuffer &buffer, const unsigned int uChunkSize, const unsigned int uBufferPosition, const unsigned int uProcessedSamples);

    bool getTransientMode();
    void setTransientMode(const bool transient_mode);

    int getCrestFactor();
    void setCrestFactor(const int crest_factor);

    //==========================================================================
    int getNumPrograms();
    int getNumChannels();
    int getCurrentProgram();
    void setCurrentProgram(int index);
    const String getProgramName(int index);
    void changeProgramName(int index, const String &newName);

    //==========================================================================
    void getStateInformation(MemoryBlock &destData);
    void setStateInformation(const void *data, int sizeInBytes);

    //==========================================================================
    juce_UseDebuggingNewOperator

private:
    JUCE_LEAK_DETECTOR(TraKmeterAudioProcessor);

    AudioFilePlayer *audioFilePlayer;
    AudioRingBuffer *pRingBufferInput;

    MeterBallistics *pMeterBallistics;
    TraKmeterPluginParameters *pPluginParameters;

    Dither *pDither;

    int nNumInputChannels;
    bool bSampleRateIsValid;

    int nSamplesInBuffer;
    float fProcessedSeconds;

    bool bTransientMode;
    int nCrestFactor;
    float *fPeakLevels;
    float *fRmsLevels;
    int nDecibels;
    double dGain;

    int *nOverflows;

    int countOverflows(AudioRingBuffer *ring_buffer, const unsigned int channel, const unsigned int length, const unsigned int pre_delay);
};

AudioProcessor *JUCE_CALLTYPE createPluginFilter();

#endif  // __TRAKMETER_PLUGINPROCESSOR_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
