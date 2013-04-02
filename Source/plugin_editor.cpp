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

#include "plugin_editor.h"

//==============================================================================
TraKmeterAudioProcessorEditor::TraKmeterAudioProcessorEditor(TraKmeterAudioProcessor* ownerFilter, int nNumChannels, int CrestFactor)
    : AudioProcessorEditor(ownerFilter)
{
    // the editor window does not have any transparent areas
    // (increases performance on redrawing)
    setOpaque(true);

    bIsValidating = false;
    bReloadMeters = true;

    nInputChannels = nNumChannels;
    nCrestFactor = CrestFactor;

    // The plug-in editor's size as well as the location of buttons
    // and labels will be set later on in this constructor.

    pProcessor = ownerFilter;
    pProcessor->addActionListener(this);

    ButtonCrestFactor = new TextButton("K-20");
    ButtonCrestFactor->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonCrestFactor->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonCrestFactor->addListener(this);
    addAndMakeVisible(ButtonCrestFactor);

    ButtonTransientMode = new TextButton("Transient");
    ButtonTransientMode->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonTransientMode->setColour(TextButton::buttonOnColourId, Colours::orange);

    ButtonTransientMode->addListener(this);
    addAndMakeVisible(ButtonTransientMode);

    ButtonMixMode = new TextButton("Mixing");
    ButtonMixMode->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonMixMode->setColour(TextButton::buttonOnColourId, Colours::red);

    ButtonMixMode->addListener(this);
    addAndMakeVisible(ButtonMixMode);

    SliderGain = new Slider("Gain");
    SliderGain->setSliderStyle(Slider::RotaryVerticalDrag);
    SliderGain->setRange(-12.0f, 12.0f, 1.0f);
    SliderGain->setTextBoxStyle(Slider::TextBoxRight, true, 50, 20);
    SliderGain->setDoubleClickReturnValue(true, 0.0);
    SliderGain->setTextValueSuffix(" dB");

    SliderGain->setColour(Slider::rotarySliderFillColourId, Colours::white);
    SliderGain->setColour(Slider::textBoxTextColourId, Colours::yellow);
    SliderGain->setColour(Slider::textBoxBackgroundColourId, Colours::darkgrey.darker(0.7f));
    SliderGain->setColour(Slider::textBoxOutlineColourId, Colours::darkgrey.darker(0.4f));

    SliderGain->addListener(this);
    addChildComponent(SliderGain);

    ButtonReset = new TextButton("Reset");
    ButtonReset->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonReset->setColour(TextButton::buttonOnColourId, Colours::red);

    ButtonReset->addListener(this);
    addAndMakeVisible(ButtonReset);

#ifdef DEBUG
    LabelDebug = new Label("Debug Notification", "DEBUG");
    LabelDebug->setColour(Label::textColourId, Colours::red);
    LabelDebug->setJustificationType(Justification::centred);
    addAndMakeVisible(LabelDebug);
#else
    LabelDebug = NULL;
#endif

    ButtonValidation = new TextButton("Validate");
    ButtonValidation->setColour(TextButton::textColourOnId, Colours::white);
    ButtonValidation->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonValidation->setColour(TextButton::buttonOnColourId, Colours::blue);

    ButtonValidation->addListener(this);
    addAndMakeVisible(ButtonValidation);

    ButtonAbout = new TextButton("About");
    ButtonAbout->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonAbout->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonAbout->addListener(this);
    addAndMakeVisible(ButtonAbout);

    // This is where our plug-in editor's size is set.
    resizeEditor();

    pProcessor->addActionListenerParameters(this);
    trakmeter = NULL;

    int nIndex = TraKmeterPluginParameters::selCrestFactor;
    changeParameter(nIndex);

    nIndex = TraKmeterPluginParameters::selTransientMode;
    changeParameter(nIndex);

    nIndex = TraKmeterPluginParameters::selMixMode;
    changeParameter(nIndex);

    nIndex = TraKmeterPluginParameters::selGain;
    changeParameter(nIndex);
}


TraKmeterAudioProcessorEditor::~TraKmeterAudioProcessorEditor()
{
    pProcessor->removeActionListener(this);
    pProcessor->removeActionListenerParameters(this);

    deleteAllChildren();
}


void TraKmeterAudioProcessorEditor::resizeEditor()
{
    nHeight = 20 * TraKmeter::TRAKMETER_SEGMENT_HEIGHT + 103;
    nRightColumnStart = 2 * TraKmeter::TRAKMETER_LABEL_WIDTH + nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) + 20;

    setSize(nRightColumnStart + 70, nHeight);

    ButtonReset->setBounds(nRightColumnStart, 10, 60, 20);
    ButtonCrestFactor->setBounds(nRightColumnStart, 45, 60, 20);
    ButtonTransientMode->setBounds(nRightColumnStart, 70, 60, 20);
    ButtonMixMode->setBounds(nRightColumnStart, 95, 60, 20);
    SliderGain->setBounds(nRightColumnStart - 10, 121, 70, 20);

    ButtonValidation->setBounds(nRightColumnStart, nHeight - 56, 60, 20);
    ButtonAbout->setBounds(nRightColumnStart, nHeight - 31, 60, 20);

    if (LabelDebug)
    {
        LabelDebug->setBounds(nRightColumnStart, nHeight - 82, 60, 16);
    }
}


