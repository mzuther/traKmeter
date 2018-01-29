/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2018 Martin Zuther (http://www.mzuther.de/)

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

#pragma once

#include "FrutHeader.h"


class TraKmeterPluginParameters :
    public frut::parameter::Juggler
{
public:
    TraKmeterPluginParameters();

    File getValidationFile();
    void setValidationFile(const File &fileValidation);

    static const File getSkinDirectory();
    String getSkinName();
    void setSkinName(const String &strSkinName);

    enum Parameters  // public namespace!
    {
        selTransientMode = 0,
        selCrestFactor,
        selMeterType,

        numberOfParametersRevealed,

        selValidationFileName = numberOfParametersRevealed,
        selValidationSelectedChannel,
        selValidationAverageMeterLevel,
        selValidationPeakMeterLevel,
        selValidationCSVFormat,
        selSkinName,

        numberOfParametersComplete,

        selSplitMeters = 0,
        selCombinedMeters,

        nNumMeterTypes,
    };

private:
    JUCE_LEAK_DETECTOR(TraKmeterPluginParameters);
};


// Local Variables:
// ispell-local-dictionary: "british"
// End:
