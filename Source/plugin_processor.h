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

#ifndef TRAKMETER_PLUGIN_PROCESSOR_H
#define TRAKMETER_PLUGIN_PROCESSOR_H

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

    std::shared_ptr<MeterBallistics> getLevels();
    void resetMeters();

    virtual bool processBufferChunk(AudioBuffer<float> &buffer) override;

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
    void resetOnPlay();

    std::unique_ptr<AudioFilePlayer> audioFilePlayer_;
    std::unique_ptr<frut::audio::RingBuffer<float>> ringBuffer_;

    std::shared_ptr<MeterBallistics> meterBallistics_;

    TraKmeterPluginParameters pluginParameters_;

    frut::dsp::Dither dither_;

    const int trakmeterBufferSize_;

    bool sampleRateIsValid_;
    bool isSilent_;
    bool hasStopped_;

    int numberOfChannels_;
    float processedSeconds_;
};

AudioProcessor *JUCE_CALLTYPE createPluginFilter();

#endif  // TRAKMETER_PLUGIN_PROCESSOR_H
