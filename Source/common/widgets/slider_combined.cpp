/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)

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


SliderCombined::SliderCombined(ParameterJuggler *pParameters, int nParameterIndex, int nParameterIndexSwitch)
{
    pCombined = dynamic_cast<PluginParameterCombined *>(pParameters->getPluginParameter(nParameterIndex));
    jassert(pCombined != nullptr);

    pModeSwitch = dynamic_cast<PluginParameterBoolean *>(pParameters->getPluginParameter(nParameterIndexSwitch));
    jassert(pModeSwitch != nullptr);

    setRange(0.0f, 1.0f, pCombined->getStepSize());
    setDoubleClickReturnValue(true, pCombined->getDefaultFloat());
    colourRotary = Colours::white;

    Path pathCircle;
    pathCircle.addEllipse(0.0f, 0.0f, 8.0f, 8.0f);

    DrawablePath drawCircleOff;
    drawCircleOff.setFill(FillType(Colours::red.darker(0.2f)));
    drawCircleOff.setStrokeFill(FillType(Colours::black));
    drawCircleOff.setStrokeType(PathStrokeType(1.0f));
    drawCircleOff.setPath(pathCircle);

    DrawablePath drawCircleOn(drawCircleOff);
    drawCircleOn.setFill(FillType(Colours::red.darker(2.0f)));

    DrawablePath drawCircleOffOver(drawCircleOff);
    drawCircleOffOver.setFill(FillType(Colours::red));

    DrawablePath drawCircleOnOver(drawCircleOff);
    drawCircleOnOver.setFill(FillType(Colours::red.darker(0.8f)));

    toggleButton = new DrawableButton("Parameter Switch #" + nParameterIndexSwitch, DrawableButton::ImageFitted);
    toggleButton->setClickingTogglesState(true);
    toggleButton->setToggleState(pModeSwitch->getBoolean(), dontSendNotification);
    toggleButton->setImages(&drawCircleOff, &drawCircleOffOver, &drawCircleOnOver, nullptr, &drawCircleOn, &drawCircleOnOver, &drawCircleOffOver, nullptr);
    toggleButton->setColour(DrawableButton::backgroundColourId, Colours::transparentBlack);
    toggleButton->setColour(DrawableButton::backgroundOnColourId, Colours::transparentBlack);

    addAndMakeVisible(toggleButton);
}


void SliderCombined::visibilityChanged()
{
    Slider::visibilityChanged();

    setSliderStyle(Slider::RotaryVerticalDrag);
    setColour(Slider::rotarySliderFillColourId, colourRotary);
    setColour(Slider::textBoxTextColourId, Colours::white);
    setColour(Slider::textBoxBackgroundColourId, Colours::darkgrey.darker(0.7f));
    setColour(Slider::textBoxOutlineColourId, Colours::darkgrey.darker(0.4f));
}


void SliderCombined::resized()
{
    Slider::resized();

    int nWidth = getBounds().getWidth();
    setTextBoxStyle(Slider::TextBoxBelow, true, nWidth, 18);

    int nSwitchWidth = 15;
    int nSwitchX = nWidth - nSwitchWidth + 3;
    int nSwitchY = -3;

    toggleButton->setBounds(nSwitchX, nSwitchY, nSwitchWidth, nSwitchWidth);
}


void SliderCombined::setSliderColour(const Colour &colour)
{
    colourRotary = colour;
    setColour(Slider::rotarySliderFillColourId, colourRotary);
}


void SliderCombined::updateMode()
{
    toggleButton->setToggleState(pModeSwitch->getBoolean(), dontSendNotification);

    setRange(0.0f, 1.0f, pCombined->getStepSize());
    setDoubleClickReturnValue(true, pCombined->getDefaultFloat());

    float fNewValue = pCombined->getFloat();
    float fCurrentValue = (float) getValue();

    // make sure that the associated parameter gets updated even when
    // the slider doesn't change its position on mode change
    if (fNewValue == fCurrentValue)
    {
        float fMinimum = (float) getMinimum();
        float fMaximum = (float) getMaximum();

        // temporally set to nonsense value WITHOUT notification
        if (fNewValue == fMinimum)
        {
            setValue(fMaximum, dontSendNotification);
        }
        else
        {
            setValue(fMinimum, dontSendNotification);
        }
    }

    // set to new value WITH notification
    setValue(fNewValue, sendNotificationAsync);

    repaint();
}


void SliderCombined::addButtonListener(Button::Listener *newListener)
{
    toggleButton->addListener(newListener);
}


void SliderCombined::removeListener(Button::Listener *listener)
{
    toggleButton->removeListener(listener);
}


float SliderCombined::getRealFloat()
{
    return pCombined->getRealFloat();
}


bool SliderCombined::getBoolean()
{
    return pCombined->getBoolean();
}


int SliderCombined::getRealInteger()
{
    return pCombined->getRealInteger();
}


double SliderCombined::getValueFromText(const String &strText)
{
    return pCombined->getFloatFromText(strText);
}


String SliderCombined::getTextFromValue(double dValue)
{
    return pCombined->getTextFromFloat((float) dValue);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
