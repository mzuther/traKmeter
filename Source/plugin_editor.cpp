/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2018 Martin Zuther (http://www.mzuther.de/)

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

#include "plugin_editor.h"


static void window_about_callback(
    int modalResult,
    TraKmeterAudioProcessorEditor *pEditor)
{
    if (pEditor != nullptr)
    {
        pEditor->windowAboutCallback(modalResult);
    }
}


static void window_skin_callback(
    int modalResult,
    TraKmeterAudioProcessorEditor *pEditor)
{
    if (pEditor != nullptr)
    {
        pEditor->windowSkinCallback(modalResult);
    }
}


static void window_validation_callback(
    int modalResult,
    TraKmeterAudioProcessorEditor *pEditor)
{
    if (pEditor != nullptr)
    {
        pEditor->windowValidationCallback(modalResult);
    }
}


TraKmeterAudioProcessorEditor::TraKmeterAudioProcessorEditor(
    TraKmeterAudioProcessor *ownerFilter,
    int nNumChannels) :
    AudioProcessorEditor(ownerFilter)
{
    // load look and feel
    currentLookAndFeel_ = new frut::skin::LookAndFeel_Frut_V3;
    setLookAndFeel(currentLookAndFeel_);

    // the editor window does not have any transparent areas
    // (increases performance on redrawing)
    setOpaque(true);

    // prevent meter reload during initialisation
    isInitialising = true;

    isValidating = false;
    validationDialogOpen = false;

    numberOfInputChannels = nNumChannels;
    segmentHeight = 10;

    // The plug-in editor's size as well as the location of buttons
    // and labels will be set later on in this constructor.

    audioProcessor = ownerFilter;
    audioProcessor->addActionListener(this);

    ButtonRecordingLevel_10.setRadioGroupId(1);
    ButtonRecordingLevel_10.addListener(this);
    addAndMakeVisible(ButtonRecordingLevel_10);

    ButtonRecordingLevel_15.setRadioGroupId(1);
    ButtonRecordingLevel_15.addListener(this);
    addAndMakeVisible(ButtonRecordingLevel_15);

    ButtonRecordingLevel_20.setRadioGroupId(1);
    ButtonRecordingLevel_20.addListener(this);
    addAndMakeVisible(ButtonRecordingLevel_20);

    ButtonReset.addListener(this);
    addAndMakeVisible(ButtonReset);

    ButtonSkin.addListener(this);
    addAndMakeVisible(ButtonSkin);

    ButtonValidation.addListener(this);
    addAndMakeVisible(ButtonValidation);

    ButtonAbout.addListener(this);
    addAndMakeVisible(ButtonAbout);

#ifdef DEBUG
    // moves debug label to the back of the editor's z-plane to that
    // it doesn't overlay (and thus block) any other components
    addAndMakeVisible(LabelDebug, 0);
#endif

    // prevent unnecessary redrawing of plugin editor
    BackgroundImage.setOpaque(true);
    // moves background image to the back of the editor's z-plane to
    // that it doesn't overlay (and thus block) any other components
    addAndMakeVisible(BackgroundImage, 0);

    updateParameter(TraKmeterPluginParameters::selTargetRecordingLevel);

    // locate directory containing the skins
    skinDirectory = TraKmeterPluginParameters::getSkinDirectory();

    // force meter reload after initialisation ...
    isInitialising = false;

    // apply skin to plug-in editor
    currentSkinName = audioProcessor->getParameterSkinName();
    loadSkin();
}


TraKmeterAudioProcessorEditor::~TraKmeterAudioProcessorEditor()
{
    audioProcessor->removeActionListener(this);
}


void TraKmeterAudioProcessorEditor::loadSkin()
{
    File fileSkin = skinDirectory.getChildFile(currentSkinName + ".skin");

    if (!fileSkin.existsAsFile())
    {
        Logger::outputDebugString("[Skin] file \"" + fileSkin.getFileName() + "\" not found");

        currentSkinName = "Default";
        fileSkin = skinDirectory.getChildFile(currentSkinName + ".skin");
    }

    audioProcessor->setParameterSkinName(currentSkinName);

    int recordingLevel = audioProcessor->getRealInteger(
                             TraKmeterPluginParameters::selTargetRecordingLevel);
    skin.loadSkin(fileSkin,
                  numberOfInputChannels,
                  recordingLevel);

    // will also apply skin to plug-in editor
    needsMeterReload = true;
    reloadMeters();
}


void TraKmeterAudioProcessorEditor::applySkin()
{
    // prevent skin application during meter initialisation
    if (isInitialising)
    {
        return;
    }

    // update skin
    int recordingLevel = audioProcessor->getRealInteger(
                             TraKmeterPluginParameters::selTargetRecordingLevel);
    skin.updateSkin(numberOfInputChannels,
                    recordingLevel);

    // moves background image to the back of the editor's z-plane;
    // will also resize plug-in editor
    skin.setBackgroundImage(&BackgroundImage,
                            this);

    skin.placeAndSkinButton("button_recording_level_10",
                            &ButtonRecordingLevel_10);
    skin.placeAndSkinButton("button_recording_level_15",
                            &ButtonRecordingLevel_15);
    skin.placeAndSkinButton("button_recording_level_20",
                            &ButtonRecordingLevel_20);

    skin.placeAndSkinButton("button_reset",
                            &ButtonReset);
    skin.placeAndSkinButton("button_skin",
                            &ButtonSkin);

    skin.placeAndSkinButton("button_validate",
                            &ButtonValidation);
    skin.placeAndSkinButton("button_about",
                            &ButtonAbout);

#ifdef DEBUG
    skin.placeAndSkinLabel("label_debug",
                           &LabelDebug);
#endif

    if (trakmeter)
    {
        trakmeter->applySkin(&skin);
    }
}


void TraKmeterAudioProcessorEditor::windowAboutCallback(
    int modalResult)
{
    ignoreUnused(modalResult);

    // manually deactivate about button
    ButtonAbout.setToggleState(false, dontSendNotification);
}


void TraKmeterAudioProcessorEditor::windowSkinCallback(
    int modalResult)
{
    // manually deactivate skin button
    ButtonSkin.setToggleState(false, dontSendNotification);

    // user has selected a skin
    if (modalResult > 0)
    {
        // apply skin to plug-in editor
        loadSkin();
    }
}


void TraKmeterAudioProcessorEditor::windowValidationCallback(
    int modalResult)
{
    ignoreUnused(modalResult);

    audioProcessor->silenceInput(false);
    validationDialogOpen = false;

    // manually set button according to validation state
    ButtonValidation.setToggleState(isValidating, dontSendNotification);
}


void TraKmeterAudioProcessorEditor::actionListenerCallback(
    const String &strMessage)
{
    // "PC" ==> parameter changed, followed by a hash and the
    // parameter's ID
    if (strMessage.startsWith("PC#"))
    {
        String strIndex = strMessage.substring(3);
        int nIndex = strIndex.getIntValue();
        jassert(nIndex >= 0);
        jassert(nIndex < audioProcessor->getNumParameters());

        if (audioProcessor->hasChanged(nIndex))
        {
            updateParameter(nIndex);
        }
    }
    // "UM" ==> update meters
    else if (!strMessage.compare("UM"))
    {
        MeterBallistics *pMeterBallistics = audioProcessor->getLevels();

        if (pMeterBallistics)
        {
            trakmeter->setLevels(pMeterBallistics);
        }

        if (isValidating && !audioProcessor->isValidating())
        {
            isValidating = false;
        }
    }
    // "V+" ==> validation started
    else if ((!strMessage.compare("V+")) && audioProcessor->isValidating())
    {
        isValidating = true;
    }
    // "V-" ==> validation stopped
    else if (!strMessage.compare("V-"))
    {
        if (!validationDialogOpen)
        {
            ButtonValidation.setToggleState(false, dontSendNotification);
        }

        // do nothing till you hear from me... :)
    }
    else
    {
        DBG("[traKmeter] Received unknown action message \"" + strMessage + "\".");
    }
}


void TraKmeterAudioProcessorEditor::updateParameter(
    int nIndex)
{
    audioProcessor->clearChangeFlag(nIndex);

    switch (nIndex)
    {
    case TraKmeterPluginParameters::selTargetRecordingLevel:
    {
        int recordingLevel = audioProcessor->getRealInteger(
                                 TraKmeterPluginParameters::selTargetRecordingLevel);

        if (recordingLevel == -10)
        {
            ButtonRecordingLevel_10.setToggleState(true, dontSendNotification);

            // will also apply skin to plug-in editor
            needsMeterReload = true;
        }
        else if (recordingLevel == -15)
        {
            ButtonRecordingLevel_15.setToggleState(true, dontSendNotification);

            // will also apply skin to plug-in editor
            needsMeterReload = true;
        }
        else if (recordingLevel == -20)
        {
            ButtonRecordingLevel_20.setToggleState(true, dontSendNotification);

            // will also apply skin to plug-in editor
            needsMeterReload = true;
        }
        else
        {
            DBG("[K-Meter] invalid recording level");
        }
    }
    break;
    }

    // prevent meter reload during initialisation
    if (!isInitialising)
    {
        // will also apply skin to plug-in editor
        reloadMeters();
    }
}


void TraKmeterAudioProcessorEditor::reloadMeters()
{
    if (needsMeterReload)
    {
        needsMeterReload = false;

        if (trakmeter)
        {
            removeChildComponent(trakmeter);
        }

        int targetRecordingLevel = audioProcessor->getRealInteger(
                                       TraKmeterPluginParameters::selTargetRecordingLevel);
        bool discreteMeter = true;
        float autoFadeFactor = 0.0f;

        Array<Colour> levelMeterColours;

        levelMeterColours.add(Colour(0.00f, 1.0f, 1.0f, 1.0f));  // overload
        levelMeterColours.add(Colour(0.18f, 1.0f, 1.0f, 1.0f));  // warning
        levelMeterColours.add(Colour(0.30f, 1.0f, 1.0f, 1.0f));  // fine
        levelMeterColours.add(Colour(0.58f, 1.0f, 1.0f, 1.0f));  // signal

        trakmeter = new TraKmeter(numberOfInputChannels,
                                  segmentHeight,
                                  autoFadeFactor,
                                  discreteMeter,
                                  targetRecordingLevel,
                                  levelMeterColours,
                                  levelMeterColours);

        // moves traKmeter to the back of the editor's z-plane so that
        // it doesn't overlay (and thus block) any other components
        addAndMakeVisible(trakmeter, 0);

        // moves background image to the back of the editor's z-plane
        applySkin();
    }
}


void TraKmeterAudioProcessorEditor::buttonClicked(
    Button *button)
{
    if (button == &ButtonReset)
    {
        MeterBallistics *pMeterBallistics = audioProcessor->getLevels();

        if (pMeterBallistics)
        {
            pMeterBallistics->reset();
        }

        // apply skin to plug-in editor
        loadSkin();
    }
    else if (button == &ButtonRecordingLevel_10)
    {
        audioProcessor->changeParameter(
            TraKmeterPluginParameters::selTargetRecordingLevel,
            TraKmeterPluginParameters::selRecordingLevel_10 /
            float(TraKmeterPluginParameters::nNumRecordingLevels - 1));
    }
    else if (button == &ButtonRecordingLevel_15)
    {
        audioProcessor->changeParameter(
            TraKmeterPluginParameters::selTargetRecordingLevel,
            TraKmeterPluginParameters::selRecordingLevel_15 /
            float(TraKmeterPluginParameters::nNumRecordingLevels - 1));
    }
    else if (button == &ButtonRecordingLevel_20)
    {
        audioProcessor->changeParameter(
            TraKmeterPluginParameters::selTargetRecordingLevel,
            TraKmeterPluginParameters::selRecordingLevel_20 /
            float(TraKmeterPluginParameters::nNumRecordingLevels - 1));
    }
    else if (button == &ButtonSkin)
    {
        // manually activate button (will be deactivated in dialog
        // window callback)
        button->setToggleState(true, dontSendNotification);

        // prepare and launch dialog window
        DialogWindow *windowSkin = frut::widgets::WindowSkinContent::createDialogWindow(
                                       this, &currentSkinName, skinDirectory);

        // attach callback to dialog window
        ModalComponentManager::getInstance()->attachCallback(windowSkin, ModalCallbackFunction::forComponent(window_skin_callback, this));
    }
    else if (button == &ButtonAbout)
    {
        // manually activate button (will be deactivated in dialog
        // window callback)
        button->setToggleState(true, dontSendNotification);

        StringPairArray arrChapters;

        String pluginNameAndVersion = String(ProjectInfo::projectName);
        pluginNameAndVersion += " v";
        pluginNameAndVersion += JucePlugin_VersionString;

#if JucePlugin_Build_AU
        pluginNameAndVersion += " (Audio Unit)";
#endif

#if JucePlugin_Build_LV2
        pluginNameAndVersion += " (LV2)";
#endif

#if JucePlugin_Build_VST
        pluginNameAndVersion += " (VST)";
#endif

#if JucePlugin_Build_VST3
        pluginNameAndVersion += " (VST3)";
#endif

        arrChapters.set(
            pluginNameAndVersion,
            String(JucePlugin_Desc) + ".\n");

        arrChapters.set(
            "Copyright",
            "(c) 2012-2018 Martin Zuther\n");

        arrChapters.set(
            "Contributors",
            L"Filipe Coelho\n"
            L"Bram de Jong\n");

        arrChapters.set(
            "Beta testing",
            L"Aldo D. Sudak\n"
            L"Rickard (Interfearing Sounds)\n");

        arrChapters.set(
            "Thanks",
            L"I want to thank all contributors and beta testers "
            L"and the open source community at large!\n\n"
            L"Thank you for using free software!\n");

        arrChapters.set(
            "Libraries",
#ifdef LINUX
            L"ALSA\n"
#endif
#if JUCE_ASIO
            L"ASIO\n"
#endif
#ifdef LINUX
            L"FreeType\n"
            L"JACK\n"
#endif
            L"JUCE\n"
#if JucePlugin_Build_LV2
            L"LV2\n"
#endif
#ifdef LINUX
            L"POSIX Threads\n"
#endif
#if JucePlugin_Build_VST || JucePlugin_Build_VST3
            L"VST\n"
#endif
#ifdef LINUX
            L"Xlib\n"
            L"Xext\n"
#endif
        );

#if JucePlugin_Build_VST || JucePlugin_Build_VST3
        // display trademarks (but only when necessary)
        arrChapters.set(
            "Trademarks",
            L"VST PlugIn Technology by Steinberg Media Technologies\n");
#endif

#if JUCE_ASIO
        // display trademarks (but only when necessary)
        arrChapters.set(
            "Trademarks",
            L"ASIO Driver Technology by Steinberg Media Technologies\n");
#endif

        arrChapters.set(
            "License",
            L"This program is free software: you can redistribute it "
            L"and/or modify it under the terms of the GNU General "
            L"Public License as published by the Free Software "
            L"Foundation, either version 3 of the License, or (at "
            L"your option) any later version.\n\n"

            L"This program is distributed in the hope that it will "
            L"be useful, but WITHOUT ANY WARRANTY; without even "
            L"the implied warranty of MERCHANTABILITY or FITNESS "
            L"FOR A PARTICULAR PURPOSE.  See the GNU General Public "
            L"License for more details.\n\n"

            L"You should have received a copy of the GNU General "
            L"Public License along with this program.  If not, "
            L"see <http://www.gnu.org/licenses/>.\n\n"

            L"Thank you for using free software!");

        // prepare and launch dialog window
        int width = 270;
        int height = 540;

        DialogWindow *windowAbout = frut::widgets::WindowAboutContent::createDialogWindow(
                                        this, width, height, arrChapters);

        // attach callback to dialog window
        ModalComponentManager::getInstance()->attachCallback(windowAbout, ModalCallbackFunction::forComponent(window_about_callback, this));
    }
    else if (button == &ButtonValidation)
    {
        // manually activate button
        button->setToggleState(true, dontSendNotification);

        validationDialogOpen = true;
        audioProcessor->stopValidation();
        audioProcessor->silenceInput(true);

        // prepare and launch dialog window
        DialogWindow *windowValidation = WindowValidationContent::createDialogWindow(
                                             this, audioProcessor);

        // attach callback to dialog window
        ModalComponentManager::getInstance()->attachCallback(windowValidation, ModalCallbackFunction::forComponent(window_validation_callback, this));
    }
}


void TraKmeterAudioProcessorEditor::resized()
{
}
