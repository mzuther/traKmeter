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

#pragma once

#include "FrutHeader.h"
#include "audio_file_player.h"
#include "meter_ballistics.h"
#include "plugin_parameters.h"


class TraKmeterAudioProcessor :
    public AudioProcessor,
    public ActionBroadcaster,
    virtual public frut::audio::RingBufferProcessor<float>
{
public:
    TraKmeterAudioProcessor();
    ~TraKmeterAudioProcessor();

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset() override;

    void processBlock(AudioBuffer<float> &buffer,
                      MidiBuffer &midiMessages) override;
    void processBlock(AudioBuffer<double> &buffer,
                      MidiBuffer &midiMessages) override;

    void silenceInput(bool isSilentNew);

    void startValidation(File fileAudio, int nSelectedChannel,
                         bool bReportCSV, bool bAverageMeterLevel,
                         bool bPeakMeterLevel);

    void stopValidation();
    bool isValidating();

    AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    int getNumParameters() override;
    const String getParameterName(int nIndex) override;
    const String getParameterText(int nIndex) override;

    float getParameter(int nIndex) override;
    void changeParameter(int nIndex, float fValue);
    void setParameter(int nIndex, float fValue) override;

    void clearChangeFlag(int nIndex);
    bool hasChanged(int nIndex);
    void updateParameters(bool bIncludeHiddenParameters);

    File getParameterValidationFile();
    void setParameterValidationFile(const File &fileValidation);

    String getParameterSkinName();
    void setParameterSkinName(const String &strSkinName);

    bool getBoolean(int nIndex);
    int getRealInteger(int nIndex);

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;

    double getTailLengthSeconds() const override;

    MeterBallistics *getLevels();

    virtual void processBufferChunk(const unsigned int uChunkSize,
                                    const unsigned int uBufferPosition,
                                    const unsigned int uProcessedSamples);

    bool getTransientMode();
    void setTransientMode(const bool transient_mode);

    int getCrestFactor();
    void setCrestFactor(const int crest_factor);

    int getNumPrograms() override;

    int getCurrentProgram() override;
    void setCurrentProgram(int nIndex) override;

    const String getProgramName(int nIndex) override;
    void changeProgramName(int nIndex, const String &newName) override;

    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

private:
    JUCE_LEAK_DETECTOR(TraKmeterAudioProcessor);

    static BusesProperties getBusesProperties();

    ScopedPointer<AudioFilePlayer> audioFilePlayer;
    ScopedPointer<frut::audio::RingBuffer<float>> pRingBufferInput;

    ScopedPointer<MeterBallistics> pMeterBallistics;

    TraKmeterPluginParameters pluginParameters;

    frut::dsp::Dither dither_;

    const int nTrakmeterBufferSize;

    bool bSampleRateIsValid;
    bool isSilent;

    int NumberOfChannels;
    int nSamplesInBuffer;
    float fProcessedSeconds;

    bool bTransientMode;
    int nCrestFactor;

    int countOverflows(frut::audio::RingBuffer<float> *ring_buffer,
                       const unsigned int channel,
                       const unsigned int length,
                       const unsigned int pre_delay);
};

AudioProcessor *JUCE_CALLTYPE createPluginFilter();


// Local Variables:
// ispell-local-dictionary: "british"
// End:
