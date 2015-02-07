/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2015 Martin Zuther (http://www.mzuther.de/)

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
TraKmeterAudioProcessorEditor::TraKmeterAudioProcessorEditor(TraKmeterAudioProcessor *ownerFilter, TraKmeterPluginParameters *parameters, int nNumChannels, int CrestFactor)
    : AudioProcessorEditor(ownerFilter)
{
    // the editor window does not have any transparent areas
    // (increases performance on redrawing)
    setOpaque(true);

    bIsValidating = false;
    bReloadMeters = true;

    nInputChannels = nNumChannels;
    nCrestFactor = CrestFactor;
    nSegmentHeight = 10;

    // The plug-in editor's size as well as the location of buttons
    // and labels will be set later on in this constructor.

    pProcessor = ownerFilter;
    pProcessor->addActionListener(this);

    ButtonMeterType = new TextButton("Split");
    ButtonMeterType->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonMeterType->setColour(TextButton::buttonOnColourId, Colours::green);

    ButtonMeterType->addListener(this);
    addAndMakeVisible(ButtonMeterType);

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

    int nIndex = TraKmeterPluginParameters::selGain;
    String strName = parameters->getName(nIndex);
    SliderGain = new SliderSwitch(strName, parameters, nIndex);
    SliderGain->setSliderColour(Colours::red);

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
    LabelDebug = nullptr;
#endif

    ButtonValidation = new TextButton("Validate");
    ButtonValidation->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonValidation->setColour(TextButton::buttonOnColourId, Colours::red);

    ButtonValidation->addListener(this);
    addAndMakeVisible(ButtonValidation);

    ButtonAbout = new TextButton("About");
    ButtonAbout->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonAbout->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonAbout->addListener(this);
    addAndMakeVisible(ButtonAbout);

    // This is where our plug-in editor's size is set.
    resizeEditor();

    trakmeter = nullptr;

    updateParameter(TraKmeterPluginParameters::selTransientMode);
    updateParameter(TraKmeterPluginParameters::selCrestFactor);
    updateParameter(TraKmeterPluginParameters::selMixMode);
    updateParameter(TraKmeterPluginParameters::selGain);
    updateParameter(TraKmeterPluginParameters::selMeterType);
}


TraKmeterAudioProcessorEditor::~TraKmeterAudioProcessorEditor()
{
    pProcessor->removeActionListener(this);

    deleteAllChildren();
}


void TraKmeterAudioProcessorEditor::resizeEditor()
{
    nHeight = 20 * nSegmentHeight + 85;
    nRightColumnStart = 2 * TraKmeter::TRAKMETER_LABEL_WIDTH + nInputChannels * (TraKmeter::TRAKMETER_SEGMENT_WIDTH + 6) + 20;

    setSize(nRightColumnStart + 70, nHeight);

    ButtonReset->setBounds(nRightColumnStart, 10, 60, 20);

    ButtonMeterType->setBounds(nRightColumnStart, 45, 60, 20);
    ButtonCrestFactor->setBounds(nRightColumnStart, 70, 60, 20);
    ButtonTransientMode->setBounds(nRightColumnStart, 95, 60, 20);
    ButtonMixMode->setBounds(nRightColumnStart, 120, 60, 20);
    SliderGain->setBounds(nRightColumnStart + 5, 145, 50, 50);

    ButtonValidation->setBounds(nRightColumnStart, nHeight - 56, 60, 20);
    ButtonAbout->setBounds(nRightColumnStart, nHeight - 31, 60, 20);

    if (LabelDebug)
    {
        LabelDebug->setBounds(nRightColumnStart, nHeight - 78, 60, 16);
    }
}


void TraKmeterAudioProcessorEditor::actionListenerCallback(const String &strMessage)
{
    // "PC" --> parameter changed, followed by a hash and the
    // parameter's ID
    if (strMessage.startsWith("PC#"))
    {
        String strIndex = strMessage.substring(3);
        int nIndex = strIndex.getIntValue();
        jassert(nIndex >= 0);
        jassert(nIndex < pProcessor->getNumParameters());

        if (pProcessor->hasChanged(nIndex))
        {
            updateParameter(nIndex);
        }
    }
    // "UM" --> update meters
    else if (!strMessage.compare("UM"))
    {
        MeterBallistics *pMeterBallistics = pProcessor->getLevels();

        if (pMeterBallistics)
        {
            trakmeter->setLevels(pMeterBallistics);
        }

        if (bIsValidating && !pProcessor->isValidating())
        {
            bIsValidating = false;
            ButtonValidation->setToggleState(false, dontSendNotification);
        }
    }
    // "V+" --> validation started
    else if ((!strMessage.compare("V+")) && pProcessor->isValidating())
    {
        bIsValidating = true;
        ButtonValidation->setToggleState(true, dontSendNotification);
    }
    // "V-" --> validation stopped
    else if (!strMessage.compare("V-"))
    {
        // do nothing till you hear from me... :)
    }
    else
    {
        DBG("[traKmeter] Received unknown action message \"" + strMessage + "\".");
    }
}


void TraKmeterAudioProcessorEditor::updateParameter(int nIndex)
{
    pProcessor->clearChangeFlag(nIndex);

    switch (nIndex)
    {
    case TraKmeterPluginParameters::selMeterType:
    {
        bool bMeterType = pProcessor->getBoolean(nIndex);
        ButtonMeterType->setToggleState(!bMeterType, dontSendNotification);

        bReloadMeters = true;
    }
    break;

    case TraKmeterPluginParameters::selCrestFactor:
    {
        nCrestFactor = pProcessor->getRealInteger(nIndex);
        ButtonCrestFactor->setToggleState(nCrestFactor != 0, dontSendNotification);

        bReloadMeters = true;
    }
    break;

    case TraKmeterPluginParameters::selTransientMode:
    {
        bool bTransientMode = pProcessor->getBoolean(nIndex);
        ButtonTransientMode->setToggleState(bTransientMode, dontSendNotification);
    }
    break;

    case TraKmeterPluginParameters::selMixMode:
    {
        bool bMixMode = pProcessor->getBoolean(nIndex);
        ButtonMixMode->setToggleState(bMixMode, dontSendNotification);

        SliderGain->setVisible(bMixMode);
    }
    break;

    case TraKmeterPluginParameters::selGain:
    {
        float fValue = pProcessor->getParameter(nIndex);
        SliderGain->setValue(fValue, dontSendNotification);
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
            trakmeter = nullptr;
        }

        int nMeterType = pProcessor->getRealInteger(TraKmeterPluginParameters::selMeterType);
        trakmeter = new TraKmeter("traKmeter (level meter)", 10, 10, nCrestFactor, nInputChannels, nSegmentHeight, nMeterType);
        addAndMakeVisible(trakmeter);
    }
}

//==============================================================================
void TraKmeterAudioProcessorEditor::paint(Graphics &g)
{
    g.setGradientFill(ColourGradient(Colours::darkgrey.darker(0.8f), 0, 0, Colours::darkgrey.darker(1.4f), 0, (float) getHeight(), false));
    g.fillAll();
}


void TraKmeterAudioProcessorEditor::buttonClicked(Button *button)
{
    if (button == ButtonReset)
    {
        MeterBallistics *pMeterBallistics = pProcessor->getLevels();

        if (pMeterBallistics)
        {
            pMeterBallistics->reset();
        }
    }
    else if (button == ButtonMeterType)
    {
        pProcessor->changeParameter(TraKmeterPluginParameters::selMeterType, button->getToggleState());
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
        WindowAbout windowAbout(this);
        windowAbout.runModalLoop();
    }
    else if (button == ButtonValidation)
    {
        WindowValidation windowValidation(this, pProcessor);
        windowValidation.runModalLoop();
    }
}


void TraKmeterAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == SliderGain)
    {
        float fValue = (float) slider->getValue();
        pProcessor->changeParameter(TraKmeterPluginParameters::selGain, fValue);
    }
}


void TraKmeterAudioProcessorEditor::resized()
{
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
