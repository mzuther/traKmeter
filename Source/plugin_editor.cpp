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

    // prevent meter reload during initialisation
    bInitialising = true;

    bIsValidating = false;
    bValidateWindow = false;

    nInputChannels = nNumChannels;
    nCrestFactor = CrestFactor;
    nSegmentHeight = 10;

    // The plug-in editor's size as well as the location of buttons
    // and labels will be set later on in this constructor.

    pProcessor = ownerFilter;
    pProcessor->addActionListener(this);

    ButtonMeterType = new ImageButton("Split");
    ButtonMeterType->addListener(this);
    addAndMakeVisible(ButtonMeterType);

    ButtonCrestFactor = new ImageButton("K-20");
    ButtonCrestFactor->addListener(this);
    addAndMakeVisible(ButtonCrestFactor);

    ButtonTransientMode = new ImageButton("Transient");
    ButtonTransientMode->addListener(this);
    addAndMakeVisible(ButtonTransientMode);

    ButtonMixMode = new ImageButton("Mixing");
    ButtonMixMode->addListener(this);
    addAndMakeVisible(ButtonMixMode);

    int nIndex = TraKmeterPluginParameters::selGain;
    String strName = parameters->getName(nIndex);
    SliderGain = new SliderSwitch(strName, parameters, nIndex);
    SliderGain->setSliderColour(Colours::red);

    SliderGain->addListener(this);
    addChildComponent(SliderGain);

    ButtonReset = new ImageButton("Reset");
    ButtonReset->addListener(this);
    addAndMakeVisible(ButtonReset);

    ButtonSkin = new ImageButton("Skin");
    ButtonSkin->addListener(this);
    addAndMakeVisible(ButtonSkin);

    ButtonValidation = new ImageButton("Validate");
    ButtonValidation->addListener(this);
    addAndMakeVisible(ButtonValidation);

    ButtonAbout = new ImageButton("About");
    ButtonAbout->addListener(this);
    addAndMakeVisible(ButtonAbout);

    LabelDebug = new ImageComponent("Debug Notification");
    // moves debug label to the back of the editor's z-plane to that
    // it doesn't overlay (and thus block) any other components
    addAndMakeVisible(LabelDebug, 0);

    BackgroundImage = new ImageComponent("Background Image");
    // prevent unnecessary redrawing of plugin editor
    BackgroundImage->setOpaque(true);
    // moves background image to the back of the editor's z-plane to
    // that it doesn't overlay (and thus block) any other components
    addAndMakeVisible(BackgroundImage, 0);

    trakmeter = nullptr;

    updateParameter(TraKmeterPluginParameters::selTransientMode);
    updateParameter(TraKmeterPluginParameters::selCrestFactor);
    updateParameter(TraKmeterPluginParameters::selMixMode);
    updateParameter(TraKmeterPluginParameters::selGain);
    updateParameter(TraKmeterPluginParameters::selMeterType);

    // the following may or may not work on Mac
    File fileApplicationDirectory = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory();
    fileSkinDirectory = fileApplicationDirectory.getChildFile("./trakmeter-skins/");

    pSkin = nullptr;
    strSkinName = pProcessor->getParameterSkinName();
    loadSkin();

    // force meter reload after initialisation ...
    bInitialising = false;
    bReloadMeters = true;

    // will also apply skin to plug-in editor
    reloadMeters();
}


TraKmeterAudioProcessorEditor::~TraKmeterAudioProcessorEditor()
{
    pProcessor->removeActionListener(this);

    delete pSkin;
    pSkin = nullptr;

    deleteAllChildren();
}


void TraKmeterAudioProcessorEditor::loadSkin()
{
    if (pSkin != nullptr)
    {
        delete pSkin;
        pSkin = nullptr;
    }

    File fileSkin = fileSkinDirectory.getChildFile(strSkinName + ".skin");

    if (!fileSkin.existsAsFile())
    {
        Logger::outputDebugString("[Skin] file \"" + fileSkin.getFileName() + "\" not found");

        strSkinName = "Default";
        fileSkin = fileSkinDirectory.getChildFile(strSkinName + ".skin");
    }

    pProcessor->setParameterSkinName(strSkinName);

    int nMeterType = pProcessor->getRealInteger(TraKmeterPluginParameters::selMeterType);
    pSkin = new Skin(fileSkin, nInputChannels, nCrestFactor, nMeterType);
}


