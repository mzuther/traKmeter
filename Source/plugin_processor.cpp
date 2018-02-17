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

#include "plugin_processor.h"
#include "plugin_editor.h"


/*==============================================================================

Flow of parameter processing:

  Editor:      buttonClicked(button) / sliderValueChanged(slider)
  Processor:   changeParameter(nIndex, fValue)
  Processor:   setParameter(nIndex, fValue)
  Parameters:  setFloat(nIndex, fValue)
  Editor:      actionListenerCallback(strMessage)
  Editor:      updateParameter(nIndex)

==============================================================================*/

TraKmeterAudioProcessor::TraKmeterAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor(getBusesProperties()),
#endif
    trakmeterBufferSize_(1024)
{
    frut::Frut::printVersionNumbers();

#ifdef TRAKMETER_MULTI
    numberOfChannels_ = 8;
#else
    numberOfChannels_ = 2;
#endif

    sampleRateIsValid_ = false;
    processedSeconds_ = 0.0f;

    setLatencySamples(0);

    // depend on "TraKmeterPluginParameters"!
    transientMode_ = getBoolean(TraKmeterPluginParameters::selTransientMode);
    crestFactor_ = getRealInteger(TraKmeterPluginParameters::selCrestFactor);
}


TraKmeterAudioProcessor::~TraKmeterAudioProcessor()
{
    removeAllActionListeners();
}


AudioProcessor::BusesProperties TraKmeterAudioProcessor::getBusesProperties()
{
#ifdef TRAKMETER_MULTI

    return BusesProperties()
           .withInput("Main In",
                      AudioChannelSet::discreteChannels(8))
           .withOutput("Main Out",
                       AudioChannelSet::discreteChannels(8));

#else

    return BusesProperties()
           .withInput("Main In",
                      AudioChannelSet::stereo())
           .withOutput("Main Out",
                       AudioChannelSet::stereo());

#endif
}


#ifndef JucePlugin_PreferredChannelConfigurations
bool TraKmeterAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const
{
    // main bus: do not allow differing input and output layouts
    if (layouts.getMainInputChannelSet() != layouts.getMainOutputChannelSet())
    {
        return false;
    }

    // main bus: do not allow disabling of input channels
    if (layouts.getMainInputChannelSet().isDisabled())
    {
        return false;
    }

#ifdef TRAKMETER_MULTI

    // prefer main bus with eight input channels --> okay
    if (layouts.getMainInputChannelSet().size() == 8)
    {
        return true;
    }

    // main bus with one to seven input channels --> okay
    if ((layouts.getMainInputChannelSet().size() >= 1) &&
            (layouts.getMainInputChannelSet().size() <= 7))
    {
        return true;
    }

#else

    // main bus with mono input --> okay
    if (layouts.getMainInputChannelSet() == AudioChannelSet::mono())
    {
        return true;
    }

    // main bus with stereo input --> okay
    if (layouts.getMainInputChannelSet() == AudioChannelSet::stereo())
    {
        return true;
    }

#endif

    // current channel layout is not allowed
    return false;
}
#endif


const String TraKmeterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}


int TraKmeterAudioProcessor::getNumParameters()
{
    return pluginParameters_.getNumParameters(false);
}


const String TraKmeterAudioProcessor::getParameterName(
    int nIndex)
{
    return pluginParameters_.getName(nIndex);
}


const String TraKmeterAudioProcessor::getParameterText(
    int nIndex)
{
    return pluginParameters_.getText(nIndex);
}


float TraKmeterAudioProcessor::getParameter(
    int nIndex)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters_.getFloat(nIndex);
}


void TraKmeterAudioProcessor::changeParameter(
    int nIndex,
    float fValue)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    // notify host of parameter change (this will automatically call
    // "setParameter"!)
    beginParameterChangeGesture(nIndex);
    setParameterNotifyingHost(nIndex, fValue);
    endParameterChangeGesture(nIndex);
}


void TraKmeterAudioProcessor::setParameter(
    int nIndex,
    float fValue)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    // Please only call this method directly for non-automatable
    // values!

    pluginParameters_.setFloat(nIndex, fValue);

    if (nIndex == TraKmeterPluginParameters::selTransientMode)
    {
        setTransientMode(getBoolean(nIndex));
    }
    else if (nIndex == TraKmeterPluginParameters::selCrestFactor)
    {
        setCrestFactor(getRealInteger(nIndex));
    }

    // notify plug-in editor of parameter change
    if (pluginParameters_.hasChanged(nIndex))
    {
        // for visible parameters, notify the editor of changes (this
        // will also clear the change flag)
        if (nIndex < pluginParameters_.getNumParameters(false))
        {
            // "PC" --> parameter changed, followed by a hash and the
            // parameter's ID
            sendActionMessage("PC#" + String(nIndex));
        }
        // for hidden parameters, we only have to clear the change
        // flag
        else
        {
            pluginParameters_.clearChangeFlag(nIndex);
        }
    }
}


