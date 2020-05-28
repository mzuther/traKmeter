/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2020 Martin Zuther (http://www.mzuther.de/)

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

#ifndef TRAKMETER_PLUGIN_EDITOR_H
#define TRAKMETER_PLUGIN_EDITOR_H

#include "FrutHeader.h"

#include "plugin_parameters.h"
#include "plugin_processor.h"
#include "skin.h"
#include "trakmeter.h"
#include "window_validation_content.h"


class TraKmeterAudioProcessorEditor :
    public AudioProcessorEditor,
    public Button::Listener,
    public ActionListener
{
public:
    TraKmeterAudioProcessorEditor(TraKmeterAudioProcessor &processor,
                                  int nNumChannels);
    ~TraKmeterAudioProcessorEditor();

    void buttonClicked(Button *button) override;

    void actionListenerCallback(const String &strMessage) override;
    void updateParameter(int nIndex);

    void windowAboutCallback(int modalResult);
    void windowSkinCallback(int modalResult);
    void windowValidationCallback(int modalResult);

    virtual void paint(Graphics &g) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TraKmeterAudioProcessorEditor);

    void reloadMeters();
    void applySkin();
    void loadSkin();

    bool needsMeterReload;
    bool isValidating;
    bool validationDialogOpen;
    bool isInitialising;

    int numberOfInputChannels;

    int segmentHeight;

    TraKmeterAudioProcessor &audioProcessor;

    frut::skin::LookAndFeel_Frut_V3 customLookAndFeel_;
    std::unique_ptr<TraKmeter> trakmeter_;

    File skinDirectory;
    Skin skin;
    String currentSkinName;

    DrawableButton ButtonReset;

    DrawableButton ButtonRecordingLevel_10;
    DrawableButton ButtonRecordingLevel_15;
    DrawableButton ButtonRecordingLevel_20;

    DrawableButton ButtonSkin;
    DrawableButton ButtonValidation;
    DrawableButton ButtonAbout;

#ifdef DEBUG
    DrawableComposite LabelDebug;
#endif // DEBUG

    DrawableComposite DrawableBackground;
};

#endif  // TRAKMETER_PLUGIN_EDITOR_H
