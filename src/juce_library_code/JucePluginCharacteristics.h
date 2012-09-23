/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012 Martin Zuther (http://www.mzuther.de/)

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

--------------------------------------------------------------------------------

    This header file contains configuration options for the plug-in.

---------------------------------------------------------------------------- */

#ifndef __PLUGINCHARACTERISTICS_TRAKMETER__
#define __PLUGINCHARACTERISTICS_TRAKMETER__

#define JucePlugin_Build_VST    1
#define JucePlugin_Build_AU     0
#define JucePlugin_Build_RTAS   0

#ifdef TRAKMETER_MULTI
#ifdef DEBUG
#define JucePlugin_Name                 "traKmeter (Multi, Debug)"
#else
#define JucePlugin_Name                 "traKmeter (Multi)"
#endif
#else
#ifdef DEBUG
#define JucePlugin_Name                 "traKmeter (Stereo, Debug)"
#else
#define JucePlugin_Name                 "traKmeter (Stereo)"
#endif
#endif
#define JucePlugin_Desc                 "Loudness meter for correctly setting up tracking and mixing levels"
#define JucePlugin_Manufacturer         "mzuther"
#define JucePlugin_ManufacturerCode     'mz'
#define JucePlugin_PluginCode           'traK'
#ifdef TRAKMETER_MULTI
#define JucePlugin_MaxNumInputChannels  8
#define JucePlugin_MaxNumOutputChannels 8
#define JucePlugin_PreferredChannelConfigurations   {4, 4}, {6, 6}, {8, 8}
#else
#define JucePlugin_MaxNumInputChannels  2
#define JucePlugin_MaxNumOutputChannels 2
#define JucePlugin_PreferredChannelConfigurations   {1, 1}, {2, 2}
#endif
#define JucePlugin_IsSynth              0
#define JucePlugin_WantsMidiInput       0
#define JucePlugin_ProducesMidiOutput   0
#define JucePlugin_SilenceInProducesSilenceOut  1
#define JucePlugin_TailLengthSeconds    0
#define JucePlugin_EditorRequiresKeyboardFocus  0
#define JucePlugin_VersionCode          0x10301
#define JucePlugin_VersionString        "1.03.1"
#define JucePlugin_VSTUniqueID          JucePlugin_PluginCode
#define JucePlugin_VSTCategory          kPlugCategEffect
#define JucePlugin_AUMainType           kAudioUnitType_Effect
#define JucePlugin_AUSubType            JucePlugin_PluginCode
#define JucePlugin_AUExportPrefix       JuceProjectAU
#define JucePlugin_AUExportPrefixQuoted "JuceProjectAU"
#define JucePlugin_AUManufacturerCode   JucePlugin_ManufacturerCode
#define JucePlugin_CFBundleIdentifier   de.mzuther.traKmeter
#define JucePlugin_AUCocoaViewClassName JuceProjectAU_V1
#define JucePlugin_RTASCategory         ePlugInCategory_None
#define JucePlugin_RTASManufacturerCode JucePlugin_ManufacturerCode
#define JucePlugin_RTASProductId        JucePlugin_PluginCode

#endif   // __PLUGINCHARACTERISTICS_TRAKMETER__