void TraKmeterAudioProcessor::clearChangeFlag(
    int nIndex)
{
    pluginParameters_.clearChangeFlag(nIndex);
}


bool TraKmeterAudioProcessor::hasChanged(
    int nIndex)
{
    return pluginParameters_.hasChanged(nIndex);
}


void TraKmeterAudioProcessor::updateParameters(
    bool bIncludeHiddenParameters)
{
    int nNumParameters = pluginParameters_.getNumParameters(false);

    for (int nIndex = 0; nIndex < nNumParameters; ++nIndex)
    {
        if (pluginParameters_.hasChanged(nIndex))
        {
            float fValue = pluginParameters_.getFloat(nIndex);
            changeParameter(nIndex, fValue);
        }
    }

    if (bIncludeHiddenParameters)
    {
        // handle hidden parameters here!

        // the following parameters need no updating:
        //
        // * selValidationFileName
        // * selValidationSelectedChannel
        // * selValidationAverageMeterLevel
        // * selValidationPeakMeterLevel
        // * selValidationCSVFormat
        // * selSkinName
    }
}


bool TraKmeterAudioProcessor::getBoolean(
    int nIndex)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters_.getBoolean(nIndex);
}


int TraKmeterAudioProcessor::getRealInteger(
    int nIndex)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters_.getRealInteger(nIndex);
}


File TraKmeterAudioProcessor::getParameterValidationFile()
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters_.getValidationFile();
}


void TraKmeterAudioProcessor::setParameterValidationFile(
    const File &fileValidation)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    pluginParameters_.setValidationFile(fileValidation);
}


String TraKmeterAudioProcessor::getParameterSkinName()
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters_.getSkinName();
}


void TraKmeterAudioProcessor::setParameterSkinName(
    const String &strSkinName)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    pluginParameters_.setSkinName(strSkinName);
}


bool TraKmeterAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}


bool TraKmeterAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}


double TraKmeterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}


int TraKmeterAudioProcessor::getNumPrograms()
{
    return 0;
}


int TraKmeterAudioProcessor::getCurrentProgram()
{
    return 0;
}


void TraKmeterAudioProcessor::setCurrentProgram(
    int nIndex)
{
    ignoreUnused(nIndex);
}


const String TraKmeterAudioProcessor::getProgramName(
    int nIndex)
{
    ignoreUnused(nIndex);

    return String::empty;
}


void TraKmeterAudioProcessor::changeProgramName(
    int nIndex,
    const String &newName)
{
    ignoreUnused(nIndex, newName);
}


void TraKmeterAudioProcessor::prepareToPlay(
    double sampleRate,
    int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    Logger::outputDebugString("[traKmeter] preparing to play");

    if ((sampleRate < 44100) || (sampleRate > 192000))
    {
        Logger::outputDebugString("[traKmeter] WARNING: sample rate of " +
                                  String(sampleRate) + " Hz not supported");
        sampleRateIsValid_ = false;
        return;
    }
    else
    {
        sampleRateIsValid_ = true;
    }

    isSilent = false;

    Logger::outputDebugString("[traKmeter] number of input channels: " +
                              String(getMainBusNumInputChannels()));
    Logger::outputDebugString("[traKmeter] number of output channels: " +
                              String(getMainBusNumOutputChannels()));

    dither_.initialise(jmax(getMainBusNumInputChannels(),
                            getMainBusNumOutputChannels()),
                       24);

    meterBallistics_ = new MeterBallistics(numberOfChannels_,
                                           crestFactor_,
                                           true,
                                           false,
                                           transientMode_);

    samplesInBuffer_ = 0;

    // make sure that ring buffer can hold at least
    // trakmeterBufferSize_ samples and is large enough to receive a
    // full block of audio
    int ringBufferSize = jmax(samplesPerBlock, trakmeterBufferSize_);

    int preDelay = trakmeterBufferSize_;
    int chunkSize = trakmeterBufferSize_;

    ringBufferInput_ = new frut::audio::RingBuffer<float>(
        numberOfChannels_,
        ringBufferSize,
        preDelay,
        chunkSize);

    ringBufferInput_->setCallbackClass(this);
}


void TraKmeterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free
    // up any spare memory, etc.

    Logger::outputDebugString("[traKmeter] releasing resources");
    Logger::outputDebugString("");

    meterBallistics_ = nullptr;
    audioFilePlayer_ = nullptr;
}


void TraKmeterAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers,
    // etc, as it means there's been a break in the audio's
    // continuity.
}


void TraKmeterAudioProcessor::processBlock(
    AudioBuffer<float> &buffer,
    MidiBuffer &midiMessages)
{
    jassert(!isUsingDoublePrecision());
    ignoreUnused(midiMessages);

    // This is the place where you'd normally do the guts of your
    // plug-in's audio processing...

    int NumberOfSamples = buffer.getNumSamples();

    if (!sampleRateIsValid_)
    {
        buffer.clear();
        return;
    }

    if (getMainBusNumInputChannels() < 1)
    {
        Logger::outputDebugString("[traKmeter] no input channels!");
        return;
    }

    // In case we have more outputs than inputs, we'll clear any
    // output channels that didn't contain input data, because these
    // aren't guaranteed to be empty -- they may contain garbage.

    for (int nChannel = getMainBusNumInputChannels(); nChannel < getMainBusNumOutputChannels(); ++nChannel)
    {
        buffer.clear(nChannel, 0, NumberOfSamples);
    }

    if (audioFilePlayer_)
    {
        audioFilePlayer_->fillBufferChunk(buffer);
    }
    // silence input if validation window is open
    else if (isSilent)
    {
        buffer.clear();
    }

    // process input samples
    ringBufferInput_->addSamples(buffer, 0, NumberOfSamples);

    samplesInBuffer_ += NumberOfSamples;
    samplesInBuffer_ %= trakmeterBufferSize_;
}


void TraKmeterAudioProcessor::processBlock(
    AudioBuffer<double> &buffer,
    MidiBuffer &midiMessages)
{
    jassert(isUsingDoublePrecision());
    ignoreUnused(midiMessages);

    // This is the place where you'd normally do the guts of your
    // plug-in's audio processing...

    int NumberOfSamples = buffer.getNumSamples();

    if (!sampleRateIsValid_)
    {
        for (int nChannel = 0; nChannel < getMainBusNumOutputChannels(); ++nChannel)
        {
            buffer.clear(nChannel, 0, NumberOfSamples);
        }

        return;
    }

    if (getMainBusNumInputChannels() < 1)
    {
        Logger::outputDebugString("[traKmeter] no input channels!");
        return;
    }

    // In case we have more outputs than inputs, we'll clear any
    // output channels that didn't contain input data, because these
    // aren't guaranteed to be empty -- they may contain garbage.

    for (int nChannel = getMainBusNumInputChannels(); nChannel < getMainBusNumOutputChannels(); ++nChannel)
    {
        buffer.clear(nChannel, 0, NumberOfSamples);
    }

    if (audioFilePlayer_)
    {
        audioFilePlayer_->fillBufferChunk(buffer);
    }
    // silence input if validation window is open
    else if (isSilent)
    {
        buffer.clear();
    }

    // the process buffer is not going to be heard, so truncate
    // samples!
    AudioBuffer<float> processBuffer(numberOfChannels_, NumberOfSamples);
    dither_.truncateToFloat(buffer, processBuffer);

    // process input samples
    ringBufferInput_->addSamples(processBuffer, 0, NumberOfSamples);

    samplesInBuffer_ += NumberOfSamples;
    samplesInBuffer_ %= trakmeterBufferSize_;
}


void TraKmeterAudioProcessor::processBufferChunk(
    frut::audio::RingBuffer<float> *ringBuffer,
    const int chunkSize)
{
    ignoreUnused(ringBuffer);

    // length of buffer chunk in fractional seconds
    // (1024 samples / 44100 samples/s = 23.2 ms)
    processedSeconds_ = (float) chunkSize / (float) getSampleRate();

    for (int nChannel = 0; nChannel < getMainBusNumInputChannels(); ++nChannel)
    {
        // determine peak level for chunkSize samples
        float fPeakLevels = ringBufferInput_->getMagnitude(
                                nChannel, chunkSize);

        // determine peak level for chunkSize samples
        float fRmsLevels = ringBufferInput_->getRMSLevel(
                               nChannel, chunkSize);

        // apply meter ballistics and store values so that the editor
        // can access them
        meterBallistics_->updateChannel(
            nChannel, processedSeconds_, fPeakLevels, fRmsLevels);

        // "UM" --> update meters
        sendActionMessage("UM");
    }
}


void TraKmeterAudioProcessor::silenceInput(
    bool isSilentNew)
{
    isSilent = isSilentNew;
}


void TraKmeterAudioProcessor::startValidation(
    File fileAudio,
    int nSelectedChannel,
    bool bReportCSV,
    bool bAverageMeterLevel,
    bool bPeakMeterLevel)
{
    // reset all meters before we start the validation
    meterBallistics_->reset();

    isSilent = false;

    audioFilePlayer_ = new AudioFilePlayer(fileAudio,
                                           (int) getSampleRate(),
                                           meterBallistics_,
                                           crestFactor_);

    if (audioFilePlayer_->matchingSampleRates())
    {
        audioFilePlayer_->setReporters(nSelectedChannel, bReportCSV,
                                       bAverageMeterLevel, bPeakMeterLevel);

        // refresh editor; "V+" --> validation started
        sendActionMessage("V+");
    }
    else
    {
        stopValidation();

        AlertWindow::showMessageBoxAsync(
            AlertWindow::WarningIcon,
            "Validation error",
            "Sample rates of host and validation file do not match.");
    }

}


void TraKmeterAudioProcessor::stopValidation()
{
    isSilent = false;
    audioFilePlayer_ = nullptr;

    // reset all meters after the validation
    meterBallistics_->reset();

    // refresh editor; "V-" --> validation stopped
    sendActionMessage("V-");
}


bool TraKmeterAudioProcessor::isValidating()
{
    if (!audioFilePlayer_)
    {
        return false;
    }
    else
    {
        if (audioFilePlayer_->isPlaying())
        {
            return true;
        }
        else
        {
            stopValidation();
            return false;
        }
    }
}


MeterBallistics *TraKmeterAudioProcessor::getLevels()
{
    return meterBallistics_;
}


bool TraKmeterAudioProcessor::getTransientMode()
{
    return transientMode_;
}


void TraKmeterAudioProcessor::setTransientMode(
    const bool transientMode)
{
    if (transientMode != transientMode_)
    {
        transientMode_ = transientMode;

        if (meterBallistics_)
        {
            meterBallistics_ = new MeterBallistics(numberOfChannels_,
                                                   crestFactor_,
                                                   true,
                                                   false,
                                                   transientMode_);
        }
    }
}


int TraKmeterAudioProcessor::getCrestFactor()
{
    return crestFactor_;
}


void TraKmeterAudioProcessor::setCrestFactor(
    const int crestFactor)
{
    if (crestFactor != crestFactor_)
    {
        crestFactor_ = crestFactor;

        if (meterBallistics_)
        {
            meterBallistics_->setCrestFactor(crestFactor_);
        }

        if (audioFilePlayer_)
        {
            audioFilePlayer_->setCrestFactor(crestFactor_);
        }
    }
}


AudioProcessorEditor *TraKmeterAudioProcessor::createEditor()
{
    //  meter ballistics are not updated when the editor is closed, so
    //  reset them here
    if (meterBallistics_)
    {
        meterBallistics_->reset();
    }

    return new TraKmeterAudioProcessorEditor(
               this, numberOfChannels_, crestFactor_);
}


bool TraKmeterAudioProcessor::hasEditor() const
{
    return true;
}


void TraKmeterAudioProcessor::getStateInformation(
    MemoryBlock &destData)
{
    XmlElement xmlParameters = pluginParameters_.storeAsXml();

    DBG("[traKmeter]");
    DBG("[traKmeter] storing plug-in parameters:");
    DBG("[traKmeter]");
    DBG(String("[traKmeter]   ") + xmlParameters.createDocument("").replace(
            "\n", "\n[traKmeter]   "));

    copyXmlToBinary(xmlParameters, destData);
}


void TraKmeterAudioProcessor::setStateInformation(
    const void *data,
    int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlParameters(
        getXmlFromBinary(data, sizeInBytes));

    DBG("[traKmeter] loading plug-in parameters:");
    DBG("[traKmeter]");
    DBG(String("[traKmeter]   ") + xmlParameters->createDocument("").replace(
            "\n", "\n[traKmeter]   "));

    pluginParameters_.loadFromXml(xmlParameters);
    updateParameters(true);
}


// This creates new instances of the plug-in.
AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new TraKmeterAudioProcessor();
}
