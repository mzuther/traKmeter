/* ----------------------------------------------------------------------------

   Parameter Juggler
   =================
   Module for handling plug-in parameters in JUCE

   Copyright (c) 2013-2015 Martin Zuther (http://www.mzuther.de/)

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

#include "wrapped_parameter_combined.h"


WrappedParameterCombined::WrappedParameterCombined(float real_minimum, float real_maximum, float resolution, float log_factor, int decimal_places)
{
    strName = String::empty;
    strAttribute = String::empty;

    // these parameters should not be deleted outside of this class
    pSwitch = new WrappedParameterSwitch(true);
    pContinuous = new WrappedParameterContinuous(real_minimum, real_maximum, resolution, log_factor, decimal_places, true);
    pModeSwitch = new WrappedParameterToggleSwitch("Discrete", "Continuous", true);

    bUseConstants = true;
    pModeSwitch->setBoolean(bUseConstants);

    setChangeFlag();
    setChangeFlagMode();
}


WrappedParameterCombined::~WrappedParameterCombined()
{
    delete pSwitch;
    pSwitch = nullptr;

    delete pContinuous;
    pContinuous = nullptr;

    delete pModeSwitch;
    pModeSwitch = nullptr;
}


String WrappedParameterCombined::getName()
{
    return strName;
}


void WrappedParameterCombined::setName(const String &strParameterName)
{
    strName = strParameterName;
    strAttribute = strName.removeCharacters(" ");

    pModeSwitch->setName(strName + " Mode");
}


void WrappedParameterCombined::addConstant(const float fRealValue, const String &strText)
{
    pSwitch->addConstant(fRealValue, strText);
}


bool WrappedParameterCombined::getMode()
{
    return bUseConstants;
}


bool WrappedParameterCombined::setMode(bool use_constants)
{
    if (bUseConstants != use_constants)
    {
        return toggleMode();
    }
    else
    {
        return true;
    }
}


bool WrappedParameterCombined::toggleMode()
{
    pModeSwitch->toggleState();
    bUseConstants = pModeSwitch->getBoolean();

    setChangeFlag();
    setChangeFlagMode();

    if (bUseConstants)
    {
        float fRealValue = pContinuous->getRealFloat();
        return pSwitch->setNearestRealFloat(fRealValue);
    }
    else
    {
        float fRealValue = pSwitch->getRealFloat();
        return pContinuous->setNearestRealFloat(fRealValue);
    }
}


WrappedParameterToggleSwitch *WrappedParameterCombined::getModeSwitch()
{
    return pModeSwitch;
}


float WrappedParameterCombined::getInterval()
{
    if (bUseConstants)
    {
        return pSwitch->getInterval();
    }
    else
    {
        return pContinuous->getInterval();
    }
}


float WrappedParameterCombined::getDefaultFloat()
{
    if (bUseConstants)
    {
        return pSwitch->getDefaultFloat();
    }
    else
    {
        return pContinuous->getDefaultFloat();
    }
}


float WrappedParameterCombined::getDefaultRealFloat()
{
    if (bUseConstants)
    {
        return pSwitch->getDefaultRealFloat();
    }
    else
    {
        return pContinuous->getDefaultRealFloat();
    }
}


bool WrappedParameterCombined::getDefaultBoolean()
{
    if (bUseConstants)
    {
        return pSwitch->getDefaultBoolean();
    }
    else
    {
        return pContinuous->getDefaultBoolean();
    }
}


int WrappedParameterCombined::getDefaultRealInteger()
{
    if (bUseConstants)
    {
        return pSwitch->getDefaultRealInteger();
    }
    else
    {
        return pContinuous->getDefaultRealInteger();
    }
}


bool WrappedParameterCombined::setDefaultRealFloat(float fRealValue, bool updateValue)
{
    if (bUseConstants)
    {
        bool bSwitchFound = pSwitch->setDefaultRealFloat(fRealValue, updateValue);
        bool bContinuousFound = pContinuous->setDefaultRealFloat(fRealValue, false);

        return (bSwitchFound && bContinuousFound);
    }
    else
    {
        bool bContinuousFound = pContinuous->setDefaultRealFloat(fRealValue, updateValue);
        bool bSwitchFound = pSwitch->setDefaultRealFloat(fRealValue, true);

        return (bContinuousFound && bSwitchFound);
    }
}


float WrappedParameterCombined::getFloat()
{
    if (bUseConstants)
    {
        return pSwitch->getFloat();
    }
    else
    {
        return pContinuous->getFloat();
    }
}


bool WrappedParameterCombined::setFloat(float fValue)
{
    if (bUseConstants)
    {
        return pSwitch->setFloat(fValue);
    }
    else
    {
        return pContinuous->setFloat(fValue);
    }
}


float WrappedParameterCombined::getRealFloat()
{
    if (bUseConstants)
    {
        return pSwitch->getRealFloat();
    }
    else
    {
        return pContinuous->getRealFloat();
    }
}


bool WrappedParameterCombined::setRealFloat(float fRealValue)
{
    if (bUseConstants)
    {
        return pSwitch->setRealFloat(fRealValue);
    }
    else
    {
        return pContinuous->setRealFloat(fRealValue);
    }
}


bool WrappedParameterCombined::getBoolean()
{
    if (bUseConstants)
    {
        return pSwitch->getBoolean();
    }
    else
    {
        return pContinuous->getBoolean();
    }
}


bool WrappedParameterCombined::setBoolean(bool bValue)
{
    if (bUseConstants)
    {
        return pSwitch->setBoolean(bValue);
    }
    else
    {
        return pContinuous->setBoolean(bValue);
    }
}


int WrappedParameterCombined::getRealInteger()
{
    if (bUseConstants)
    {
        return pSwitch->getRealInteger();
    }
    else
    {
        return pContinuous->getRealInteger();
    }
}


bool WrappedParameterCombined::setRealInteger(int nRealValue)
{
    if (bUseConstants)
    {
        return pSwitch->setRealInteger(nRealValue);
    }
    else
    {
        return pContinuous->setRealInteger(nRealValue);
    }
}


String WrappedParameterCombined::getText()
{
    if (bUseConstants)
    {
        return pSwitch->getText();
    }
    else
    {
        return pContinuous->getText();
    }
}


bool WrappedParameterCombined::setText(const String &strText)
{
    if (bUseConstants)
    {
        return pSwitch->setText(strText);
    }
    else
    {
        return pContinuous->setText(strText);
    }
}


void WrappedParameterCombined::setSuffix(const String &suffix)
{
    pContinuous->setSuffix(suffix);
}


float WrappedParameterCombined::getFloatFromText(const String &strText)
{
    if (bUseConstants)
    {
        return pSwitch->getFloatFromText(strText);
    }
    else
    {
        return pContinuous->getFloatFromText(strText);
    }
}


String WrappedParameterCombined::getTextFromFloat(float fValue)
{
    if (bUseConstants)
    {
        return pSwitch->getTextFromFloat(fValue);
    }
    else
    {
        return pContinuous->getTextFromFloat(fValue);
    }
}


bool WrappedParameterCombined::hasChanged()
{
    bool bChangedSwitch = pSwitch->hasChanged();
    bool bChangedContinuous = pContinuous->hasChanged();

    return (bChangedSwitch || bChangedContinuous);
}


void WrappedParameterCombined::clearChangeFlag()
{
    pSwitch->clearChangeFlag();
    pContinuous->clearChangeFlag();
}


void WrappedParameterCombined::setChangeFlag()
{
    pSwitch->setChangeFlag();
    pContinuous->setChangeFlag();
}


bool WrappedParameterCombined::hasChangedMode()
{
    return bChangedMode;
}


void WrappedParameterCombined::clearChangeFlagMode()
{
    bChangedMode = false;
}


void WrappedParameterCombined::setChangeFlagMode()
{
    bChangedMode = true;
}


bool WrappedParameterCombined::saveFromDeletion()
{
    return false;
}


void WrappedParameterCombined::loadFromXml(XmlElement *xml)
{
    XmlElement *xml_element = xml->getChildByName(strAttribute);

    if (xml_element)
    {
        bool useConstants = xml_element->getBoolAttribute("use_constants", true);
        float fRealValue;

        if (xml_element->hasAttribute("value"))
        {
            fRealValue = (float) xml_element->getDoubleAttribute("value", getDefaultRealFloat());
        }
        else
        {
            String strRealValue = xml_element->getAllSubText().trim();

            if (strRealValue.isEmpty())
            {
                fRealValue = getDefaultRealFloat();
            }
            else
            {
                fRealValue = strRealValue.getFloatValue();
            }
        }

        setMode(useConstants);
        setRealFloat(fRealValue);
    }
}


void WrappedParameterCombined::storeAsXml(XmlElement *xml)
{
    XmlElement *xml_element = new XmlElement(strAttribute);

    if (xml_element)
    {
        float fRealValue = getRealFloat();

        xml_element->setAttribute("use_constants", bUseConstants ? "true" : "false");
        xml_element->setAttribute("value", fRealValue);
        xml->addChildElement(xml_element);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
