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

#ifndef __METER_SEGMENT_MULTI_H__
#define __METER_SEGMENT_MULTI_H__

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class MeterSegmentMulti : public Component
{
public:
    MeterSegmentMulti(const String& componentName, float fCrestFactor, bool bIsAverageLevel, bool bDisplayPeaks);
    ~MeterSegmentMulti();

    void setLevels(float fLevel, float fLevelPeak);
    void paint(Graphics& g);
    void resized();
    void visibilityChanged();

private:
    JUCE_LEAK_DETECTOR(MeterSegmentMulti);

    float fHue;
    float fBrightness;

    float fHuePeak;
    float fBrightnessPeak;

    float fThreshold1;
    float fThreshold2;
    float fThreshold3;
    float fThreshold4;

    float fSignalThreshold;
    float fSignalRange;

    bool displayPeaks;
};


#endif  // __METER_SEGMENT_MULTI_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
