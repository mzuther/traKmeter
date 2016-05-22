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

#include "window_validation_content.h"


/// Create content component for dialog window with validation settings.
///
/// @param processor audio plug-in processor
///
/// ### Exit values
///
/// | %Value | %Result                                        |
/// | :----: | ---------------------------------------------- |
/// | 0      | window has been closed "by force" or by using the "Cancel" button |
/// | 1      | window has been closed to start validation     |
///
WindowValidationContent::WindowValidationContent(
    TraKmeterAudioProcessor *processor)

{
    // dimensions of content component
    int componentWidth = 170;
    int componentHeight = 190;

    // store handle to audio plug-in processor (used for getting and
    // setting plug-in parameters)
    audioProcessor = processor;

    // get current number of audio input channels
    int numberOfInputChannels = audioProcessor->getMainBusNumInputChannels();

    // get current audio sample rate
    int sampleRate = static_cast<int>(audioProcessor->getSampleRate());

    // get current audio channel used for validation
    int selectedChannel = audioProcessor->getRealInteger(
                              TraKmeterPluginParameters::selValidationSelectedChannel);

    // get current audio file used for validation
    File validationFile = audioProcessor->getParameterValidationFile();

    // initialise parent content component
    initialise(componentWidth,
               componentHeight,
               numberOfInputChannels,
               sampleRate,
               selectedChannel,
               validationFile);
}


/// Static helper function to create a dialog window for validation
/// settings.
///
/// @param pluginEditor audio plug-in editor
///
/// @param audioProcessor audio processor
///
/// @return created dialog window
///
DialogWindow *WindowValidationContent::createDialogWindow(
    AudioProcessorEditor *pluginEditor,
    TraKmeterAudioProcessor *audioProcessor)

{
    // prepare dialog window
    DialogWindow::LaunchOptions windowValidationLauncher;

    // create content component
    WindowValidationContent *contentComponent =
        new WindowValidationContent(audioProcessor);

    // initialise dialog window settings
    windowValidationLauncher.dialogTitle = String("Validation");
    windowValidationLauncher.dialogBackgroundColour = Colours::white;
    windowValidationLauncher.content.setOwned(contentComponent);
    windowValidationLauncher.componentToCentreAround = pluginEditor;

    windowValidationLauncher.escapeKeyTriggersCloseButton = true;
    windowValidationLauncher.useNativeTitleBar = false;
    windowValidationLauncher.resizable = false;
    windowValidationLauncher.useBottomRightCornerResizer = false;

    // launch dialog window
    DialogWindow *windowValidation = windowValidationLauncher.launchAsync();
    windowValidation->setAlwaysOnTop(true);

    return windowValidation;
}


/// Initialise dialog window components.
///
/// @param componentWidth width of content component
///
/// @param componentHeight height of content component
///
/// @param numberOfInputChannels current number of audio input
///        channels
///
/// @param sampleRate current audio sample rate
///
/// @param selectedChannel current audio channel used for validation
///        (starting at 0; -1 designates all channels)
///
/// @param validationFile current audio file used for validation
///
void WindowValidationContent::initialise(
    int componentWidth,
    int componentHeight,
    int numberOfInputChannels,
    int sampleRate,
    int selectedChannel,
    const File &validationFile)

{
    // call method of super class
    frut::widget::WindowValidationContent::initialise(
        componentWidth,
        componentHeight,
        numberOfInputChannels,
        sampleRate,
        selectedChannel,
        validationFile);


    // initialise button for selecting the validation output format
    buttonDumpCSV_.setButtonText("CSV format");

    buttonDumpCSV_.setToggleState(
        audioProcessor->getBoolean(
            TraKmeterPluginParameters::selValidationCSVFormat),
        dontSendNotification);

    addAndMakeVisible(buttonDumpCSV_);


    // initialise channel selection slider
    sliderSelectChannel_.setValue(
        audioProcessor->getRealInteger(
            TraKmeterPluginParameters::selValidationSelectedChannel),
        dontSendNotification);

    addAndMakeVisible(sliderSelectChannel_);


    // initialise button for average level logging
    buttonDumpAverageLevel_.setButtonText("Average meter level");

    buttonDumpAverageLevel_.setToggleState(
        audioProcessor->getBoolean(
            TraKmeterPluginParameters::selValidationAverageMeterLevel),
        dontSendNotification);

    addAndMakeVisible(buttonDumpAverageLevel_);


    // initialise button for peak level logging
    buttonDumpPeakLevel_.setButtonText("Peak meter level");

    buttonDumpPeakLevel_.setToggleState(
        audioProcessor->getBoolean(
            TraKmeterPluginParameters::selValidationPeakMeterLevel),
        dontSendNotification);

    addAndMakeVisible(buttonDumpPeakLevel_);


    // style and place the dialog window's components
    applySkin();
}


