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
   AudioProcessor( getBusesProperties() ),
#endif // JucePlugin_PreferredChannelConfigurations
   trakmeterBufferSize_( 1024 )
{
   frut::Frut::printVersionNumbers();

#ifdef TRAKMETER_STEREO
   numberOfChannels_ = 2;
#else // TRAKMETER_STEREO
   numberOfChannels_ = 8;
#endif // TRAKMETER_STEREO

   ringBuffer_ = nullptr;
   audioFilePlayer_ = nullptr;
   meterBallistics_ = nullptr;

   sampleRateIsValid_ = false;
   isSilent_ = false;
   hasStopped_ = true;
   reloadEditor_ = false;

   processedSeconds_ = 0.0f;

   // this is a meter for recording: do not introduce latency!
   setLatencySamples( 0 );
}


TraKmeterAudioProcessor::~TraKmeterAudioProcessor()
{
   removeAllActionListeners();
}


#ifndef JucePlugin_PreferredChannelConfigurations

AudioProcessor::BusesProperties TraKmeterAudioProcessor::getBusesProperties()
{
#ifdef TRAKMETER_STEREO

   return BusesProperties()
          .withInput( "Main",
                      AudioChannelSet::stereo() )
          .withOutput( "Main",
                       AudioChannelSet::stereo() );

#else // TRAKMETER_STEREO

   return BusesProperties()
          .withInput( "Main",
                      AudioChannelSet::canonicalChannelSet( 8 ) )
          .withOutput( "Main",
                       AudioChannelSet::canonicalChannelSet( 8 ) );

#endif // TRAKMETER_STEREO
}

#endif // JucePlugin_PreferredChannelConfigurations


#ifndef JucePlugin_PreferredChannelConfigurations

bool TraKmeterAudioProcessor::isBusesLayoutSupported( const BusesLayout& layouts ) const
{
#ifdef TRAKMETER_STEREO

   // main output must be stereo
   if ( layouts.getMainOutputChannelSet() != AudioChannelSet::stereo() ) {
      return false;
   }

#else // TRAKMETER_STEREO

#ifdef TRAKMETER_MULTICHANNEL

   // main output must have 8 channels
   if ( layouts.getMainOutputChannelSet() != AudioChannelSet::canonicalChannelSet( 8 ) ) {
      return false;
   }

#else // TRAKMETER_MULTICHANNEL

   // main output may have from 2 to 8 channels (preventing mono
   // layout keeps processorLayoutsChanged() simple)
   if ( ( layouts.getMainOutputChannelSet().size() < 2 ) ||
        ( layouts.getMainOutputChannelSet().size() > 8 ) ) {
      return false;
   }

#endif // TRAKMETER_MULTICHANNEL

#endif // TRAKMETER_STEREO

   // main input must be enabled
   if ( layouts.getMainInputChannelSet().isDisabled() ) {
      return false;
   }

   // main input and main output must be identical
   if ( layouts.getMainInputChannelSet() != layouts.getMainOutputChannelSet() ) {
      return false;
   }

   // valid channel layout
   return true;
}

#endif // JucePlugin_PreferredChannelConfigurations


#ifndef JucePlugin_PreferredChannelConfigurations

void TraKmeterAudioProcessor::processorLayoutsChanged()
{
   reloadEditor_ = true;

   if ( getMainBusNumOutputChannels() <= 2 ) {
      numberOfChannels_ = 2;
   } else {
      numberOfChannels_ = 8;
   }
}

#endif // JucePlugin_PreferredChannelConfigurations


const String TraKmeterAudioProcessor::getName() const
{
   return JucePlugin_Name;
}


int TraKmeterAudioProcessor::getNumParameters()
{
   return pluginParameters_.getNumParameters( false );
}


const String TraKmeterAudioProcessor::getParameterName( int nIndex )
{
   return pluginParameters_.getName( nIndex );
}


const String TraKmeterAudioProcessor::getParameterText( int nIndex )
{
   return pluginParameters_.getText( nIndex );
}


float TraKmeterAudioProcessor::getParameter( int nIndex )
{
   // This method will be called by the host, probably on the audio
   // thread, so it's absolutely time-critical. Don't use critical
   // sections or anything GUI-related, or anything at all that may
   // block in any way!

   return pluginParameters_.getFloat( nIndex );
}


