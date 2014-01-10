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

#ifndef __METER_SEGMENT_OVERLOAD_H__
#define __METER_SEGMENT_OVERLOAD_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "mz_tools.h"


//==============================================================================
/**
*/
class MeterSegmentOverload : public Component
{
public:
    MeterSegmentOverload(const String& componentName, float fThreshold, float fRange, int nCrestFactor, bool bDiscreteLevels, bool bDisplayPeaks, int nColor);
    ~MeterSegmentOverload();

    void setLevels(float fLevel, float fLevelPeak, float fLevelMaximum);
    void paint(Graphics& g);
    void resized();
    void visibilityChanged();

private:
    JUCE_LEAK_DETECTOR(MeterSegmentOverload);

    float fHue;
    float fBrightness;

    float fLowerThreshold;
    float fUpperThreshold;
    float fThresholdRange;
    int nCrestFactor;

    float fMaximumLevel;
    String strMaximumLevel;

    bool bPeakMarker;
    bool discreteLevels;
    bool displayPeaks;
};


#endif  // __METER_SEGMENT_OVERLOAD_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
