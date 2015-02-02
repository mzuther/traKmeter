/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2015 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __SIGNAL_METER_H__
#define __SIGNAL_METER_H__

#include "JuceHeader.h"
#include "trakmeter.h"
#include "meter_signal_led.h"
#include "plugin_processor.h"


//==============================================================================
/**
*/
class SignalMeter : public Component
{
public:
    SignalMeter(const String &componentName, int PosX, int PosY, int width, int CrestFactor, int nNumChannels);
    ~SignalMeter();

    void setLevels(MeterBallistics *pMeterBallistics);
    void paint(Graphics &g);
    int getPreferredHeight();
    void resized();
    void visibilityChanged();

private:
    JUCE_LEAK_DETECTOR(SignalMeter);

    int nPosX;
    int nPosY;
    int nHeight;
    int nWidth;

    int nSegmentHeight;
    int nMeterHeight;

    int nCrestFactor;
    int nInputChannels;

    MeterSignalLed **PeakMeterSignals;

    void drawMarkers(Graphics &g, String &strMarker, int x, int y, int width, int height);
};


#endif  // __SIGNAL_METER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
