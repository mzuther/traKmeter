/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
   Common classes for use with the JUCE library

   Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __PLUGIN_PARAMETER_STRING_H__
#define __PLUGIN_PARAMETER_STRING_H__

#include "JuceHeader.h"
#include "plugin_parameter.h"


/// Plug-in parameter for storing a String value.
///
/// The methods of this class may be called on the audio thread, so
/// they are absolutely time-critical!
///
class PluginParameterString : virtual public PluginParameter
{
public:
    PluginParameterString(const String &newValue);
    ~PluginParameterString();

    virtual void setDefaultRealFloat(float newRealValue, bool updateParameter) override;
    virtual void setFloat(float newValue) override;
    virtual void setRealFloat(float newRealValue) override;

    virtual const String getText() override;
    virtual void setText(const String &newValue) override;

    virtual float getFloatFromText(const String &newValue) override;
    virtual const String getTextFromFloat(float newValue) override;

    virtual void loadFromXml(XmlElement *xmlDocument) override;
    virtual void storeAsXml(XmlElement *xmlDocument) override;

private:
    JUCE_LEAK_DETECTOR(PluginParameterString);

    String textValue;
};


#endif  // __PLUGIN_PARAMETER_STRING_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
