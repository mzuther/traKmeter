/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2013 Martin Zuther (http://www.mzuther.de/)

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
    nParam = new int[nNumParameters];

    nParam[selTransientMode] = 1;
    nParam[selCrestFactor] = 20;
    nParam[selMixMode] = 0;
    nParam[selGain] = 0;

    nParam[selValidationSelectedChannel] = -1;
    nParam[selValidationAverageMeterLevel] = 1;
    nParam[selValidationPeakMeterLevel] = 1;

    nParam[selValidationCSVFormat] = 0;

    strValidationFile = String::empty;

    bParamChanged = new bool[nNumParameters];

    for (int nIndex = 0; nIndex < nNumParameters; nIndex++)
    {
        UnmarkParameter(nIndex);
    }
}


TraKmeterPluginParameters::~TraKmeterPluginParameters()
{
    removeAllActionListeners();

    delete [] nParam;
    nParam = NULL;

    delete [] bParamChanged;
    bParamChanged = NULL;
}


int TraKmeterPluginParameters::getNumParameters(bool bIncludeHiddenParameters)
{
    if (bIncludeHiddenParameters)
    {
        return nNumParameters;
    }
    else
    {
        return nNumParametersRevealed;
    }
}


bool TraKmeterPluginParameters::getParameterAsBool(int nIndex)
{
    return (getParameterAsInt(nIndex) != 0) ? true : false;
}


float TraKmeterPluginParameters::getParameterAsFloat(int nIndex)
{
    int nValue = getParameterAsInt(nIndex);
    return translateParameterToFloat(nIndex, nValue);
}


int TraKmeterPluginParameters::getParameterAsInt(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));

    return nParam[nIndex];
}


void TraKmeterPluginParameters::setParameterFromBool(int nIndex, bool bValue)
{
    setParameterFromInt(nIndex, bValue ? 1 : 0);
}


void TraKmeterPluginParameters::setParameterFromFloat(int nIndex, float fValue)
{
    int nValue = translateParameterToInt(nIndex, fValue);
    setParameterFromInt(nIndex, nValue);
}


void TraKmeterPluginParameters::setParameterFromInt(int nIndex, int nValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));

    if (nParam[nIndex] != nValue)
    {
        if (nIndex == selCrestFactor)
        {
            nParam[nIndex] = nValue;
        }
        else if (nIndex == selGain)
        {
            nParam[nIndex] = nValue;
        }
        else if (nIndex == selValidationSelectedChannel)
        {
            nParam[nIndex] = nValue;
        }
        else
        {
            nParam[nIndex] = (nValue != 0) ? 1 : 0;
        }

        MarkParameter(nIndex);
        // "PC" --> parameter changed, followed by a hash and the
        // parameter's ID
        sendActionMessage("PC#" + String(nIndex));
    }
}


File TraKmeterPluginParameters::getValidationFile()
{
    File fileValidation = File(strValidationFile);

    if (fileValidation.existsAsFile())
    {
        return fileValidation;
    }
    else
    {
        return File::nonexistent;
    }
}


void TraKmeterPluginParameters::setValidationFile(File& fileValidation)
{
    if (fileValidation.existsAsFile())
    {
        strValidationFile = fileValidation.getFullPathName();
    }
}


void TraKmeterPluginParameters::MarkParameter(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));

    bParamChanged[nIndex] = true;
}


void TraKmeterPluginParameters::UnmarkParameter(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));

    bParamChanged[nIndex] = false;
}


bool TraKmeterPluginParameters::isParameterMarked(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));

    return bParamChanged[nIndex];
}


const String TraKmeterPluginParameters::getParameterName(int nIndex)
{
    switch (nIndex)
    {
    case selTransientMode:
        return "Transient mode";
        break;

    case selCrestFactor:
        return "Crest factor";
        break;

    case selMixMode:
        return "Mixing mode";
        break;

    case selGain:
        return "Gain";
        break;

    case selValidationFileName:
        return "Validation: file name";
        break;

    case selValidationSelectedChannel:
        return "Validation: selected channel";
        break;

    case selValidationAverageMeterLevel:
        return "Validation: average meter level";
        break;

    case selValidationPeakMeterLevel:
        return "Validation: peak meter level";
        break;

    case selValidationCSVFormat:
        return "Validation: CSV output format";
        break;

    default:
        return "invalid";
        break;
    }
}