void TraKmeterAudioProcessor::changeParameter( int nIndex,
                                               float fValue )
{
   // This method will be called by the host, probably on the audio
   // thread, so it's absolutely time-critical. Don't use critical
   // sections or anything GUI-related, or anything at all that may
   // block in any way!

   // notify host of parameter change (this will automatically call
   // "setParameter"!)
   beginParameterChangeGesture( nIndex );
   setParameterNotifyingHost( nIndex, fValue );
   endParameterChangeGesture( nIndex );
}


void TraKmeterAudioProcessor::setParameter( int nIndex,
                                            float fValue )
{
   // This method will be called by the host, probably on the audio
   // thread, so it's absolutely time-critical. Don't use critical
   // sections or anything GUI-related, or anything at all that may
   // block in any way!

   // Please only call this method directly for non-automatable
   // values!

   pluginParameters_.setFloat( nIndex, fValue );

   // notify plug-in editor of parameter change
   if ( pluginParameters_.hasChanged( nIndex ) ) {
      // for visible parameters, notify the editor of changes (this
      // will also clear the change flag)
      if ( nIndex < pluginParameters_.getNumParameters( false ) ) {
         // "PC" ==> parameter changed, followed by a hash and the
         // parameter's ID
         sendActionMessage( "PC#" + String( nIndex ) );
         // for hidden parameters, we only have to clear the change
         // flag
      } else {
         pluginParameters_.clearChangeFlag( nIndex );
      }
   }
}


void TraKmeterAudioProcessor::clearChangeFlag( int nIndex )
{
   pluginParameters_.clearChangeFlag( nIndex );
}


bool TraKmeterAudioProcessor::hasChanged( int nIndex )
{
   return pluginParameters_.hasChanged( nIndex );
}


void TraKmeterAudioProcessor::updateParameters( bool bIncludeHiddenParameters )
{
   int nNumParameters = pluginParameters_.getNumParameters( false );

   for ( int nIndex = 0; nIndex < nNumParameters; ++nIndex ) {
      if ( pluginParameters_.hasChanged( nIndex ) ) {
         float fValue = pluginParameters_.getFloat( nIndex );
         changeParameter( nIndex, fValue );
      }
   }

   if ( bIncludeHiddenParameters ) {
      // handle hidden parameters here!

      // the following parameters need no updating:
      //
      // * selValidationFileName
      // * selValidationSelectedChannel
      // * selValidationAverageMeterLevel
      // * selValidationPeakMeterLevel
      // * selValidationCSVFormat
   }
}


bool TraKmeterAudioProcessor::getBoolean( int nIndex )
{
   // This method will be called by the host, probably on the audio
   // thread, so it's absolutely time-critical. Don't use critical
   // sections or anything GUI-related, or anything at all that may
   // block in any way!

   return pluginParameters_.getBoolean( nIndex );
}


int TraKmeterAudioProcessor::getRealInteger( int nIndex )
{
   // This method will be called by the host, probably on the audio
   // thread, so it's absolutely time-critical. Don't use critical
   // sections or anything GUI-related, or anything at all that may
   // block in any way!

   return pluginParameters_.getRealInteger( nIndex );
}


File TraKmeterAudioProcessor::getParameterValidationFile()
{
   // This method will be called by the host, probably on the audio
   // thread, so it's absolutely time-critical. Don't use critical
   // sections or anything GUI-related, or anything at all that may
   // block in any way!

   return pluginParameters_.getValidationFile();
}


void TraKmeterAudioProcessor::setParameterValidationFile( const File& fileValidation )
{
   // This method will be called by the host, probably on the audio
   // thread, so it's absolutely time-critical. Don't use critical
   // sections or anything GUI-related, or anything at all that may
   // block in any way!

   pluginParameters_.setValidationFile( fileValidation );
}


bool TraKmeterAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
   return true;
#else // JucePlugin_WantsMidiInput
   return false;
#endif // JucePlugin_WantsMidiInput
}


bool TraKmeterAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
   return true;
#else // JucePlugin_ProducesMidiOutput
   return false;
#endif // JucePlugin_ProducesMidiOutput
}


bool TraKmeterAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
   return true;
#else // JucePlugin_IsMidiEffect
   return false;
#endif // JucePlugin_IsMidiEffect
}


double TraKmeterAudioProcessor::getTailLengthSeconds() const
{
   return 0.0;
}


