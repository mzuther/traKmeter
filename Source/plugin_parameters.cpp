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

#include "plugin_parameters.h"


// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

TraKmeterPluginParameters::TraKmeterPluginParameters() :
    Juggler("TRAKMETER_SETTINGS", numberOfParametersComplete,
            numberOfParametersRevealed)
{
    // parameters created here will be deleted in
    // "frut::parameter::Juggler"!

    frut::parameter::ParBoolean *ParameterTransientMode =
        new frut::parameter::ParBoolean("On", "Off");
    ParameterTransientMode->setName("Transient mode");
    ParameterTransientMode->setDefaultBoolean(true, true);
    add(ParameterTransientMode, selTransientMode);


    frut::parameter::ParSwitch *ParameterCrestFactor =
        new frut::parameter::ParSwitch();
    ParameterCrestFactor->setName("Crest factor");

    ParameterCrestFactor->addPreset(0.0f,  "0 dB (digital full-scale)");
    ParameterCrestFactor->addPreset(20.0f, "20 dB (K-20 scale)");

    ParameterCrestFactor->setDefaultRealFloat(20.0f, true);
    add(ParameterCrestFactor, selCrestFactor);


    frut::parameter::ParBoolean *ParameterMixMode =
        new frut::parameter::ParBoolean("On", "Off");
    ParameterMixMode->setName("Mixing mode");
    ParameterMixMode->setDefaultBoolean(false, true);
    add(ParameterMixMode, selMixMode);


    frut::parameter::ParSwitch *ParameterGain =
        new frut::parameter::ParSwitch();
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


    frut::parameter::ParSwitch *ParameterMeterType =
        new frut::parameter::ParSwitch();
    ParameterMeterType->setName("Meter type");

    ParameterMeterType->addPreset(selSplitMeters,     "Split");
    ParameterMeterType->addPreset(selCombinedMeters,  "Combined");

    ParameterMeterType->setDefaultRealFloat(selSplitMeters, true);
    add(ParameterMeterType, selMeterType);


    frut::parameter::ParString *ParameterValidationFileName =
        new frut::parameter::ParString(String::empty);
    ParameterValidationFileName->setName("Validation file");
    add(ParameterValidationFileName, selValidationFileName);


    frut::parameter::ParSwitch *ParameterValidationSelectedChannel =
        new frut::parameter::ParSwitch();
    ParameterValidationSelectedChannel->setName("Validation audio channel");

    // values correspond to the channel index in "AudioBuffer"
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


    frut::parameter::ParBoolean *ParameterValidationAverageMeterLevel =
        new frut::parameter::ParBoolean("On", "Off");
    ParameterValidationAverageMeterLevel->setName("Validate average meter level");
    ParameterValidationAverageMeterLevel->setDefaultBoolean(true, true);
    add(ParameterValidationAverageMeterLevel, selValidationAverageMeterLevel);


    frut::parameter::ParBoolean *ParameterValidationPeakMeterLevel =
        new frut::parameter::ParBoolean("On", "Off");
    ParameterValidationPeakMeterLevel->setName("Validate peak meter level");
    ParameterValidationPeakMeterLevel->setDefaultBoolean(true, true);
    add(ParameterValidationPeakMeterLevel, selValidationPeakMeterLevel);


    frut::parameter::ParBoolean *ParameterValidationCSVFormat =
        new frut::parameter::ParBoolean("CSV", "Full");
    ParameterValidationCSVFormat->setName("Validation output format");
    ParameterValidationCSVFormat->setDefaultBoolean(false, true);
    add(ParameterValidationCSVFormat, selValidationCSVFormat);


    // locate directory containing the skins
    File skinDirectory = getSkinDirectory();

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

    frut::parameter::ParString *ParameterSkinName =
        new frut::parameter::ParString(defaultSkinName);
    ParameterSkinName->setName("Skin");
    add(ParameterSkinName, selSkinName);
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


void TraKmeterPluginParameters::setValidationFile(const File &validationFile)
{
    if (validationFile.existsAsFile())
    {
        String validationFileName = validationFile.getFullPathName();
        setText(selValidationFileName, validationFileName);
    }
}


const File TraKmeterPluginParameters::getSkinDirectory()
{
    // On all platforms we want the skins folder to be located with
    // the executable.  On Mac, the executable is *not* the same as
    // the application folder because what looks like an application
    // is really a package (i.e. a folder) with the executable being
    // buried inside.
    //
    // When deploying on a Mac, right-click on the build target and
    // select "Show Package Contents".  Navigate through
    // Contents/MacOS and you will find the K-Meter executable.  Put
    // the kmeter folder here.
    //
    // Thanks to Tod Gentille!

    File applicationDirectory;

#ifdef __APPLE__
    applicationDirectory = File::getSpecialLocation(
                               File::currentExecutableFile).getParentDirectory();
#else
    applicationDirectory = File::getSpecialLocation(
                               File::currentApplicationFile).getParentDirectory();
#endif

    return applicationDirectory.getChildFile("./trakmeter/skins/");
}


String TraKmeterPluginParameters::getSkinName()
{
    return getText(selSkinName);
}


void TraKmeterPluginParameters::setSkinName(const String &strSkinName)
{
    setText(selSkinName, strSkinName);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