const String TraKmeterPluginParameters::getParameterText(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));

    if (nIndex == selCrestFactor)
    {
        return getParameterAsBool(nIndex) ? "K-20" : "None";
    }
    else if (nIndex == selGain)
    {
        int nDecibels = nParam[nIndex];

        if (nDecibels > 0)
        {
            return "+" + String(nDecibels) + " dB";
        }
        else if (nDecibels < 0)
        {
            return String(nDecibels) + " dB";
        }
        else
        {
            return "0 dB";
        }
    }
    else if (nIndex == selValidationFileName)
    {
        File fileValidation = File(strValidationFile);

        if (fileValidation.existsAsFile())
        {
            return strValidationFile;
        }
        else
        {
            return String::empty;
        }
    }
    else if (nIndex == selValidationSelectedChannel)
    {
        if (nParam[nIndex] < 0)
        {
            return "All";
        }
        else
        {
            return String(nParam[nIndex]);
        }
    }
    else
    {
        return getParameterAsBool(nIndex) ? "On" : "Off";
    }
}


float TraKmeterPluginParameters::translateParameterToFloat(int nIndex, int nValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));

    if (nIndex == selCrestFactor)
    {
        return (nValue != 0) ? 1.0f : 0.0f;
    }
    else if (nIndex == selGain)
    {
        // 0.00f: -12 dB
        // ...
        // 0.12f:   0 dB
        // ...
        // 0.24f: +12 dB
        return nValue / 100.0f + 0.12f;
    }
    else if (nIndex == selValidationSelectedChannel)
    {
        // 0.00f: dump all channels
        // 0.01f: dump channel #0
        // 0.02f: dump channel #1
        // ...
        // 1.00f: dump channel #99
        return (nValue + 1.0f) / 100.0f;
    }
    else
    {
        return (nValue != 0) ? 1.0f : 0.0f;
    }
}


int TraKmeterPluginParameters::translateParameterToInt(int nIndex, float fValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));

    if (nIndex == selCrestFactor)
    {
        return (fValue > 0.5f) ? 20 : 0;
    }
    else if (nIndex == selGain)
    {
        // 0.00f: -12 dB
        // ...
        // 0.12f:   0 dB
        // ...
        // 0.24f: +12 dB
        int nRoundedValue = int(fValue * 100.0f + 0.5f);
        return nRoundedValue - 12;
    }
    else if (nIndex == selValidationSelectedChannel)
    {
        // 0.00f: dump all channels
        // 0.01f: dump channel #0
        // 0.02f: dump channel #1
        // ...
        // 1.00f: dump channel #99
        int nRoundedValue = int(fValue * 100.0f + 0.5f);
        return nRoundedValue - 1;
    }
    else
    {
        return (fValue > 0.5f) ? true : false;
    }
}


XmlElement TraKmeterPluginParameters::storeAsXml()
{
    XmlElement xml("TRAKMETER_SETTINGS");

    xml.setAttribute("TransientMode", getParameterAsInt(selTransientMode));
    xml.setAttribute("CrestFactor", getParameterAsInt(selCrestFactor));
    xml.setAttribute("MixMode", getParameterAsInt(selMixMode));
    xml.setAttribute("Gain", getParameterAsInt(selGain));

    xml.setAttribute("ValidationFile", strValidationFile);
    xml.setAttribute("ValidationSelectedChannel", getParameterAsInt(selValidationSelectedChannel));
    xml.setAttribute("ValidationAverageMeterLevel", getParameterAsInt(selValidationAverageMeterLevel));
    xml.setAttribute("ValidationPeakMeterLevel", getParameterAsInt(selValidationPeakMeterLevel));
    xml.setAttribute("ValidationCSVFormat", getParameterAsInt(selValidationCSVFormat));

    return xml;
}


void TraKmeterPluginParameters::loadFromXml(XmlElement* xml)
{
    if (xml && xml->hasTagName("TRAKMETER_SETTINGS"))
    {
        setParameterFromInt(selTransientMode, xml->getIntAttribute("TransientMode", getParameterAsInt(selTransientMode)));
        setParameterFromInt(selCrestFactor, xml->getIntAttribute("CrestFactor", getParameterAsInt(selCrestFactor)));
        setParameterFromInt(selMixMode, xml->getIntAttribute("MixMode", getParameterAsInt(selMixMode)));
        setParameterFromInt(selGain, xml->getIntAttribute("Gain", getParameterAsInt(selGain)));

        File fileValidation = File(xml->getStringAttribute("ValidationFile", strValidationFile));
        setValidationFile(fileValidation);

        setParameterFromInt(selValidationSelectedChannel, xml->getIntAttribute("ValidationSelectedChannel", getParameterAsInt(selValidationSelectedChannel)));
        setParameterFromInt(selValidationAverageMeterLevel, xml->getIntAttribute("ValidationAverageMeterLevel", getParameterAsInt(selValidationAverageMeterLevel)));
        setParameterFromInt(selValidationPeakMeterLevel, xml->getIntAttribute("ValidationPeakMeterLevel", getParameterAsInt(selValidationPeakMeterLevel)));
        setParameterFromInt(selValidationCSVFormat, xml->getIntAttribute("ValidationCSVFormat", getParameterAsInt(selValidationCSVFormat)));
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