StringArray TraKmeterAudioProcessor::getAlternateDisplayNames() const
{
   StringArray displayNames( JucePlugin_Name );
   displayNames.add( "traKmeter" );
   displayNames.add( "trkm" );

   return displayNames;
}


int TraKmeterAudioProcessor::getNumPrograms()
{
   // some hosts don't cope very well if you tell them there are no
   // programs, so this should be at least 1, even if you're not
   // really implementing programs.
   return 1;
}


int TraKmeterAudioProcessor::getCurrentProgram()
{
   return 0;
}


void TraKmeterAudioProcessor::setCurrentProgram( int nIndex )
{
   ignoreUnused( nIndex );
}


const String TraKmeterAudioProcessor::getProgramName( int nIndex )
{
   ignoreUnused( nIndex );

   return String();
}


void TraKmeterAudioProcessor::changeProgramName( int nIndex,
                                                 const String& newName )
{
   ignoreUnused( nIndex, newName );
}


void TraKmeterAudioProcessor::prepareToPlay( double sampleRate,
                                             int samplesPerBlock )
{
   // Use this method as the place to do any pre-playback
   // initialisation that you need..

   Logger::outputDebugString( "[traKmeter] preparing to play" );

   if ( ( sampleRate < 44100 ) || ( sampleRate > 192000 ) ) {
      Logger::outputDebugString( "[traKmeter] WARNING: sample rate of " +
                                 String( sampleRate ) + " Hz not supported" );
      sampleRateIsValid_ = false;
      return;
   } else {
      sampleRateIsValid_ = true;
   }

   isSilent_ = false;
   hasStopped_ = true;

   Logger::outputDebugString( "[traKmeter] number of main/aux input channels:  " +
                              String( getMainBusNumInputChannels() ) + "/" +
                              String( getTotalNumInputChannels() - getMainBusNumInputChannels() ) );
   Logger::outputDebugString( "[traKmeter] number of main/aux output channels: " +
                              String( getMainBusNumOutputChannels() ) + "/" +
                              String( getTotalNumOutputChannels() - getMainBusNumOutputChannels() ) );

   if ( reloadEditor_ ) {
      reloadEditor_ = false;
      auto editor = dynamic_cast<TraKmeterAudioProcessorEditor*>( getActiveEditor() );

      if ( editor != nullptr ) {
         editor->setNumberOfChannels( numberOfChannels_ );
      }
   }

   dither_.initialise( jmax( getMainBusNumInputChannels(),
                             getMainBusNumOutputChannels() ),
                       24 );

   meterBallistics_ = std::make_shared<MeterBallistics>(
                         numberOfChannels_,
                         true,
                         false );

   // make sure that ring buffer can hold at least
   // trakmeterBufferSize_ samples and is large enough to receive a
   // full block of audio
   int ringBufferSize = jmax( samplesPerBlock, trakmeterBufferSize_ );

   // this is a meter for recording: do not use pre-delay!
   int preDelay = 0;
   int chunkSize = trakmeterBufferSize_;

   ringBuffer_ = std::make_unique<frut::audio::RingBuffer<float>>(
                    getMainBusNumOutputChannels(),
                    ringBufferSize,
                    preDelay,
                    chunkSize );

   ringBuffer_->setCallbackClass( this );
}


void TraKmeterAudioProcessor::releaseResources()
{
   // When playback stops, you can use this as an opportunity to free
   // up any spare memory, etc.

   Logger::outputDebugString( "[traKmeter] releasing resources" );
   Logger::outputDebugString( "" );

   hasStopped_ = true;

   meterBallistics_ = nullptr;
   audioFilePlayer_ = nullptr;

   ringBuffer_ = nullptr;
}


void TraKmeterAudioProcessor::reset()
{
   // Use this method as the place to clear any delay lines, buffers,
   // etc, as it means there's been a break in the audio's
   // continuity.

   hasStopped_ = true;
   processedSeconds_ = 0.0f;
   ringBuffer_->clear();
}


