/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2013 Martin Zuther (http://www.mzuther.de/)

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

#include "window_validation.h"


WindowValidation::WindowValidation(int nWidth, int nHeight, TraKmeterAudioProcessor* processor)
    : ResizableWindow("Validation traKmeter", false)
    // create new window child of width "nWidth" and height "nHeight"
{
    pProcessor = processor;
    pProcessor->stopValidation();
    fileValidation = pProcessor->getParameterValidationFile();

    // set dimensions to those passed to the function ...
    setBounds(0, 0, nWidth, nHeight);

    // ... and keep the new window on top
    setAlwaysOnTop(true);

    // this window does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    // prohibit movement of the new window
    pConstrainer = new ProhibitingBoundsConstrainer();
    setConstrainer(pConstrainer);

    // empty windows are boring, so let's prepare a space for some
    // window components
    contentComponent = new Component("Window Area");
    setContentOwned(contentComponent, false);

    nEditorWidth = 160;
    nEditorX = nWidth - nEditorWidth - 8;

    LabelFileSelection = new Label("Label FileSelection", fileValidation.getFileName());
    LabelFileSelection->setBounds(nEditorX, nHeight - 169, nEditorWidth - 38, 20);
    LabelFileSelection->setMinimumHorizontalScale(1.0f);
    LabelFileSelection->setColour(Label::textColourId, Colours::white);
    LabelFileSelection->setColour(Label::backgroundColourId, Colours::grey.darker(0.6f));
    LabelFileSelection->setColour(Label::outlineColourId, Colours::black);

    // add and display the label
    contentComponent->addAndMakeVisible(LabelFileSelection);

    ButtonFileSelection = new TextButton("...");
    ButtonFileSelection->setBounds(nEditorX + nEditorWidth - 34, nHeight - 169, 30, 20);

    ButtonFileSelection->addListener(this);
    contentComponent->addAndMakeVisible(ButtonFileSelection);

    LabelSampleRateValue = new Label("Label SampleRate", "Host SR: ");
    LabelSampleRateValue->setBounds(nEditorX, nHeight - 144, 75, 20);
    LabelSampleRateValue->setColour(Label::textColourId, Colours::white);
    contentComponent->addAndMakeVisible(LabelSampleRateValue);

    int nSampleRate = (int) pProcessor->getSampleRate();
    String strSampleRateThousands = String(nSampleRate / 1000);
    String strSampleRateOnes = String(nSampleRate % 1000).paddedLeft('0', 3);
    String strSampleRate = strSampleRateThousands + " " + strSampleRateOnes + " Hz";

    LabelSampleRateValue = new Label("Label SampleRateValue", strSampleRate);
    LabelSampleRateValue->setBounds(nEditorX + 66, nHeight - 144, 82, 20);
    LabelSampleRateValue->setMinimumHorizontalScale(1.0f);
    LabelSampleRateValue->setColour(Label::textColourId, Colours::white);
    LabelSampleRateValue->setColour(Label::backgroundColourId, Colours::grey.darker(0.6f));
    LabelSampleRateValue->setColour(Label::outlineColourId, Colours::black);

    // add and display the label
    contentComponent->addAndMakeVisible(LabelSampleRateValue);

    LabelDumpSelectedChannel = new Label("Selected channel", "Channel: ");
    LabelDumpSelectedChannel->setBounds(nEditorX, nHeight - 119, 75, 20);
    LabelDumpSelectedChannel->setColour(Label::textColourId, Colours::white);
    contentComponent->addAndMakeVisible(LabelDumpSelectedChannel);

    SliderDumpSelectedChannel = new ChannelSlider("Selected channel", pProcessor->getNumChannels() - 1);
    SliderDumpSelectedChannel->setBounds(nEditorX + 66, nHeight - 119, 70, 20);
    SliderDumpSelectedChannel->setColour(ChannelSlider::textBoxTextColourId, Colours::white);
    SliderDumpSelectedChannel->setColour(ChannelSlider::textBoxBackgroundColourId, Colours::grey.darker(0.6f));
    SliderDumpSelectedChannel->setColour(ChannelSlider::textBoxOutlineColourId, Colours::black);

    ButtonDumpCSV = new ToggleButton("CSV");
    ButtonDumpCSV->setBounds(nEditorX, nHeight - 89, 50, 20);
    ButtonDumpCSV->setColour(ToggleButton::textColourId, Colours::white);
    ButtonDumpCSV->setToggleState(pProcessor->getParameterAsBool(TraKmeterPluginParameters::selValidationCSVFormat), dontSendNotification);
    contentComponent->addAndMakeVisible(ButtonDumpCSV);

    SliderDumpSelectedChannel->setValue(pProcessor->getParameterAsInt(TraKmeterPluginParameters::selValidationSelectedChannel), dontSendNotification);
    contentComponent->addAndMakeVisible(SliderDumpSelectedChannel);

    ButtonDumpAverageMeterLevel = new ToggleButton("Avg");
    ButtonDumpAverageMeterLevel->setBounds(nEditorX + 50, nHeight - 89, 50, 20);
    ButtonDumpAverageMeterLevel->setColour(ToggleButton::textColourId, Colours::white);
    ButtonDumpAverageMeterLevel->setToggleState(pProcessor->getParameterAsBool(TraKmeterPluginParameters::selValidationAverageMeterLevel), dontSendNotification);
    contentComponent->addAndMakeVisible(ButtonDumpAverageMeterLevel);

    ButtonDumpPeakMeterLevel = new ToggleButton("Peak");
    ButtonDumpPeakMeterLevel->setBounds(nEditorX + 100, nHeight - 89, 65, 20);
    ButtonDumpPeakMeterLevel->setColour(ToggleButton::textColourId, Colours::white);
    ButtonDumpPeakMeterLevel->setToggleState(pProcessor->getParameterAsBool(TraKmeterPluginParameters::selValidationPeakMeterLevel), dontSendNotification);
    contentComponent->addAndMakeVisible(ButtonDumpPeakMeterLevel);

    // create and position a "validation" button which closes the
    // window and runs the selected audio file when clicked
    ButtonValidation = new TextButton("Validate");
    ButtonValidation->setBounds(nEditorX + 95, nHeight - 59, 60, 20);
    ButtonValidation->setColour(TextButton::textColourOffId, Colours::black);
    ButtonValidation->setColour(TextButton::buttonColourId, Colours::red);
    ButtonValidation->setColour(TextButton::buttonOnColourId, Colours::grey);

    // add "validation" window as button listener and display the
    // button
    ButtonValidation->addListener(this);
    contentComponent->addAndMakeVisible(ButtonValidation);

    // create and position a "validation" button which closes the
    // window when clicked
    ButtonCancel = new TextButton("Cancel");
    ButtonCancel->setBounds(nEditorX + 30, nHeight - 59, 60, 20);
    ButtonCancel->setColour(TextButton::textColourOffId, Colours::black);
    ButtonCancel->setColour(TextButton::buttonColourId, Colours::yellow);
    ButtonCancel->setColour(TextButton::buttonOnColourId, Colours::grey);

    // add "validation" window as button listener and display the button
    ButtonCancel->addListener(this);
    contentComponent->addAndMakeVisible(ButtonCancel);
}


WindowValidation::~WindowValidation()
{
    delete pConstrainer;
    pConstrainer = NULL;

    // delete all children of the window; "contentComponent" will be
    // deleted by the base class, so please leave it alone!
    contentComponent->deleteAllChildren();
}


void WindowValidation::paint(Graphics& g)
{
    int nHeight = getHeight();

    // fill window background with grey colour gradient
    g.setGradientFill(ColourGradient(Colours::darkgrey.darker(0.4f), 0, 0, Colours::darkgrey.darker(1.0f), 0, (float) nHeight, false));
    g.fillAll();

    g.setColour(Colours::white);
    g.setOpacity(0.15f);
    g.drawRect(nEditorX - 1, nHeight - 169, nEditorWidth + 4, 139);

    g.setColour(Colours::white);
    g.setOpacity(0.05f);
    g.fillRect(nEditorX, nHeight - 168, nEditorWidth + 3, 137);
}


void WindowValidation::buttonClicked(Button* button)
{
    // find out which button has been clicked
    if (button == ButtonValidation)
    {
        int nSelectedChannel = (int) SliderDumpSelectedChannel->getValue();
        float fSelectedChannel = (nSelectedChannel + 1.0f) / 100.0f;
        pProcessor->setParameter(TraKmeterPluginParameters::selValidationSelectedChannel, fSelectedChannel);

        bool bReportCSV = ButtonDumpCSV->getToggleState();
        pProcessor->setParameter(TraKmeterPluginParameters::selValidationCSVFormat, bReportCSV ? 1.0f : 0.0f);

        bool bAverageMeterLevel = ButtonDumpAverageMeterLevel->getToggleState();
        pProcessor->setParameter(TraKmeterPluginParameters::selValidationAverageMeterLevel, bAverageMeterLevel ? 1.0f : 0.0f);

        bool bPeakMeterLevel = ButtonDumpPeakMeterLevel->getToggleState();
        pProcessor->setParameter(TraKmeterPluginParameters::selValidationPeakMeterLevel, bPeakMeterLevel ? 1.0f : 0.0f);

        // validation file has already been initialised
        pProcessor->startValidation(fileValidation, nSelectedChannel, bReportCSV, bAverageMeterLevel, bPeakMeterLevel);

        // close window by making it invisible
        setVisible(false);
    }
    else if (button == ButtonCancel)
    {
        // close window by making it invisible
        setVisible(false);
    }
    else if (button == ButtonFileSelection)
    {
        WildcardFileFilter wildcardFilter("*.wav;*.aiff;*.flac", "", "Audio files (*.wav, *.aiff, *.flac)");

        FileBrowserComponent browser(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles, fileValidation, &wildcardFilter, NULL);

        FileChooserDialogBox dialogBox("Open audio file", "Please select an audio file to inject into traKmeter's audio path.", browser, true, getLookAndFeel().findColour(AlertWindow::backgroundColourId));

        if (dialogBox.show())
        {
            File selectedFile = browser.getSelectedFile(0);
            pProcessor->setParameterValidationFile(selectedFile);
            fileValidation = pProcessor->getParameterValidationFile();
            LabelFileSelection->setText(fileValidation.getFileName(), dontSendNotification);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
