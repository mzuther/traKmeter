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

#include "meter_segment_multi.h"


MeterSegmentMulti::MeterSegmentMulti(const String& componentName, float fCrestFactor, bool bIsAverageLevel, bool bDisplayPeaks)
{
    // set component name
    setName(componentName);

    // display peaks on meter?
    displayPeaks = bDisplayPeaks;

    // setup internal level thresholds (according to peak or average
    // level mode)
    if (bIsAverageLevel)
    {
        fThreshold1 = fCrestFactor - 17.0f;
        fThreshold2 = fThreshold1 - 1.0f;
        fThreshold3 = fThreshold2 - 4.0f;
        fThreshold4 = fThreshold3 - 4.0f;
    }
    else
    {
        fThreshold1 = fCrestFactor - 9.0f;
        fThreshold2 = fThreshold1 - 1.0f;
        fThreshold3 = fThreshold2 - 6.0f;
        fThreshold4 = fThreshold3 - 4.0f;
    }

    // signals: detected at -60 dB FS and above
    fSignalThreshold = fCrestFactor - 60.0f;
    fSignalRange = fThreshold4 - fSignalThreshold;

    // initialise meter's brightness (0.0f is dark, 1.0f is fully lit)
    fBrightness = 0.00f;
    fBrightnessPeak = 0.00f;

    // initialise meter's hue
    fHue = 0.00f;
    fHuePeak = 0.00f;

    // make sure that segment is drawn after initialisation
    setLevels(-9999.9f, -9999.9f);
}


MeterSegmentMulti::~MeterSegmentMulti()
{
    // nothing to do, really
}


void MeterSegmentMulti::paint(Graphics& g)
{
    // get meter segment's screen dimensions
    int width = getWidth();
    int height = getHeight();

    // initialise meter segment's colour from hue and brightness
    g.setColour(Colour(fHue, 1.0f, fBrightness, 1.0f));

    // fill meter segment with solid colour, but leave a border of one
    // pixel for peak marker
    g.fillRect(1, 1, width - 2, height - 2);

    // darken space between border and meter segment (width: 1 pixel)
    if (fBrightness > 0.2f)
    {
        float fBrightnessSpace = (fBrightness - 0.20f) / 2.0f;
        g.setColour(Colour(0.00f, 1.0f, 0.00f, fBrightnessSpace));
        g.drawRect(1, 1, width - 2, height - 2);
    }

    // if peak marker is lit, draw a rectangle around meter segment
    // (width: 1 pixel)
    if (displayPeaks)
    {
        g.setColour(Colour(fHuePeak, 1.0f, fBrightnessPeak, 1.0f));
    }
    else
    {
        g.setColour(Colours::black);
    }

    g.drawRect(0, 0, width, height);
}


void MeterSegmentMulti::visibilityChanged()
{
    // if this function did not exist, the meter segment wouldn't be
    // drawn until the first level change!
}


void MeterSegmentMulti::resized()
{
}


void MeterSegmentMulti::setLevels(float fLevel, float fLevelPeak)
{
    // store old brightness values
    float fBrightnessOld = fBrightness;
    float fBrightnessPeakOld = fBrightnessPeak;

    // store old hue values
    float fHueOld = fHue;
    float fHuePeakOld = fHuePeak;

    // set meter segment's hue and brightness from current level
    if (fLevel > fThreshold1)
    {
        // meter segment is red
        fHue = 0.00f;
        fBrightness = 1.00f;
    }
    else if (fLevel > fThreshold2)
    {
        // meter segment is yellow
        fHue = 0.18f;
        fBrightness = 1.00f;
    }
    else if (fLevel > fThreshold3)
    {
        // meter segment is green
        fHue = 0.30f;
        fBrightness = 1.00f;
    }
    else if (fLevel > fThreshold4)
    {
        // meter segment is blue
        fHue = 0.58f;
        fBrightness = 1.00f;
    }
    else if (fLevel > fSignalThreshold)
    {
        // meter segment is blue
        fHue = 0.58f;

        // calculate brightness from current level
        fBrightness = (fLevel - fSignalThreshold) / fSignalRange;

        // to look well, meter segments should be left with some
        // colour and not have the same brightness as a fully lit
        // meter segment
        fBrightness = fBrightness * 0.50f + 0.20f;
    }
    else
    {
        // meter segment is dark
        fHue = 0.58f;
        fBrightness = 0.20f;
    }

    // set peak mark's hue and brightness from current peak level
    if (fLevelPeak > fThreshold1)
    {
        // peak mark is red
        fHuePeak = 0.00f;
        fBrightnessPeak = 1.00f;
    }
    else if (fLevelPeak > fThreshold2)
    {
        // peak mark is yellow
        fHuePeak = 0.18f;
        fBrightnessPeak = 1.00f;
    }
    else if (fLevelPeak > fThreshold3)
    {
        // peak mark is green
        fHuePeak = 0.30f;
        fBrightnessPeak = 1.00f;
    }
    else if (fLevelPeak > fThreshold4)
    {
        // peak mark is blue
        fHuePeak = 0.58f;
        fBrightnessPeak = 1.00f;
    }
    else
    {
        // peak mark is dark
        fHuePeak = 0.58f;
        fBrightnessPeak = 0.00f;
    }

    // re-paint meter segment only when brightness, hue or peak marker
    // have changed
    if ((fBrightness != fBrightnessOld) || (fHue != fHueOld) || (fBrightnessPeak != fBrightnessPeakOld) || (fHuePeak != fHuePeakOld))
    {
        repaint(getLocalBounds());
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