void TraKmeterAudioProcessorEditor::actionListenerCallback(const String& message)
{
    // "PC" --> parameter changed, followed by a hash and the
    // parameter's ID
    if (message.startsWith("PC#"))
    {
        String strIndex = message.substring(3);
        int nIndex = strIndex.getIntValue();
        jassert(nIndex >= 0);
        jassert(nIndex < pProcessor->getNumParameters());

        if (pProcessor->isParameterMarked(nIndex))
        {
            changeParameter(nIndex);
            pProcessor->UnmarkParameter(nIndex);
        }
    }
    // "UM" --> update meters
    else if (!message.compare("UM"))
    {
        MeterBallistics* pMeterBallistics = pProcessor->getLevels();

        if (pMeterBallistics)
        {
            trakmeter->setLevels(pMeterBallistics);
        }

        if (bIsValidating && !pProcessor->isValidating())
        {
            bIsValidating = false;
            ButtonValidation->setColour(TextButton::buttonColourId, Colours::grey);
        }
    }
    // "V+" --> validation started
    else if ((!message.compare("V+")) && pProcessor->isValidating())
    {
        bIsValidating = true;
        ButtonValidation->setColour(TextButton::buttonColourId, Colours::red);
    }
    // "V-" --> validation stopped
    else if (!message.compare("V-"))
    {
        // do nothing till you hear from me... :)
    }
    else
    {
        DBG("[traKmeter] Received unknown action message \"" + message + "\".");
    }
}


void TraKmeterAudioProcessorEditor::changeParameter(int nIndex)
{
    switch (nIndex)
    {
    case TraKmeterPluginParameters::selCrestFactor:
    {
        nCrestFactor = pProcessor->getParameterAsInt(nIndex);
        ButtonCrestFactor->setToggleState(nCrestFactor != 0, false);

        bReloadMeters = true;
    }
    break;

    case TraKmeterPluginParameters::selTransientMode:
    {
        bool bTransientMode = pProcessor->getParameterAsBool(nIndex);
        ButtonTransientMode->setToggleState(bTransientMode, false);
    }
    break;

    case TraKmeterPluginParameters::selMixMode:
    {
        bool bMixMode = pProcessor->getParameterAsBool(nIndex);
        ButtonMixMode->setToggleState(bMixMode, false);

        SliderGain->setVisible(bMixMode);
    }
    break;

    case TraKmeterPluginParameters::selGain:
    {
        int nGain = pProcessor->getParameterAsInt(nIndex);
        SliderGain->setValue(nGain, false);
    }
    break;
    }

    reloadMeters();
}


void TraKmeterAudioProcessorEditor::reloadMeters()
{
    if (bReloadMeters)
    {
        bReloadMeters = false;

        if (trakmeter)
        {
            removeChildComponent(trakmeter);
            delete trakmeter;
            trakmeter = NULL;
        }

        trakmeter = new TraKmeter("traKmeter (level meter)", 10, 10, nCrestFactor, nInputChannels);
        addAndMakeVisible(trakmeter);
    }
}

//==============================================================================
void TraKmeterAudioProcessorEditor::paint(Graphics& g)
{
    g.setGradientFill(ColourGradient(Colours::darkgrey.darker(0.8f), 0, 0, Colours::darkgrey.darker(1.4f), 0, (float) getHeight(), false));
    g.fillAll();
}


void TraKmeterAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == ButtonReset)
    {
        MeterBallistics* pMeterBallistics = pProcessor->getLevels();

        if (pMeterBallistics)
        {
            pMeterBallistics->reset();
        }
    }
    else if (button == ButtonCrestFactor)
    {
        pProcessor->changeParameter(TraKmeterPluginParameters::selCrestFactor, !button->getToggleState());
    }
    else if (button == ButtonTransientMode)
    {
        pProcessor->changeParameter(TraKmeterPluginParameters::selTransientMode, !button->getToggleState());
    }
    else if (button == ButtonMixMode)
    {
        pProcessor->changeParameter(TraKmeterPluginParameters::selMixMode, !button->getToggleState());
    }
    else if (button == ButtonAbout)
    {
        WindowAbout* windowAbout = new WindowAbout(getWidth(), getHeight());
        addAndMakeVisible(windowAbout);

        windowAbout->runModalLoop();

        removeChildComponent(windowAbout);
        delete windowAbout;
        windowAbout = NULL;
    }
    else if (button == ButtonValidation)
    {
        WindowValidation* windowValidation = new WindowValidation(getWidth(), getHeight(), pProcessor);
        addAndMakeVisible(windowValidation);

        windowValidation->runModalLoop();

        removeChildComponent(windowValidation);
        delete windowValidation;
        windowValidation = NULL;
    }
}


void TraKmeterAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == SliderGain)
    {
        float fValue = float(slider->getValue()) / 100.0f + 0.12f;
        pProcessor->changeParameter(TraKmeterPluginParameters::selGain, fValue);
    }
}


void TraKmeterAudioProcessorEditor::resized()
{
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
