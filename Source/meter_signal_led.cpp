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

#include "meter_signal_led.h"


MeterSignalLed::MeterSignalLed(const String &componentName, String &label, float hue)
{
    // set component name
    setName(componentName);

    // set label
    strLabel = label;

    // meter segment is blue
    fHue = hue;

    // initialise meter segment's brightness (0.0f is dark, 1.0f is
    // fully lit)
    fBrightness = 0.0f;
}


MeterSignalLed::~MeterSignalLed()
{
    // nothing to do, really
}


void MeterSignalLed::paint(Graphics &g)
{
    // get meter segment's screen dimensions
    int width = getWidth();
    int height = getHeight();

    // initialise meter segment's colour from hue and brightness
    g.setColour(Colour(fHue, 0.9f, fBrightness, 1.0f));

    // fill meter segment with solid colour, but leave a border of one
    // pixel for peak marker
    g.fillRect(1, 1, width - 2, height - 2);

    // initialise colour for label (light colour for levels below the
    // threshold and black for all other levels)
    if (fBrightness <= 0.32f)
    {
        g.setColour(Colour(fHue, 0.4f, 0.75f, 1.0f));
    }
    else
    {
        g.setColour(Colours::black);
    }

    // draw label
    g.setFont(12.0f);
    g.drawFittedText(strLabel, 0, 1, width, 10, Justification::centred, 1, 1.0f);

    // draw a black rectangle around meter segment (width: 1 pixel)
    g.setColour(Colours::black);
    g.drawRect(0, 0, width, height);
}


void MeterSignalLed::visibilityChanged()
{
    // if this function did not exist, the meter segment wouldn't be
    // drawn until the first level change!
}


void MeterSignalLed::resized()
{
}


void MeterSignalLed::setLevel(float fLevel)
{
    // store old brightness and peak marker values
    float fBrightnessOld = fBrightness;

    // calculate brightness from meter readout
    fBrightness = fLevel * 0.65f + 0.32f;

    // re-paint meter segment only when the brightness has changed
    if (fBrightness != fBrightnessOld)
    {
        repaint(getLocalBounds());
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
