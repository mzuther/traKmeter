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

#ifndef __WINDOW_VALIDATION_CONTENT_H__
#define __WINDOW_VALIDATION_CONTENT_H__

#include "FrutHeader.h"
#include "plugin_processor.h"


/// Customized dialog window for validation settings.
///
class WindowValidationContent :
    public frut::GenericWindowValidationContent
{
public:
    WindowValidationContent(TraKmeterAudioProcessor *processor);

    static DialogWindow *createDialogWindow(
        AudioProcessorEditor *pluginEditor,
        TraKmeterAudioProcessor *audioProcessor);

    virtual void buttonClicked(Button *button);
    virtual void applySkin();

    virtual void initialise(int componentWidth,
                            int componentHeight,
                            int numberOfInputChannels,
                            int sampleRate,
                            int selectedChannel,
                            const File &validationFile);

    virtual void selectValidationFile(const File &validationFile);

private:
    JUCE_LEAK_DETECTOR(WindowValidationContent);

    TraKmeterAudioProcessor *audioProcessor;

    ToggleButton buttonDumpCSV_;
    ToggleButton buttonDumpAverageLevel_;
    ToggleButton buttonDumpPeakLevel_;
};


#endif  // __WINDOW_VALIDATION_CONTENT_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
