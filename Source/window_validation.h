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

#ifndef __WINDOW_VALIDATION_H__
#define __WINDOW_VALIDATION_H__

#include "JuceHeader.h"
#include "plugin_processor.h"
#include "common/widgets/generic_channel_slider.h"


class WindowValidation : public DocumentWindow, ButtonListener
{
public:
    WindowValidation(Component *pEditorWindow, TraKmeterAudioProcessor *processor);
    ~WindowValidation();

    void buttonClicked(Button *button);
    void closeButtonPressed();

private:
    JUCE_LEAK_DETECTOR(WindowValidation);

    TraKmeterAudioProcessor *pProcessor;
    File fileValidation;

    Component contentComponent;

    Label LabelFileSelection;
    Label LabelSampleRate;
    Label LabelSampleRateValue;

    TextButton ButtonFileSelection;
    TextButton ButtonValidation;
    TextButton ButtonCancel;

    Label LabelDumpSelectedChannel;
    GenericChannelSlider SliderDumpSelectedChannel;
    ToggleButton ButtonDumpCSV;
    ToggleButton ButtonDumpAverageMeterLevel;
    ToggleButton ButtonDumpPeakMeterLevel;
};


#endif  // __WINDOW_VALIDATION_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
