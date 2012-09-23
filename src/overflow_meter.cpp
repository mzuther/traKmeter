/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012 Martin Zuther (http://www.mzuther.de/)

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

#include "overflow_meter.h"

OverflowMeter::OverflowMeter(const String& componentName) : Label(componentName, "0")
{
    nOverflows = 0;

    // this component does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    setFont(12.0f);
    setJustificationType(Justification::centredRight);
    setColour(Label::backgroundColourId, Colours::grey.darker(0.7f));
    setColour(Label::textColourId, Colours::white);
    setColour(Label::outlineColourId, Colours::grey.darker(0.2f));
}

OverflowMeter::~OverflowMeter()
{
}

void OverflowMeter::setOverflows(int Overflows)
{
    int nOverflowsOld = nOverflows;
    nOverflows = Overflows;

    if (nOverflows != nOverflowsOld)
    {
        setText(String(nOverflows), false);

        if (nOverflowsOld == 0)
        {
            setColour(Label::backgroundColourId, Colours::red.darker(0.2f));
        }
        else if (nOverflows == 0)
        {
            setColour(Label::backgroundColourId, Colours::grey.darker(0.7f));
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End: