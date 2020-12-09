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

#include "plugin_parameters.h"


// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

TraKmeterPluginParameters::TraKmeterPluginParameters() :
   Juggler( "TRAKMETER_SETTINGS",
            numberOfParametersComplete,
            numberOfParametersRevealed )
{
   // parameters created here will be deleted in
   // "frut::parameters::Juggler"!

   auto ParameterTargetRecordingLevel = new frut::parameters::ParSwitch();
   ParameterTargetRecordingLevel->init();
   ParameterTargetRecordingLevel->setName( "Target recording level" );

   ParameterTargetRecordingLevel->addPreset( -10.0f, "-10 dB FS peak" );
   ParameterTargetRecordingLevel->addPreset( -15.0f, "-15 dB FS peak" );
   ParameterTargetRecordingLevel->addPreset( -20.0f, "-20 dB FS peak" );

   ParameterTargetRecordingLevel->setDefaultRealFloat( -10.0f, true );
   add( ParameterTargetRecordingLevel, selTargetRecordingLevel );


   auto ParameterValidationFileName = new frut::parameters::ParString();
   ParameterValidationFileName->init( "" );
   ParameterValidationFileName->setName( "Validation file" );
   add( ParameterValidationFileName, selValidationFileName );


   auto ParameterValidationSelectedChannel = new frut::parameters::ParSwitch();
   ParameterValidationSelectedChannel->init();
   ParameterValidationSelectedChannel->setName( "Validation audio channel" );

   // values correspond to the channel index in "AudioBuffer"
   ParameterValidationSelectedChannel->addPreset( -1.0f, "All" );
   ParameterValidationSelectedChannel->addPreset( 0.0f,   "1" );
   ParameterValidationSelectedChannel->addPreset( 1.0f,   "2" );
#ifdef TRAKMETER_MULTI
   ParameterValidationSelectedChannel->addPreset( 2.0f,   "3" );
   ParameterValidationSelectedChannel->addPreset( 3.0f,   "4" );
   ParameterValidationSelectedChannel->addPreset( 4.0f,   "5" );
   ParameterValidationSelectedChannel->addPreset( 5.0f,   "6" );
   ParameterValidationSelectedChannel->addPreset( 6.0f,   "7" );
   ParameterValidationSelectedChannel->addPreset( 7.0f,   "8" );
#endif // TRAKMETER_MULTI

   ParameterValidationSelectedChannel->setDefaultRealFloat( -1.0f, true );
   add( ParameterValidationSelectedChannel, selValidationSelectedChannel );


   auto ParameterValidationAverageMeterLevel = new frut::parameters::ParBoolean();
   ParameterValidationAverageMeterLevel->init( "On", "Off" );
   ParameterValidationAverageMeterLevel->setName( "Validate average meter level" );
   ParameterValidationAverageMeterLevel->setDefaultBoolean( true, true );
   add( ParameterValidationAverageMeterLevel, selValidationAverageMeterLevel );


   auto ParameterValidationPeakMeterLevel = new frut::parameters::ParBoolean();
   ParameterValidationPeakMeterLevel->init( "On", "Off" );
   ParameterValidationPeakMeterLevel->setName( "Validate peak meter level" );
   ParameterValidationPeakMeterLevel->setDefaultBoolean( true, true );
   add( ParameterValidationPeakMeterLevel, selValidationPeakMeterLevel );


   auto ParameterValidationCSVFormat = new frut::parameters::ParBoolean();
   ParameterValidationCSVFormat->init( "CSV", "Full" );
   ParameterValidationCSVFormat->setName( "Validation output format" );
   ParameterValidationCSVFormat->setDefaultBoolean( false, true );
   add( ParameterValidationCSVFormat, selValidationCSVFormat );
}


File TraKmeterPluginParameters::getValidationFile()
{
   File validationFile = File( getText( selValidationFileName ) );

   if ( validationFile.existsAsFile() ) {
      return validationFile;
   } else {
      return File();
   }
}


void TraKmeterPluginParameters::setValidationFile( const File& validationFile )
{
   if ( validationFile.existsAsFile() ) {
      String validationFileName = validationFile.getFullPathName();
      setText( selValidationFileName, validationFileName );
   }
}


const File TraKmeterPluginParameters::getResourceDirectory()
{
   // On all platforms we want the resource folder to be located with
   // the executable.  On Mac, the executable is *not* the same as
   // the application folder because what looks like an application
   // is really a package (i.e. a folder) with the executable being
   // buried inside.
   //
   // When deploying on a Mac, right-click on the build target and
   // select "Show Package Contents".  Navigate through
   // Contents/MacOS and you will find the traKmeter executable.  Put
   // the "trakmeter" folder here.
   //
   // Thanks to Tod Gentille!

   File applicationDirectory;

#ifdef __APPLE__
   applicationDirectory = File::getSpecialLocation(
                             File::currentExecutableFile ).getParentDirectory();
#else // __APPLE__
   applicationDirectory = File::getSpecialLocation(
                             File::currentApplicationFile ).getParentDirectory();
#endif // __APPLE__

#if JucePlugin_Build_VST3
   return applicationDirectory.getChildFile( "../Resources/" );
#else // JucePlugin_Build_VST3
   return applicationDirectory;
#endif // JucePlugin_Build_VST3
}
