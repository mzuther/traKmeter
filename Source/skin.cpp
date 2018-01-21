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

#include "skin.h"


bool Skin::loadSkin(
    File &skinFile,
    int numberOfChannels,
    int crestFactor,
    int meterType)

{
    updateSkin(numberOfChannels,
               crestFactor,
               meterType);

    return loadFromXml(skinFile, "trakmeter-skin", "1.2");
}


void Skin::updateSkin(int numberOfChannels,
                      int crestFactor,
                      int meterType)

{
    jassert(numberOfChannels > 0);

    currentBackgroundName_ = "image";

    if (numberOfChannels <= 2)
    {
        currentFallbackName_ = "stereo";
    }
    else
    {
        currentFallbackName_ = "multi";
    }

    if (meterType == TraKmeterPluginParameters::selSplitMeters)
    {
        currentFallbackName_ += "_split";
    }
    else
    {
        currentFallbackName_ += "_combined";
    }

    if (crestFactor == 20)
    {
        currentGroupName_ = currentFallbackName_ + "_k20";
    }
    else
    {
        currentGroupName_ = currentFallbackName_ + "_normal";
    }

    if (document_ != nullptr)
    {
        skinGroup_ = document_->getChildByName(currentGroupName_);
        skinFallback_1_ = document_->getChildByName(currentFallbackName_);
        skinFallback_2_ = document_->getChildByName("default");
    }
    else
    {
        skinGroup_ = nullptr;
        skinFallback_1_ = nullptr;
        skinFallback_2_ = nullptr;
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
