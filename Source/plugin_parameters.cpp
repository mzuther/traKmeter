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
    strSettingsID = "TRAKMETER_SETTINGS";

    WrappedParameterToggleSwitch *ParameterTransientMode = new WrappedParameterToggleSwitch("Off", "On");
    ParameterTransientMode->setName("Transient mode");
    ParameterTransientMode->setDefaultBoolean(true, true);
    add(ParameterTransientMode, selTransientMode);


    WrappedParameterSwitch *ParameterCrestFactor = new WrappedParameterSwitch();
    ParameterCrestFactor->setName("Crest factor");

    ParameterCrestFactor->addConstant(0.0f,  "0 dB (digital full-scale)");
    ParameterCrestFactor->addConstant(20.0f, "20 dB (K-20 scale)");

    ParameterCrestFactor->setDefaultRealFloat(20.0f, true);
    add(ParameterCrestFactor, selCrestFactor);


    WrappedParameterToggleSwitch *ParameterMixMode = new WrappedParameterToggleSwitch("Off", "On");
    ParameterMixMode->setName("Mixing mode");
    ParameterMixMode->setDefaultBoolean(false, true);
    add(ParameterMixMode, selMixMode);


    WrappedParameterSwitch *ParameterGain = new WrappedParameterSwitch();
    ParameterGain->setName("Gain");

    ParameterGain->addConstant(-12.0f, "-12 dB");
    ParameterGain->addConstant(-11.0f, "-11 dB");
    ParameterGain->addConstant(-10.0f, "-10 dB");
    ParameterGain->addConstant(-9.0f,   "-9 dB");
    ParameterGain->addConstant(-8.0f,   "-8 dB");
    ParameterGain->addConstant(-7.0f,   "-7 dB");
    ParameterGain->addConstant(-6.0f,   "-6 dB");
    ParameterGain->addConstant(-5.0f,   "-5 dB");
    ParameterGain->addConstant(-4.0f,   "-4 dB");
    ParameterGain->addConstant(-3.0f,   "-3 dB");
    ParameterGain->addConstant(-2.0f,   "-2 dB");
    ParameterGain->addConstant(-1.0f,   "-1 dB");
    ParameterGain->addConstant(0.0f,     "0 dB");
    ParameterGain->addConstant(+1.0f,   "+1 dB");
    ParameterGain->addConstant(+2.0f,   "+2 dB");
    ParameterGain->addConstant(+3.0f,   "+3 dB");
    ParameterGain->addConstant(+4.0f,   "+4 dB");
    ParameterGain->addConstant(+5.0f,   "+5 dB");
    ParameterGain->addConstant(+6.0f,   "+6 dB");
    ParameterGain->addConstant(+7.0f,   "+7 dB");
    ParameterGain->addConstant(+8.0f,   "+8 dB");
    ParameterGain->addConstant(+9.0f,   "+9 dB");
    ParameterGain->addConstant(+10.0f, "+10 dB");
    ParameterGain->addConstant(+11.0f, "+11 dB");
    ParameterGain->addConstant(+12.0f, "+12 dB");

    ParameterGain->setDefaultRealFloat(0.0f, true);
    add(ParameterGain, selGain);


    WrappedParameterSwitch *ParameterMeterType = new WrappedParameterSwitch();
    ParameterMeterType->setName("Meter type");

    ParameterMeterType->addConstant(selSplitMeters,     "Split");
    ParameterMeterType->addConstant(selCombinedMeters,  "Combined");

    ParameterMeterType->setDefaultRealFloat(selSplitMeters, true);
    add(ParameterMeterType, selMeterType);


    WrappedParameterString *ParameterValidationFileName = new WrappedParameterString(String::empty);
    ParameterValidationFileName->setName("Validation: file name");
    add(ParameterValidationFileName, selValidationFileName);


    WrappedParameterSwitch *ParameterValidationSelectedChannel = new WrappedParameterSwitch();
    ParameterValidationSelectedChannel->setName("Validation: selected channel");

    // values correspond to the channel index in AudioSampleBuffer
    ParameterValidationSelectedChannel->addConstant(-1.0f, "All");
    ParameterValidationSelectedChannel->addConstant(0.0f,   "1");
    ParameterValidationSelectedChannel->addConstant(1.0f,   "2");
#ifdef TRAKMETER_MULTI
    ParameterValidationSelectedChannel->addConstant(2.0f,   "3");
    ParameterValidationSelectedChannel->addConstant(3.0f,   "4");
    ParameterValidationSelectedChannel->addConstant(4.0f,   "5");
    ParameterValidationSelectedChannel->addConstant(5.0f,   "6");
    ParameterValidationSelectedChannel->addConstant(6.0f,   "7");
    ParameterValidationSelectedChannel->addConstant(7.0f,   "8");
#endif

    ParameterValidationSelectedChannel->setDefaultRealFloat(-1.0f, true);
    add(ParameterValidationSelectedChannel, selValidationSelectedChannel);


    WrappedParameterToggleSwitch *ParameterValidationAverageMeterLevel = new WrappedParameterToggleSwitch("Off", "On");
    ParameterValidationAverageMeterLevel->setName("Validation: average meter level");
    ParameterValidationAverageMeterLevel->setDefaultBoolean(true, true);
    add(ParameterValidationAverageMeterLevel, selValidationAverageMeterLevel);


    WrappedParameterToggleSwitch *ParameterValidationPeakMeterLevel = new WrappedParameterToggleSwitch("Off", "On");
    ParameterValidationPeakMeterLevel->setName("Validation: peak meter level");
    ParameterValidationPeakMeterLevel->setDefaultBoolean(true, true);
    add(ParameterValidationPeakMeterLevel, selValidationPeakMeterLevel);


    WrappedParameterToggleSwitch *ParameterValidationCSVFormat = new WrappedParameterToggleSwitch("Off", "On");
    ParameterValidationCSVFormat->setName("Validation: CSV output format");
    ParameterValidationCSVFormat->setDefaultBoolean(false, true);
    add(ParameterValidationCSVFormat, selValidationCSVFormat);


    // the following may or may not work on Mac
    File fileApplicationDirectory = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory();
    File fileSkinDirectory = fileApplicationDirectory.getChildFile("./trakmeter-skins/");

    // locate file containing the default skin's name
    File defaultSkinFile = fileSkinDirectory.getChildFile("default_skin.ini");

    // make sure the file exists
    if (!defaultSkinFile.existsAsFile())
    {
        // create file
        defaultSkinFile.create();

        // set "Default" as default skin (using Unicode encoding)
        defaultSkinFile.replaceWithText("Default", true, true);
    }

    // load name of default skin from file
    String strDefaultSkinName = defaultSkinFile.loadFileAsString();

    WrappedParameterString *ParameterSkinName = new WrappedParameterString(strDefaultSkinName);
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
        return nNumParametersComplete;
    }
    else
    {
        return nNumParametersRevealed;
    }
}


File TraKmeterPluginParameters::getValidationFile()
{
    File fileValidation = File(getText(selValidationFileName));

    if (fileValidation.existsAsFile())
    {
        return fileValidation;
    }
    else
    {
        return File::nonexistent;
    }
}


void TraKmeterPluginParameters::setValidationFile(File &fileValidation)
{
    if (fileValidation.existsAsFile())
    {
        String strFilename = fileValidation.getFullPathName();
        setText(selValidationFileName, strFilename);
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
