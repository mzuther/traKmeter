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

// build stand-alone application for plugin
#ifdef TRAKMETER_STAND_ALONE

#include "standalone_application.h"

START_JUCE_APPLICATION(StandaloneApplication)

#endif

// build VST plugin
#ifdef TRAKMETER_VST_PLUGIN

#include "juce/src/audio/plugin_client/VST/juce_VST_Wrapper.cpp"

#endif


// Local Variables:
// ispell-local-dictionary: "british"
// End: