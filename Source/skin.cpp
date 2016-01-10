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

#include "skin.h"


Skin::Skin()
{
}


void Skin::loadSkin(File &fileSkin, int nNumChannels, int nCrestFactor, int nMeterType)
{
    updateSkin(nNumChannels, nCrestFactor, nMeterType);
    loadFromXml(fileSkin, "trakmeter-skin");
}


void Skin::updateSkin(int nNumChannels, int nCrestFactor, int nMeterType)
{
    jassert(nNumChannels > 0);
    nNumberOfChannels = nNumChannels;

    strBackgroundSelector = "image";

    if (nNumberOfChannels <= 2)
    {
        strSkinFallback_1 = "stereo";
    }
    else
    {
        strSkinFallback_1 = "multi";
    }

    if (nMeterType == TraKmeterPluginParameters::selSplitMeters)
    {
        strSkinFallback_1 += "_split";
    }
    else
    {
        strSkinFallback_1 += "_combined";
    }

    switch (nCrestFactor)
    {
    case 20:
        strSkinGroup = strSkinFallback_1 + "_k20";
        break;

    default:
        strSkinGroup = strSkinFallback_1 + "_normal";
        break;
    }

    if (xml != nullptr)
    {
        xmlSkinGroup = xml->getChildByName(strSkinGroup);
        xmlSkinFallback_1 = xml->getChildByName(strSkinFallback_1);
        xmlSkinFallback_2 = xml->getChildByName("default");
    }
    else
    {
        xmlSkinGroup = nullptr;
        xmlSkinFallback_1 = nullptr;
        xmlSkinFallback_2 = nullptr;
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
