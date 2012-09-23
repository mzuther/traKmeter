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

#ifndef __PLUGINEDITOR_TRAKMETER_H__
#define __PLUGINEDITOR_TRAKMETER_H__

#include "juce_library_code/juce_header.h"
#include "juce_library_code/JucePluginCharacteristics.h"
#include "plugin_processor.h"
#include "trakmeter.h"
#include "window_about.h"
#include "window_validation.h"


//==============================================================================
/**
*/
class TraKmeterAudioProcessorEditor : public AudioProcessorEditor, public ButtonListener, public SliderListener, public ActionListener
{
public:
    TraKmeterAudioProcessorEditor(TraKmeterAudioProcessor* ownerFilter, int nNumChannels);
    ~TraKmeterAudioProcessorEditor();

    void buttonClicked(Button* button);
    void sliderValueChanged(Slider* slider);

    void actionListenerCallback(const String& message);
    void changeParameter(int nIndex);
    void changeParameter(int nIndex, int nValue);

    //==============================================================================
    // This is just a standard Juce paint method...
    void paint(Graphics& g);
    void resized();

private:
    JUCE_LEAK_DETECTOR(TraKmeterAudioProcessorEditor);

    void reloadMeters();
    void resizeEditor();

    bool bReloadMeters;
    bool bIsValidating;
    int nInputChannels;
    int nStereoInputChannels;
    int nRightColumnStart;
    int nHeight;

    TraKmeterAudioProcessor* pProcessor;
    TraKmeter* trakmeter;

    TextButton* ButtonReset;

    TextButton* ButtonMixMode;
    TextButton* ButtonValidation;
    TextButton* ButtonAbout;

    Slider* SliderGain;

    Label* LabelDebug;
};


#endif  // __PLUGINEDITOR_TRAKMETER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
