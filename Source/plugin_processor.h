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

#ifndef __TRAKMETER_PLUGINPROCESSOR_H__
#define __TRAKMETER_PLUGINPROCESSOR_H__

#include "FrutHeader.h"
#include "audio_file_player.h"
#include "meter_ballistics.h"
#include "plugin_parameters.h"


//============================================================================
class TraKmeterAudioProcessor : public AudioProcessor, public ActionBroadcaster, virtual public frut::audio::RingBufferProcessor
{
public:
    //==========================================================================

    TraKmeterAudioProcessor();
    ~TraKmeterAudioProcessor();

    //==========================================================================
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages);

    void silenceInput(bool isSilentNew);
    void startValidation(File fileAudio, int nSelectedChannel, bool bReportCSV, bool bAverageMeterLevel, bool bPeakMeterLevel);
    void stopValidation();
    bool isValidating();

    //==========================================================================
    AudioProcessorEditor *createEditor();
    bool hasEditor() const;

    //==========================================================================
    int getNumParameters();
    const String getParameterName(int nIndex);
    const String getParameterText(int nIndex);

    float getParameter(int nIndex);
    void changeParameter(int nIndex, float fValue);
    void setParameter(int nIndex, float fValue);

    void clearChangeFlag(int nIndex);
    bool hasChanged(int nIndex);
    void updateParameters(bool bIncludeHiddenParameters);

    File getParameterValidationFile();
    void setParameterValidationFile(const File &fileValidation);

    String getParameterSkinName();
    void setParameterSkinName(const String &strSkinName);

    bool getBoolean(int nIndex);
    int getRealInteger(int nIndex);

    //==========================================================================
    const String getName() const;

    bool acceptsMidi() const;
    bool producesMidi() const;

    double getTailLengthSeconds() const;

    MeterBallistics *getLevels();
    virtual void processBufferChunk(AudioBuffer<float> &buffer, const unsigned int uChunkSize, const unsigned int uBufferPosition, const unsigned int uProcessedSamples);

    bool getTransientMode();
    void setTransientMode(const bool transient_mode);

    int getCrestFactor();
    void setCrestFactor(const int crest_factor);

    //==========================================================================
    int getNumPrograms();

    int getCurrentProgram();
    void setCurrentProgram(int nIndex);

    const String getProgramName(int nIndex);
    void changeProgramName(int nIndex, const String &newName);

    //==========================================================================
    void getStateInformation(MemoryBlock &destData);
    void setStateInformation(const void *data, int sizeInBytes);

    //==========================================================================

private:
    JUCE_LEAK_DETECTOR(TraKmeterAudioProcessor);

    ScopedPointer<AudioFilePlayer> audioFilePlayer;
    ScopedPointer<frut::audio::RingBuffer> pRingBufferInput;

    ScopedPointer<MeterBallistics> pMeterBallistics;

    const int nTrakmeterBufferSize;

    TraKmeterPluginParameters pluginParameters;
    frut::dsp::Dither dither;

    bool bSampleRateIsValid;
    bool isSilent;

    int NumberOfChannels;
    int nSamplesInBuffer;
    float fProcessedSeconds;

    bool bTransientMode;
    int nCrestFactor;
    int nDecibels;
    double dGain;

    int countOverflows(frut::audio::RingBuffer *ring_buffer, const unsigned int channel, const unsigned int length, const unsigned int pre_delay);
};

AudioProcessor *JUCE_CALLTYPE createPluginFilter();

#endif  // __TRAKMETER_PLUGINPROCESSOR_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
