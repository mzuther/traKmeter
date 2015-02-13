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

#ifndef __AVERAGE_METER_H__
#define __AVERAGE_METER_H__

#include "JuceHeader.h"
#include "meter_bar_average.h"
#include "mz_tools.h"
#include "trakmeter.h"
#include "plugin_processor.h"


//==============================================================================
/**
*/
class AverageMeter : public Component
{
public:
    AverageMeter(const String &componentName, int PosX, int PosY, int width, int nCrestFactor, int nNumChannels, int nSegmentHeight);
    ~AverageMeter();

    void setLevels(MeterBallistics *pMeterBallistics);
    int getPreferredHeight();
    void resized();
    void visibilityChanged();

private:
    JUCE_LEAK_DETECTOR(AverageMeter);

    int nPosX;
    int nPosY;
    int nHeight;
    int nWidth;

    int nInputChannels;

    MeterBarAverage **LevelMeters;
};


#endif  // __AVERAGE_METER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