void TraKmeterAudioProcessorEditor::applySkin()
{
    // prevent skin application during meter initialisation
    if (bInitialising)
    {
        return;
    }

    // update skin
    int nMeterType = pProcessor->getRealInteger(TraKmeterPluginParameters::selMeterType);
    pSkin->updateSkin(nInputChannels, nCrestFactor, nMeterType);

    // moves background image to the back of the editor's z-plane;
    // will also resize plug-in editor
    pSkin->setBackgroundImage(BackgroundImage, this);

    pSkin->placeAndSkinButton(ButtonMeterType, "button_split");
    pSkin->placeAndSkinButton(ButtonCrestFactor, "button_k20");
    pSkin->placeAndSkinButton(ButtonTransientMode, "button_transient");
    pSkin->placeAndSkinButton(ButtonMixMode, "button_mixing");

    pSkin->placeAndSkinButton(ButtonReset, "button_reset");
    pSkin->placeAndSkinButton(ButtonSkin, "button_skin");

    pSkin->placeAndSkinButton(ButtonValidation, "button_validate");
    pSkin->placeAndSkinButton(ButtonAbout, "button_about");

    pSkin->placeComponent(SliderGain, "slider_gain");

#ifdef DEBUG
    pSkin->placeAndSkinLabel(LabelDebug, "label_debug");
#else
    pSkin->placeComponent(LabelDebug, "label_debug");
    LabelDebug->setImage(Image());
    LabelDebug->setBounds(-1, -1, 1, 1);
#endif

    if (trakmeter != nullptr)
    {
        trakmeter->applySkin(pSkin);
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
        }
    }
    // "V+" --> validation started
    else if ((!strMessage.compare("V+")) && pProcessor->isValidating())
    {
        bIsValidating = true;
    }
    // "V-" --> validation stopped
    else if (!strMessage.compare("V-"))
    {
        if (!bValidateWindow)
        {
            ButtonValidation->setToggleState(false, dontSendNotification);
        }

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

        // will also apply skin to plug-in editor
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

    // prevent meter reload during initialisation
    if (!bInitialising)
    {
        // will also apply skin to plug-in editor
        reloadMeters();
    }
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

        // moves traKmeter to the back of the editor's z-plane so that
        // it doesn't overlay (and thus block) any other components
        addAndMakeVisible(trakmeter, 0);

        // moves background image to the back of the editor's z-plane
        applySkin();
    }
}

//==============================================================================
void TraKmeterAudioProcessorEditor::buttonClicked(Button *button)
{
    if (button == ButtonReset)
    {
        MeterBallistics *pMeterBallistics = pProcessor->getLevels();

        if (pMeterBallistics)
        {
            pMeterBallistics->reset();
        }

        loadSkin();

        // will also apply skin to plug-in editor
        bReloadMeters = true;
        reloadMeters();
    }
    else if (button == ButtonMeterType)
    {
        pProcessor->changeParameter(TraKmeterPluginParameters::selMeterType, button->getToggleState());

        // will also apply skin to plug-in editor
        bReloadMeters = true;
        reloadMeters();
    }
    else if (button == ButtonCrestFactor)
    {
        pProcessor->changeParameter(TraKmeterPluginParameters::selCrestFactor, !button->getToggleState());

        // will also apply skin to plug-in editor
        bReloadMeters = true;
        reloadMeters();
    }
    else if (button == ButtonTransientMode)
    {
        pProcessor->changeParameter(TraKmeterPluginParameters::selTransientMode, !button->getToggleState());
    }
    else if (button == ButtonMixMode)
    {
        pProcessor->changeParameter(TraKmeterPluginParameters::selMixMode, !button->getToggleState());
    }
    else if (button == ButtonSkin)
    {
        // manually activate button
        button->setToggleState(true, dontSendNotification);

        File fileSkin = fileSkinDirectory.getChildFile(strSkinName + ".skin");

        GenericWindowSkin windowSkin(this, fileSkin);
        windowSkin.runModalLoop();

        // manually deactivate button
        button->setToggleState(false, dontSendNotification);

        strSkinName = windowSkin.getSelectedString();
        loadSkin();

        // will also apply skin to plug-in editor
        bReloadMeters = true;
        reloadMeters();
    }
    else if (button == ButtonAbout)
    {
        // manually activate button
        button->setToggleState(true, dontSendNotification);

        StringPairArray strArray;

        strArray.set("Copyright", "(c) 2012-2015 Martin Zuther\n");

        strArray.set("Contributors",
                     L"Filipe Coelho\n"
                     L"Bram de Jong\n");

        strArray.set("Beta testing",
                     L"Rickard (Interfearing Sounds)\n");

        strArray.set("Thanks",
                     L"I want to thank all contributors "
                     L"and beta testers and the open source "
                     L"community at large!\n\n"
                     L"Thank you for using free software!\n");

        strArray.set("Libraries",
#ifdef LINUX
                     L"ALSA\n"
#endif
#ifdef LINUX
                     L"FreeType\n"
                     L"JACK\n"
#endif
                     L"JUCE\n"
#if (KMETER_LV2_PLUGIN != 0)
                     L"LV2\n"
#endif
#ifdef LINUX
                     L"POSIX Threads\n"
                     L"Xlib\n"
                     L"Xext\n"
#endif
                    );

#if (JUCE_USE_VSTSDK_2_4 != 0)

        // display trademarks (but only when necessary)
        strArray.set("Trademarks",
                     L"VST PlugIn Technology by Steinberg\n");

#endif

#if (JUCE_ASIO != 0)

        // display trademarks (but only when necessary)
        strArray.set("Trademarks",
                     L"ASIO Technology by Steinberg Media Technologies GmbH\n");

#endif

        GenericWindowAbout windowAbout(this, strArray);
        windowAbout.runModalLoop();

        // manually deactivate button
        button->setToggleState(false, dontSendNotification);
    }
    else if (button == ButtonValidation)
    {
        // manually activate button
        button->setToggleState(true, dontSendNotification);

        bValidateWindow = true;
        WindowValidation windowValidation(this, pProcessor);
        windowValidation.runModalLoop();
        bValidateWindow = false;

        // manually set button according to validation state
        button->setToggleState(bIsValidating, dontSendNotification);
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
