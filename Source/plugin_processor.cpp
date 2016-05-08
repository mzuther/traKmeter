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
    nTrakmeterBufferSize(1024),
    dither(24)
{
    frut::Frut::printVersionNumbers();

    bSampleRateIsValid = false;
    nNumInputChannels = 0;
    fProcessedSeconds = 0.0f;

    setLatencySamples(0);

    // depend on "TraKmeterPluginParameters"!
    bTransientMode = getBoolean(TraKmeterPluginParameters::selTransientMode);
    nCrestFactor = getRealInteger(TraKmeterPluginParameters::selCrestFactor);

    nDecibels = getRealInteger(TraKmeterPluginParameters::selGain);
    dGain = MeterBallistics::decibel2level_double(nDecibels);
}


TraKmeterAudioProcessor::~TraKmeterAudioProcessor()
{
    removeAllActionListeners();
}


//==============================================================================

const String TraKmeterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}


int TraKmeterAudioProcessor::getNumParameters()
{
    return pluginParameters.getNumParameters(false);
}


const String TraKmeterAudioProcessor::getParameterName(int nIndex)
{
    return pluginParameters.getName(nIndex);
}


const String TraKmeterAudioProcessor::getParameterText(int nIndex)
{
    return pluginParameters.getText(nIndex);
}


float TraKmeterAudioProcessor::getParameter(int nIndex)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters.getFloat(nIndex);
}


void TraKmeterAudioProcessor::changeParameter(int nIndex, float fValue)
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


void TraKmeterAudioProcessor::setParameter(int nIndex, float fValue)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    // Please only call this method directly for non-automatable
    // values!

    pluginParameters.setFloat(nIndex, fValue);

    if (nIndex == TraKmeterPluginParameters::selTransientMode)
    {
        setTransientMode(getBoolean(nIndex));
    }
    else if (nIndex == TraKmeterPluginParameters::selCrestFactor)
    {
        setCrestFactor(getRealInteger(nIndex));
    }
    else if (nIndex == TraKmeterPluginParameters::selGain)
    {
        nDecibels = getRealInteger(TraKmeterPluginParameters::selGain);
        dGain = MeterBallistics::decibel2level_double(nDecibels);
    }

    // notify plug-in editor of parameter change
    if (pluginParameters.hasChanged(nIndex))
    {
        // for visible parameters, notify the editor of changes (this
        // will also clear the change flag)
        if (nIndex < pluginParameters.getNumParameters(false))
        {
            // "PC" --> parameter changed, followed by a hash and the
            // parameter's ID
            sendActionMessage("PC#" + String(nIndex));
        }
        // for hidden parameters, we only have to clear the change
        // flag
        else
        {
            pluginParameters.clearChangeFlag(nIndex);
        }
    }
}


void TraKmeterAudioProcessor::clearChangeFlag(int nIndex)
{
    pluginParameters.clearChangeFlag(nIndex);
}


bool TraKmeterAudioProcessor::hasChanged(int nIndex)
{
    return pluginParameters.hasChanged(nIndex);
}


void TraKmeterAudioProcessor::updateParameters(bool bIncludeHiddenParameters)
{
    int nNumParameters = pluginParameters.getNumParameters(false);

    for (int nIndex = 0; nIndex < nNumParameters; ++nIndex)
    {
        if (pluginParameters.hasChanged(nIndex))
        {
            float fValue = pluginParameters.getFloat(nIndex);
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


bool TraKmeterAudioProcessor::getBoolean(int nIndex)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters.getBoolean(nIndex);
}


int TraKmeterAudioProcessor::getRealInteger(int nIndex)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters.getRealInteger(nIndex);
}


File TraKmeterAudioProcessor::getParameterValidationFile()
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters.getValidationFile();
}


void TraKmeterAudioProcessor::setParameterValidationFile(const File &fileValidation)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    pluginParameters.setValidationFile(fileValidation);
}


String TraKmeterAudioProcessor::getParameterSkinName()
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters.getSkinName();
}


void TraKmeterAudioProcessor::setParameterSkinName(const String &strSkinName)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    pluginParameters.setSkinName(strSkinName);
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


bool TraKmeterAudioProcessor::silenceInProducesSilenceOut() const
{
    return true;
}


double TraKmeterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}


int TraKmeterAudioProcessor::getNumChannels()
{
    return nNumInputChannels;
}


int TraKmeterAudioProcessor::getNumPrograms()
{
    return 0;
}


int TraKmeterAudioProcessor::getCurrentProgram()
{
    return 0;
}


void TraKmeterAudioProcessor::setCurrentProgram(int nIndex)
{
}


const String TraKmeterAudioProcessor::getProgramName(int nIndex)
{
    return String::empty;
}


void TraKmeterAudioProcessor::changeProgramName(int nIndex, const String &newName)
{
}

//==============================================================================

void TraKmeterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    Logger::outputDebugString("[traKmeter] preparing to play");

    if ((sampleRate < 44100) || (sampleRate > 192000))
    {
        Logger::outputDebugString("[traKmeter] WARNING: sample rate of " + String(sampleRate) + " Hz not supported");
        bSampleRateIsValid = false;
        return;
    }
    else
    {
        bSampleRateIsValid = true;
    }

    isSilent = false;
    nNumInputChannels = getMainBusNumInputChannels();

    if (nNumInputChannels <= 0)
    {
        Logger::outputDebugString("[traKmeter] no input channels detected, correcting this");
        nNumInputChannels = JucePlugin_MaxNumInputChannels;
    }
    else if (nNumInputChannels < JucePlugin_MaxNumInputChannels)
    {
        Logger::outputDebugString("[traKmeter] only " +  String(nNumInputChannels) + " input channel(s) detected, correcting this");
        nNumInputChannels = JucePlugin_MaxNumInputChannels;
    }

    Logger::outputDebugString("[traKmeter] number of input channels: " + String(nNumInputChannels));

    pMeterBallistics = new MeterBallistics(nNumInputChannels, nCrestFactor, true, false, bTransientMode);

    // make sure that ring buffer can hold at least
    // nTrakmeterBufferSize samples and is large enough to receive a
    // full block of audio
    nSamplesInBuffer = 0;
    unsigned int uRingBufferSize = (samplesPerBlock > nTrakmeterBufferSize) ? samplesPerBlock : nTrakmeterBufferSize;

    pRingBufferInput = new frut::audio::RingBuffer("Input ring buffer", nNumInputChannels, uRingBufferSize, nTrakmeterBufferSize, nTrakmeterBufferSize);
    pRingBufferInput->setCallbackClass(this);
}


void TraKmeterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free
    // up any spare memory, etc.

    Logger::outputDebugString("[traKmeter] releasing resources");
    Logger::outputDebugString("");

    pMeterBallistics = nullptr;
    audioFilePlayer = nullptr;
}


void TraKmeterAudioProcessor::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages)
{
    // This is the place where you'd normally do the guts of your
    // plug-in's audio processing...

    int nNumSamples = buffer.getNumSamples();

    if (!bSampleRateIsValid)
    {
        for (int nChannel = 0; nChannel < getMainBusNumOutputChannels(); ++nChannel)
        {
            buffer.clear(nChannel, 0, nNumSamples);
        }

        return;
    }

    if (nNumInputChannels < 1)
    {
        Logger::outputDebugString("[traKmeter] nNumInputChannels < 1");
        return;
    }

    // In case we have more outputs than inputs, we'll clear any
    // output channels that didn't contain input data, because these
    // aren't guaranteed to be empty -- they may contain garbage.

    for (int nChannel = nNumInputChannels; nChannel < getMainBusNumOutputChannels(); ++nChannel)
    {
        buffer.clear(nChannel, 0, nNumSamples);
    }

    if (audioFilePlayer)
    {
        audioFilePlayer->fillBufferChunk(&buffer);
    }

    bool bMixMode = getBoolean(TraKmeterPluginParameters::selMixMode);

    if (bMixMode && (nDecibels != 0))
    {
        for (int nChannel = 0; nChannel < buffer.getNumChannels(); ++nChannel)
        {
            for (int nSample = 0; nSample < buffer.getNumSamples(); ++nSample)
            {
                double dSampleValue = buffer.getSample(nChannel, nSample);
                float fNewSampleValue = dither.dither(dSampleValue * dGain);
                buffer.setSample(nChannel, nSample, fNewSampleValue);
            }
        }
    }

    pRingBufferInput->addSamples(buffer, 0, nNumSamples);

    nSamplesInBuffer += nNumSamples;
    nSamplesInBuffer %= nTrakmeterBufferSize;
}


void TraKmeterAudioProcessor::processBufferChunk(AudioBuffer<float> &buffer, const unsigned int uChunkSize, const unsigned int uBufferPosition, const unsigned int uProcessedSamples)
{
    // silence input if validation window is open
    if (isSilent)
    {
        buffer.clear();
        pRingBufferInput->clear();
    }

    bool hasOpenEditor = (getActiveEditor() != nullptr);

    if (hasOpenEditor)
    {
        unsigned int uPreDelay = uChunkSize / 2;

        // length of buffer chunk in fractional seconds
        // (1024 samples / 44100 samples/s = 23.2 ms)
        fProcessedSeconds = (float) uChunkSize / (float) getSampleRate();

        for (int nChannel = 0; nChannel < nNumInputChannels; ++nChannel)
        {
            // determine peak level for uChunkSize samples (use
            // pre-delay)
            float fPeakLevels = pRingBufferInput->getMagnitude(nChannel, uChunkSize, uPreDelay);

            // determine peak level for uChunkSize samples (use
            // pre-delay)
            float fRmsLevels = pRingBufferInput->getRMSLevel(nChannel, uChunkSize, uPreDelay);

            // determine overflows for uChunkSize samples (use
            // pre-delay)
            int nOverflows = countOverflows(pRingBufferInput, nChannel, uChunkSize, uPreDelay);

            // apply meter ballistics and store values so that the
            // editor can access them
            pMeterBallistics->updateChannel(nChannel, fProcessedSeconds, fPeakLevels, fRmsLevels, nOverflows);
        }

        // "UM" --> update meters
        sendActionMessage("UM");
    }

    AudioBuffer<float> TempAudioBuffer(nNumInputChannels, uChunkSize);
    pRingBufferInput->copyToBuffer(TempAudioBuffer, 0, uChunkSize, 0);
}


