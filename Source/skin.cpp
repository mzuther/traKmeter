/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2020 Martin Zuther (http://www.mzuther.de/)

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
    int targetRecordingLevel)

{
    updateSkin(numberOfChannels,
               targetRecordingLevel);

    return loadFromXml(skinFile, "trakmeter-skin", "1.5");
}


void Skin::updateSkin(int numberOfChannels,
                      int targetRecordingLevel)

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

    currentGroupName_ = currentFallbackName_;

    if (targetRecordingLevel == -10)
    {
        currentGroupName_ += "_10";
    }
    else if (targetRecordingLevel == -20)
    {
        currentGroupName_ += "_20";
    }
    else if (targetRecordingLevel == -15)
    {
        currentGroupName_ += "_15";
    }
    else
    {
        Logger::outputDebugString("[Skin] invalid target recording level");
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
