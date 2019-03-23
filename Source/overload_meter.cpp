/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2019 Martin Zuther (http://www.mzuther.de/)

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

#include "overload_meter.h"


OverloadMeter::OverloadMeter(float Threshold)
{
    // threshold, meter segment will be dark below and fully lit above
    // this level
    fThreshold = Threshold;

    // is there an overload in this moment?
    bActiveOverload = false;

    // were any overloads detected?
    bOverloadsDetected = false;

    // initialise maximum level
    fMaximumLevel = -9999.9f;

    // make sure that segment is drawn after initialisation
    setLevels(-9999.9f, -9999.9f);
}


void OverloadMeter::setLevels(float fLevel, float fMaximumLevelNew)
{
    // current level lies below threshold
    if (fLevel < fThreshold)
    {
        bActiveOverload = false;
    }
    else
    {
        bActiveOverload = true;
    }

    if (fMaximumLevel != fMaximumLevelNew)
    {
        fMaximumLevel = fMaximumLevelNew;
        String strMaximumLevel;

        if (fMaximumLevel >= fThreshold)
        {
            int nMaximumLevel = frut::math::SimpleMath::round(fMaximumLevel);

            if (nMaximumLevel > 0)
            {
                strMaximumLevel = "+" + String(nMaximumLevel);
            }
            else
            {
                strMaximumLevel = String(nMaximumLevel);
            }

            bOverloadsDetected = true;
        }
        else
        {
            strMaximumLevel = String();
            bOverloadsDetected = false;
        }

        setLabelText(String(strMaximumLevel));
    }

    if (bOverloadsDetected)
    {
        if (bActiveOverload)
        {
            setState(frut::widgets::StateLabel::State::active);
        }
        else
        {
            setState(frut::widgets::StateLabel::State::on);
        }
    }
    else
    {
        setState(frut::widgets::StateLabel::State::off);
    }
}