void TraKmeterAudioProcessor::silenceInput(bool isSilentNew)
{
    isSilent = isSilentNew;
}


void TraKmeterAudioProcessor::startValidation(File fileAudio, int nSelectedChannel, bool bReportCSV, bool bAverageMeterLevel, bool bPeakMeterLevel)
{
    audioFilePlayer = new AudioFilePlayer(fileAudio, (int) getSampleRate(), pMeterBallistics, nCrestFactor);
    audioFilePlayer->setReporters(nSelectedChannel, bReportCSV, bAverageMeterLevel, bPeakMeterLevel);

    // reset all meters before we start the validation
    pMeterBallistics->reset();

    isSilent = false;

    // refresh editor; "V+" --> validation started
    sendActionMessage("V+");
}


void TraKmeterAudioProcessor::stopValidation()
{
    isSilent = false;
    audioFilePlayer = nullptr;

    // reset all meters after the validation
    pMeterBallistics->reset();

    // refresh editor; "V-" --> validation stopped
    sendActionMessage("V-");
}


bool TraKmeterAudioProcessor::isValidating()
{
    if (!audioFilePlayer)
    {
        return false;
    }
    else
    {
        if (audioFilePlayer->isPlaying())
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


int TraKmeterAudioProcessor::countOverflows(frut::audio::RingBuffer *ring_buffer, const unsigned int channel, const unsigned int length, const unsigned int pre_delay)
{
    // initialise number of overflows in this buffer
    int nOverflows = 0;

    // loop through samples of buffer
    for (unsigned int uSample = 0; uSample < length; ++uSample)
    {
        // get current sample value
        float fSampleValue = ring_buffer->getSample(channel, uSample, pre_delay);

        // in the 16-bit domain, full scale corresponds to an absolute
        // integer value of 32'767 or 32'768, so we'll treat absolute
        // levels of 32'767 and above as overflows; this corresponds
        // to a floating-point level of 32'767 / 32'768 = 0.9999694
        // (approx. -0.001 dBFS).
        if ((fSampleValue < -0.9999f) || (fSampleValue > 0.9999f))
        {
            ++nOverflows;
        }
    }

    // return number of overflows in this buffer
    return nOverflows;
}


MeterBallistics *TraKmeterAudioProcessor::getLevels()
{
    return pMeterBallistics;
}


bool TraKmeterAudioProcessor::getTransientMode()
{
    return bTransientMode;
}


void TraKmeterAudioProcessor::setTransientMode(const bool transient_mode)
{
    if (transient_mode != bTransientMode)
    {
        bTransientMode = transient_mode;

        if (pMeterBallistics)
        {
            pMeterBallistics = new MeterBallistics(nNumInputChannels, nCrestFactor, true, false, bTransientMode);
        }
    }
}


int TraKmeterAudioProcessor::getCrestFactor()
{
    return nCrestFactor;
}


void TraKmeterAudioProcessor::setCrestFactor(const int crest_factor)
{
    if (crest_factor != nCrestFactor)
    {
        nCrestFactor = crest_factor;

        if (pMeterBallistics)
        {
            pMeterBallistics->setCrestFactor(nCrestFactor);
        }

        if (audioFilePlayer)
        {
            audioFilePlayer->setCrestFactor(nCrestFactor);
        }
    }
}


//==============================================================================

AudioProcessorEditor *TraKmeterAudioProcessor::createEditor()
{
    //  meter ballistics are not updated when the editor is closed, so
    //  reset them here
    if (pMeterBallistics)
    {
        pMeterBallistics->reset();
    }

    if (nNumInputChannels > 0)
    {
        return new TraKmeterAudioProcessorEditor(this, &pluginParameters, nNumInputChannels, nCrestFactor);
    }
    else
    {
        return new TraKmeterAudioProcessorEditor(this, &pluginParameters, JucePlugin_MaxNumInputChannels, nCrestFactor);
    }
}


bool TraKmeterAudioProcessor::hasEditor() const
{
    return true;
}


//==============================================================================

void TraKmeterAudioProcessor::getStateInformation(MemoryBlock &destData)
{
    copyXmlToBinary(pluginParameters.storeAsXml(), destData);
}


void TraKmeterAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlDocument(getXmlFromBinary(data, sizeInBytes));
    pluginParameters.loadFromXml(xmlDocument);

    updateParameters(true);
}

//==============================================================================

// This creates new instances of the plug-in.
AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new TraKmeterAudioProcessor();
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
