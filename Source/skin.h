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

#ifndef __SKIN_H__
#define __SKIN_H__

#include "FrutHeader.h"
#include "plugin_parameters.h"


class Skin :
    public frut::GenericSkin
{
public:
    void loadSkin(File &skinFile,
                  int numberOfChannels,
                  int crestFactor,
                  int meterType);

    void updateSkin(int numberOfChannels,
                    int crestFactor,
                    int meterType);

private:
    JUCE_LEAK_DETECTOR(Skin);
};

#endif   // __SKIN_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