void TraKmeterAudioProcessor::processBlock( AudioBuffer<float>& buffer,
                                            MidiBuffer& midiMessages )
{
   jassert( ! isUsingDoublePrecision() );
   ignoreUnused( midiMessages );

   // temporarily disable denormals
   ScopedNoDenormals noDenormals;

   if ( ! sampleRateIsValid_ ) {
      buffer.clear();
      return;
   }

   // In case we have more main outputs than inputs, we'll clear any
   // output channels that didn't contain input data, because these
   // aren't guaranteed to be empty -- they may contain garbage.
   for ( int channel = getMainBusNumInputChannels(); channel < getMainBusNumOutputChannels(); ++channel ) {
      buffer.clear( channel, 0, buffer.getNumSamples() );
   }

   if ( getMainBusNumInputChannels() < 1 ) {
      Logger::outputDebugString( "[traKmeter] no input channels!" );
      return;
   }

   // reset meters if playback has started
   resetOnPlay();

   if ( audioFilePlayer_ ) {
      audioFilePlayer_->copyTo( buffer );
      // silence input if validation window is open
   } else if ( isSilent_ ) {
      buffer.clear();
   }

   // copy input buffer to ring buffer
   //
   // calls "processBufferChunk" each time chunkSize samples have
   // been added!
   ringBuffer_->addFrom( buffer, 0, buffer.getNumSamples() );

   // the processed buffer data is not needed, so simulate reading
   // the ring buffer (move read pointer to prevent the "overwriting
   // unread data" debug message from appearing)
   ringBuffer_->removeToNull( buffer.getNumSamples() );
}


void TraKmeterAudioProcessor::processBlock( AudioBuffer<double>& buffer,
                                            MidiBuffer& midiMessages )
{
   jassert( isUsingDoublePrecision() );
   ignoreUnused( midiMessages );

   DBG( "[traKmeter] using double precision" );

   // temporarily disable denormals
   ScopedNoDenormals noDenormals;

   if ( ! sampleRateIsValid_ ) {
      buffer.clear();
      return;
   }

   // In case we have more main outputs than inputs, we'll clear any
   // output channels that didn't contain input data, because these
   // aren't guaranteed to be empty -- they may contain garbage.
   for ( int channel = getMainBusNumInputChannels(); channel < getMainBusNumOutputChannels(); ++channel ) {
      buffer.clear( channel, 0, buffer.getNumSamples() );
   }

   if ( getMainBusNumInputChannels() < 1 ) {
      Logger::outputDebugString( "[traKmeter] no input channels!" );
      return;
   }

   // reset meters if playback has started
   resetOnPlay();

   // create temporary buffer
   AudioBuffer<float> processBuffer( getMainBusNumOutputChannels(), buffer.getNumSamples() );

   // copy validation audio samples to temporary buffer
   if ( audioFilePlayer_ ) {
      audioFilePlayer_->copyTo( processBuffer );

      // overwrite output buffer
      dither_.convertToDouble( processBuffer, buffer );
      // silence input if validation window is open
   } else if ( isSilent_ ) {
      buffer.clear();
      processBuffer.clear();
   } else {
      // copy input to temporary buffer and convert to float; the
      // process buffer is not going to be heard, so we may truncate
      // the sample values
      dither_.truncateToFloat( buffer, processBuffer );
   }

   // copy temporary buffer to ring buffer
   //
   // calls "processBufferChunk" each time chunkSize samples have
   // been added!
   ringBuffer_->addFrom( processBuffer, 0, buffer.getNumSamples() );

   // the processed buffer data is not needed, so simulate reading
   // the ring buffer (move read pointer to prevent the "overwriting
   // unread data" debug message from appearing)
   ringBuffer_->removeToNull( buffer.getNumSamples() );
}


bool TraKmeterAudioProcessor::processBufferChunk( AudioBuffer<float>& buffer )
{
   int chunkSize = buffer.getNumSamples();

   // length of buffer chunk in fractional seconds
   // (1024 samples / 44100 samples/s = 23.2 ms)
   processedSeconds_ = static_cast<float>( chunkSize ) /
                       static_cast<float>( getSampleRate() );

   for ( int channel = 0; channel < buffer.getNumChannels(); ++channel ) {
      // determine peak level for chunkSize samples
      float peakLevel = buffer.getMagnitude( channel, 0, chunkSize );

      // determine peak level for chunkSize samples
      float rmsLevel = buffer.getRMSLevel( channel, 0, chunkSize );

      // apply meter ballistics and store values so that the editor
      // can access them
      meterBallistics_->updateChannel( channel,
                                       processedSeconds_,
                                       peakLevel,
                                       rmsLevel );
   }

   // "UM" ==> update meters
   sendActionMessage( "UM" );

   // keep ring buffer contents
   return false;
}


