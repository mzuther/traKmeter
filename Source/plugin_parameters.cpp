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

    ParameterTransientMode = new WrappedParameterToggleSwitch("Off", "On");
    ParameterTransientMode->setName("Transient mode");
    ParameterTransientMode->setDefaultBoolean(true, true);
    add(ParameterTransientMode, selTransientMode);


    ParameterCrestFactor = new WrappedParameterSwitch();
    ParameterCrestFactor->setName("Crest factor");

    ParameterCrestFactor->addConstant(0.0f,  "0 dB (digital full-scale)");
    ParameterCrestFactor->addConstant(20.0f, "20 dB (K-20 scale)");

    ParameterCrestFactor->setDefaultRealFloat(20.0f, true);
    add(ParameterCrestFactor, selCrestFactor);


    ParameterMixMode = new WrappedParameterToggleSwitch("Off", "On");
    ParameterMixMode->setName("Mixing mode");
    ParameterMixMode->setDefaultBoolean(false, true);
    add(ParameterMixMode, selMixMode);


    ParameterGain = new WrappedParameterSwitch();
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


    ParameterMeterType = new WrappedParameterSwitch();
    ParameterMeterType->setName("Meter type");

    ParameterMeterType->addConstant(selSeparateMeters,  "Separate");
    ParameterMeterType->addConstant(selCombinedMeters,  "Combined");

    ParameterMeterType->setDefaultRealFloat(selSeparateMeters, true);
    add(ParameterMeterType, selMeterType);


    ParameterValidationFileName = new WrappedParameterString(String::empty);
    ParameterValidationFileName->setName("Validation: file name");
    add(ParameterValidationFileName, selValidationFileName);


    ParameterValidationSelectedChannel = new WrappedParameterSwitch();
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


    ParameterValidationAverageMeterLevel = new WrappedParameterToggleSwitch("Off", "On");
    ParameterValidationAverageMeterLevel->setName("Validation: average meter level");
    ParameterValidationAverageMeterLevel->setDefaultBoolean(true, true);
    add(ParameterValidationAverageMeterLevel, selValidationAverageMeterLevel);


    ParameterValidationPeakMeterLevel = new WrappedParameterToggleSwitch("Off", "On");
    ParameterValidationPeakMeterLevel->setName("Validation: peak meter level");
    ParameterValidationPeakMeterLevel->setDefaultBoolean(true, true);
    add(ParameterValidationPeakMeterLevel, selValidationPeakMeterLevel);


    ParameterValidationCSVFormat = new WrappedParameterToggleSwitch("Off", "On");
    ParameterValidationCSVFormat->setName("Validation: CSV output format");
    ParameterValidationCSVFormat->setDefaultBoolean(false, true);
    add(ParameterValidationCSVFormat, selValidationCSVFormat);
}


TraKmeterPluginParameters::~TraKmeterPluginParameters()
{
    delete ParameterTransientMode;
    ParameterTransientMode = nullptr;

    delete ParameterCrestFactor;
    ParameterCrestFactor = nullptr;

    delete ParameterMixMode;
    ParameterMixMode = nullptr;

    delete ParameterGain;
    ParameterGain = nullptr;

    delete ParameterMeterType;
    ParameterMeterType = nullptr;

    delete ParameterValidationFileName;
    ParameterValidationFileName = nullptr;

    delete ParameterValidationSelectedChannel;
    ParameterValidationSelectedChannel = nullptr;

    delete ParameterValidationAverageMeterLevel;
    ParameterValidationAverageMeterLevel = nullptr;

    delete ParameterValidationPeakMeterLevel;
    ParameterValidationPeakMeterLevel = nullptr;

    delete ParameterValidationCSVFormat;
    ParameterValidationCSVFormat = nullptr;
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
    File fileValidation = File(ParameterValidationFileName->getText());

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
        ParameterValidationFileName->setText(strFilename);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