/// Style and place the dialog window's components.
///
void WindowValidationContent::applySkin()
{
    // call method of super class
    frut::widget::WindowValidationContent::applySkin();

    // style button for selecting the validation output format
    buttonDumpCSV_.setColour(
        ToggleButton::textColourId,
        Colours::black);

    // style button for average level logging
    buttonDumpAverageLevel_.setColour(
        ToggleButton::textColourId,
        Colours::black);

    // style button for peak level logging
    buttonDumpPeakLevel_.setColour(
        ToggleButton::textColourId,
        Colours::black);


    // place the components
    int positionX = 5;
    int positionY = 85;

    buttonDumpPeakLevel_.setBounds(positionX, positionY, 180, 20);

    positionY += 20;
    buttonDumpAverageLevel_.setBounds(positionX, positionY, 180, 20);

    positionY += 20;
    buttonDumpCSV_.setBounds(positionX, positionY, 180, 20);

    positionY += 31;
    buttonValidation_.setBounds(18, positionY, 60, 20);
    buttonCancel_.setBounds(88, positionY, 60, 20);
}


/// Called when a button is clicked.  The "Cancel" and file selection
/// buttons are already handled here.  Override this method to handle
/// other buttons.
///
/// @param button clicked button
///
void WindowValidationContent::buttonClicked(Button *button)
{
    // user wants to validate the meters
    if (button == &buttonValidation_)
    {
        // file name has not been set
        if (validationFile_ == File::nonexistent)
        {
            DBG("[traKmeter] file name for validation not set.");

            // prevent closing of parent dialog window
            return;
        }

        // get selected audio channel (internal value) and update
        // parameter
        float selectedChannelInternal = sliderSelectChannel_.getFloat();

        audioProcessor->setParameter(
            TraKmeterPluginParameters::selValidationSelectedChannel,
            selectedChannelInternal);

        // get selected audio channel (real value; channel numbers
        // start at 0; -1 designates all channels)
        int selectedChannel = static_cast<int>(
                                  sliderSelectChannel_.getValue());

        // get selected output format and update parameter
        bool reportCSV = buttonDumpCSV_.getToggleState();
        audioProcessor->setParameter(
            TraKmeterPluginParameters::selValidationCSVFormat,
            reportCSV ? 1.0f : 0.0f);

        // get average level log setting and update parameter
        bool logAverageLevel = buttonDumpAverageLevel_.getToggleState();
        audioProcessor->setParameter(
            TraKmeterPluginParameters::selValidationAverageMeterLevel,
            logAverageLevel ? 1.0f : 0.0f);

        // get peak level log setting and update parameter
        bool logPeakLevel = buttonDumpPeakLevel_.getToggleState();
        audioProcessor->setParameter(
            TraKmeterPluginParameters::selValidationPeakMeterLevel,
            logPeakLevel ? 1.0f : 0.0f);

        // validation file has already been initialised
        audioProcessor->startValidation(
            validationFile_, selectedChannel, reportCSV,
            logAverageLevel, logPeakLevel);

        // get parent dialog window
        DialogWindow *dialogWindow = findParentComponentOfClass<DialogWindow>();

        if (dialogWindow != nullptr)
        {
            // close dialog window (exit value 1)
            dialogWindow->exitModalState(1);
        }
    }
    // otherwise, use handling of super class
    else
    {
        frut::widget::WindowValidationContent::buttonClicked(button);
    }
}


/// Select a new audio file for validation.
///
/// @param validationFile audio file for validation
///
void WindowValidationContent::selectValidationFile(const File &validationFile)
{
    // set audio file used for validation
    validationFile_ = validationFile;

    // update plug-in parameter
    audioProcessor->setParameterValidationFile(validationFile_);

    // update label that displays the name of the validation file
    labelFileSelection_.setText(
        validationFile_.getFileName(),
        dontSendNotification);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