void TraKmeterAudioProcessor::resetOnPlay()
{
   // get play head
   AudioPlayHead* playHead = AudioProcessor::getPlayHead();

   // check success
   if ( playHead != nullptr ) {
      AudioPlayHead::CurrentPositionInfo currentPosition;

      // get current position of play head (and check success)
      if ( playHead->getCurrentPosition( currentPosition ) ) {
         // check whether sequencer is currently playing
         bool isPlayingAgain = currentPosition.isPlaying;

         // check whether playback has just started
         if ( hasStopped_ && isPlayingAgain ) {
            // clear meters
            if ( meterBallistics_ != nullptr ) {
               meterBallistics_->reset();
            }
         }

         // update play state
         hasStopped_ = !isPlayingAgain;
      }
   }
}


void TraKmeterAudioProcessor::silenceInput( bool isSilentNew )
{
   isSilent_ = isSilentNew;
}


void TraKmeterAudioProcessor::startValidation( File fileAudio,
                                               int nSelectedChannel,
                                               bool bReportCSV,
                                               bool bAverageMeterLevel,
                                               bool bPeakMeterLevel )
{
   // reset all meters before we start the validation
   meterBallistics_->reset();

   isSilent_ = false;

   audioFilePlayer_ = std::make_unique<AudioFilePlayer>(
                         fileAudio,
                         ( int ) getSampleRate(),
                         meterBallistics_ );

   if ( audioFilePlayer_->matchingSampleRates() ) {
      audioFilePlayer_->setReporters( nSelectedChannel, bReportCSV,
                                      bAverageMeterLevel, bPeakMeterLevel );

      // refresh editor; "V+" ==> validation started
      sendActionMessage( "V+" );
   } else {
      stopValidation();

      AlertWindow::showMessageBoxAsync(
         AlertWindow::WarningIcon,
         "Validation error",
         "Sample rates of host and validation file do not match." );
   }

}


void TraKmeterAudioProcessor::stopValidation()
{
   isSilent_ = false;
   audioFilePlayer_ = nullptr;

   // reset all meters after the validation
   meterBallistics_->reset();

   // refresh editor; "V-" ==> validation stopped
   sendActionMessage( "V-" );
}


bool TraKmeterAudioProcessor::isValidating()
{
   if ( ! audioFilePlayer_ ) {
      return false;
   } else {
      if ( audioFilePlayer_->isPlaying() ) {
         return true;
      } else {
         stopValidation();
         return false;
      }
   }
}


std::shared_ptr<MeterBallistics> TraKmeterAudioProcessor::getLevels()
{
   return meterBallistics_;
}


void TraKmeterAudioProcessor::resetMeters()
{
   if ( meterBallistics_ ) {
      meterBallistics_->reset();
   }
}


AudioProcessorEditor* TraKmeterAudioProcessor::createEditor()
{
   //  meter ballistics are not updated when the editor is closed, so
   //  reset them here
   if ( meterBallistics_ ) {
      meterBallistics_->reset();
   }

   return new TraKmeterAudioProcessorEditor(
             *this, numberOfChannels_ );
}


bool TraKmeterAudioProcessor::hasEditor() const
{
   return true;
}


void TraKmeterAudioProcessor::getStateInformation( MemoryBlock& destData )
{
   XmlElement xmlParameters = pluginParameters_.storeAsXml();

   DBG( "[traKmeter]" );
   DBG( "[traKmeter] storing plug-in parameters:" );
   DBG( "[traKmeter]" );
   DBG( String( "[traKmeter]   " ) + xmlParameters.toString().replace(
           "\n", "\n[traKmeter]   " ) );

   copyXmlToBinary( xmlParameters, destData );
}


void TraKmeterAudioProcessor::setStateInformation( const void* data,
                                                   int sizeInBytes )
{
   std::unique_ptr<XmlElement> xmlParameters( getXmlFromBinary( data, sizeInBytes ) );

   DBG( "[traKmeter] loading plug-in parameters:" );
   DBG( "[traKmeter]" );
   DBG( String( "[traKmeter]   " ) + xmlParameters->toString().replace(
           "\n", "\n[traKmeter]   " ) );

   pluginParameters_.loadFromXml( xmlParameters.get() );
   updateParameters( true );
}


// This creates new instances of the plug-in.
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
   return new TraKmeterAudioProcessor();
}
