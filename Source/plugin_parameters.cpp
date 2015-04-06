/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2015 Martin Zuther (http://www.mzuther.de/)

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

TraKmeterPluginParameters::TraKmeterPluginParameters()
{
    jugglerID = "TRAKMETER_SETTINGS";

    PluginParameterBoolean *ParameterTransientMode = new PluginParameterBoolean("Off", "On");
    ParameterTransientMode->setName("Transient mode");
    ParameterTransientMode->setDefaultBoolean(true, true);
    add(ParameterTransientMode, selTransientMode);


    PluginParameterSwitch *ParameterCrestFactor = new PluginParameterSwitch();
    ParameterCrestFactor->setName("Crest factor");

    ParameterCrestFactor->addPreset(0.0f,  "0 dB (digital full-scale)");
    ParameterCrestFactor->addPreset(20.0f, "20 dB (K-20 scale)");

    ParameterCrestFactor->setDefaultRealFloat(20.0f, true);
    add(ParameterCrestFactor, selCrestFactor);


    PluginParameterBoolean *ParameterMixMode = new PluginParameterBoolean("Off", "On");
    ParameterMixMode->setName("Mixing mode");
    ParameterMixMode->setDefaultBoolean(false, true);
    add(ParameterMixMode, selMixMode);


    PluginParameterSwitch *ParameterGain = new PluginParameterSwitch();
    ParameterGain->setName("Gain");

    ParameterGain->addPreset(-12.0f, "-12 dB");
    ParameterGain->addPreset(-11.0f, "-11 dB");
    ParameterGain->addPreset(-10.0f, "-10 dB");
    ParameterGain->addPreset(-9.0f,   "-9 dB");
    ParameterGain->addPreset(-8.0f,   "-8 dB");
    ParameterGain->addPreset(-7.0f,   "-7 dB");
    ParameterGain->addPreset(-6.0f,   "-6 dB");
    ParameterGain->addPreset(-5.0f,   "-5 dB");
    ParameterGain->addPreset(-4.0f,   "-4 dB");
    ParameterGain->addPreset(-3.0f,   "-3 dB");
    ParameterGain->addPreset(-2.0f,   "-2 dB");
    ParameterGain->addPreset(-1.0f,   "-1 dB");
    ParameterGain->addPreset(0.0f,     "0 dB");
    ParameterGain->addPreset(+1.0f,   "+1 dB");
    ParameterGain->addPreset(+2.0f,   "+2 dB");
    ParameterGain->addPreset(+3.0f,   "+3 dB");
    ParameterGain->addPreset(+4.0f,   "+4 dB");
    ParameterGain->addPreset(+5.0f,   "+5 dB");
    ParameterGain->addPreset(+6.0f,   "+6 dB");
    ParameterGain->addPreset(+7.0f,   "+7 dB");
    ParameterGain->addPreset(+8.0f,   "+8 dB");
    ParameterGain->addPreset(+9.0f,   "+9 dB");
    ParameterGain->addPreset(+10.0f, "+10 dB");
    ParameterGain->addPreset(+11.0f, "+11 dB");
    ParameterGain->addPreset(+12.0f, "+12 dB");

    ParameterGain->setDefaultRealFloat(0.0f, true);
    add(ParameterGain, selGain);


    PluginParameterSwitch *ParameterMeterType = new PluginParameterSwitch();
    ParameterMeterType->setName("Meter type");

    ParameterMeterType->addPreset(selSplitMeters,     "Split");
    ParameterMeterType->addPreset(selCombinedMeters,  "Combined");

    ParameterMeterType->setDefaultRealFloat(selSplitMeters, true);
    add(ParameterMeterType, selMeterType);


    PluginParameterString *ParameterValidationFileName = new PluginParameterString(String::empty);
    ParameterValidationFileName->setName("Validation: file name");
    add(ParameterValidationFileName, selValidationFileName);


    PluginParameterSwitch *ParameterValidationSelectedChannel = new PluginParameterSwitch();
    ParameterValidationSelectedChannel->setName("Validation: selected channel");

    // values correspond to the channel index in AudioSampleBuffer
    ParameterValidationSelectedChannel->addPreset(-1.0f, "All");
    ParameterValidationSelectedChannel->addPreset(0.0f,   "1");
    ParameterValidationSelectedChannel->addPreset(1.0f,   "2");
#ifdef TRAKMETER_MULTI
    ParameterValidationSelectedChannel->addPreset(2.0f,   "3");
    ParameterValidationSelectedChannel->addPreset(3.0f,   "4");
    ParameterValidationSelectedChannel->addPreset(4.0f,   "5");
    ParameterValidationSelectedChannel->addPreset(5.0f,   "6");
    ParameterValidationSelectedChannel->addPreset(6.0f,   "7");
    ParameterValidationSelectedChannel->addPreset(7.0f,   "8");
#endif

    ParameterValidationSelectedChannel->setDefaultRealFloat(-1.0f, true);
    add(ParameterValidationSelectedChannel, selValidationSelectedChannel);


    PluginParameterBoolean *ParameterValidationAverageMeterLevel = new PluginParameterBoolean("Off", "On");
    ParameterValidationAverageMeterLevel->setName("Validation: average meter level");
    ParameterValidationAverageMeterLevel->setDefaultBoolean(true, true);
    add(ParameterValidationAverageMeterLevel, selValidationAverageMeterLevel);


    PluginParameterBoolean *ParameterValidationPeakMeterLevel = new PluginParameterBoolean("Off", "On");
    ParameterValidationPeakMeterLevel->setName("Validation: peak meter level");
    ParameterValidationPeakMeterLevel->setDefaultBoolean(true, true);
    add(ParameterValidationPeakMeterLevel, selValidationPeakMeterLevel);


    PluginParameterBoolean *ParameterValidationCSVFormat = new PluginParameterBoolean("Off", "On");
    ParameterValidationCSVFormat->setName("Validation: CSV output format");
    ParameterValidationCSVFormat->setDefaultBoolean(false, true);
    add(ParameterValidationCSVFormat, selValidationCSVFormat);


    // the following may or may not work on Mac
    File applicationDirectory = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory();
    File skinDirectory = applicationDirectory.getChildFile("./trakmeter-skins/");

    // locate file containing the default skin's name
    File defaultSkinFile = skinDirectory.getChildFile("default_skin.ini");

    // make sure the file exists
    if (!defaultSkinFile.existsAsFile())
    {
        // create file
        defaultSkinFile.create();

        // set "Default" as default skin (using Unicode encoding)
        defaultSkinFile.replaceWithText("Default", true, true);
    }

    // load name of default skin from file
    String defaultSkinName = defaultSkinFile.loadFileAsString();

    PluginParameterString *ParameterSkinName = new PluginParameterString(defaultSkinName);
    ParameterSkinName->setName("Skin");
    add(ParameterSkinName, selSkinName);
}


TraKmeterPluginParameters::~TraKmeterPluginParameters()
{
    // parameters will be deleted in "ParameterJuggler"
}


int TraKmeterPluginParameters::getNumParameters(bool bIncludeHiddenParameters)
{
    if (bIncludeHiddenParameters)
    {
        return numberOfParametersComplete;
    }
    else
    {
        return numberOfParametersRevealed;
    }
}


File TraKmeterPluginParameters::getValidationFile()
{
    File validationFile = File(getText(selValidationFileName));

    if (validationFile.existsAsFile())
    {
        return validationFile;
    }
    else
    {
        return File::nonexistent;
    }
}


void TraKmeterPluginParameters::setValidationFile(File &validationFile)
{
    if (validationFile.existsAsFile())
    {
        String validationFileName = validationFile.getFullPathName();
        setText(selValidationFileName, validationFileName);
    }
}


String TraKmeterPluginParameters::getSkinName()
{
    return getText(selSkinName);
}


void TraKmeterPluginParameters::setSkinName(String &strSkinName)
{
    setText(selSkinName, strSkinName);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
