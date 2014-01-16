/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2014 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __PLUGINPARAMETERS_H__
#define __PLUGINPARAMETERS_H__

class TraKmeterPluginParameters;

#include "../JuceLibraryCode/JuceHeader.h"

//============================================================================
class TraKmeterPluginParameters  : public ActionBroadcaster
{
public:
    //==========================================================================

    TraKmeterPluginParameters();
    ~TraKmeterPluginParameters();

    int getNumParameters(bool bIncludeHiddenParameters);

    bool getParameterAsBool(int nIndex);
    float getParameterAsFloat(int nIndex);
    int getParameterAsInt(int nIndex);

    void setParameterFromBool(int nIndex, bool bValue);
    void setParameterFromFloat(int nIndex, float fValue);
    void setParameterFromInt(int nIndex, int nValue);

    File getValidationFile();
    void setValidationFile(File& fileValidation);

    void MarkParameter(int nIndex);
    void UnmarkParameter(int nIndex);
    bool isParameterMarked(int nIndex);

    const String getParameterName(int nIndex);
    const String getParameterText(int nIndex);

    int translateParameterToInt(int nIndex, float fValue);
    float translateParameterToFloat(int nIndex, int nValue);

    XmlElement storeAsXml();
    void loadFromXml(XmlElement* xml);

    enum Parameters  // public namespace!
    {
        selTransientMode = 0,
        selCrestFactor,
        selMixMode,
        selGain,
        selMeterType,

        nNumParametersRevealed,

        selValidationFileName = nNumParametersRevealed,
        selValidationSelectedChannel,
        selValidationAverageMeterLevel,
        selValidationPeakMeterLevel,
        selValidationCSVFormat,

        nNumParameters,

        selSeparateMeters = 0,
        selCombinedMeters,
        selAbstractMeters,

        nNumMeterTypes,
    };

private:
    JUCE_LEAK_DETECTOR(TraKmeterPluginParameters);

    int* nParam;
    bool* bParamChanged;
    String strValidationFile;
};

#endif  // __PLUGINPARAMETERS_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
