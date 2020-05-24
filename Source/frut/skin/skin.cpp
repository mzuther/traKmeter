/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2020 Martin Zuther (http://www.mzuther.de/)

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

namespace frut
{
namespace skin
{

// The following class members are initialized in the derived class:
//
// * currentBackgroundName_
// * currentFallbackName_
// * currentGroupName_
// * skinGroup_
// * skinFallback_1_
// * skinFallback_2_
//
bool Skin::loadFromXml(
    const File &skinFile,
    const String &rootName,
    const String &assumedVersionNumber)
{
    settingsGroup_ = nullptr;
    skinGroup_ = nullptr;
    skinFallback_1_ = nullptr;
    skinFallback_2_ = nullptr;

    backgroundWidth_ = 0;
    backgroundHeight_ = 0;

    Logger::outputDebugString(
        String("[Skin] loading file \"") +
        skinFile.getFileName() + "\"");

    document_ = juce::parseXML(skinFile);

    if (document_ == nullptr)
    {
        Logger::outputDebugString(
            String("[Skin] file \"") +
            skinFile.getFullPathName() +
            "\" not found");

        return false;
    }

    String skinVersion = getString(document_.get(), "version");

    if (skinVersion.compare(assumedVersionNumber) != 0)
    {
        Logger::outputDebugString(
            String("[Skin] file \"") +
            skinFile.getFileName() +
            "\" has incompatible version number \"" +
            skinVersion +
            "\"");

        document_ = nullptr;
        return false;
    }

    settingsGroup_ = document_->getChildByName("settings");
    skinFallback_2_ = document_->getChildByName("default");

    if ((!document_->hasTagName(rootName)) ||
            (skinFallback_2_ == nullptr))
    {
        Logger::outputDebugString(
            "[Skin] XML file not valid");

        document_ = nullptr;
        return false;
    }
    else
    {
        skinGroup_ = document_->getChildByName(currentGroupName_);

        if (skinGroup_ == nullptr)
        {
            Logger::outputDebugString(
                String("[Skin] XML element \"") +
                currentGroupName_ +
                "\" not found in settings");
        }

        skinFallback_1_ = document_->getChildByName(currentFallbackName_);

        String resourcePathName = getString(document_.get(), "path");
        resourcePath_ = File(skinFile.getSiblingFile(resourcePathName));

        if (!resourcePath_.isDirectory())
        {
            Logger::outputDebugString(
                String("[Skin] directory \"") +
                resourcePath_.getFullPathName() +
                "\" not found");

            document_ = nullptr;
            return false;
        }
    }

    String originOfY = getString(document_.get(), "origin_of_y", "top");
    originOfYIsBottom_ = originOfY.compare("bottom") == 0;

    return true;
}


File Skin::getDefaultSkinFile(
    const File &skinDirectory)
{
    // locate file containing the default skin's name
    File defaultSkinFile = skinDirectory.getChildFile("default_skin.ini");

    // make sure the file exists
    if (!defaultSkinFile.existsAsFile())
    {
        // create file
        defaultSkinFile.create();

        // set "Default" as default skin, as it comes with the plug-in
        // (uses Unicode encoding)
        defaultSkinFile.replaceWithText("Default", true, true);
    }

    return defaultSkinFile;
}


String Skin::getDefaultSkin(
    const File &skinDirectory)
{
    File defaultSkinFile = getDefaultSkinFile(skinDirectory);

    return defaultSkinFile.loadFileAsString();
}


void Skin::setDefaultSkin(
    const String &defaultSkinName,
    const File &skinDirectory)
{
    File defaultSkinFile = getDefaultSkinFile(skinDirectory);

    // uses Unicode encoding
    defaultSkinFile.replaceWithText(defaultSkinName, true, true);
}


XmlElement *Skin::getSetting(
    const String &tagName)
{
    if (document_ == nullptr)
    {
        return nullptr;
    }

    if (settingsGroup_ == nullptr)
    {
        Logger::outputDebugString(
            String("[Skin] settings group not found"));

        return nullptr;
    }

    XmlElement *xmlSetting = settingsGroup_->getChildByName(tagName);

    if (xmlSetting == nullptr)
    {
        Logger::outputDebugString(
            String("[Skin] XML element \"") +
            tagName +
            "\" not found in settings");

        return nullptr;
    }

    return xmlSetting;
}


XmlElement *Skin::getComponent(
    const String &tagName)
{
    XmlElement *xmlComponent;

    // suppress unnecessary warnings and save some time
    if (document_ == nullptr)
    {
        xmlComponent = nullptr;
    }
    else if ((skinGroup_ != nullptr) &&
             (skinGroup_->getChildByName(tagName) != nullptr))
    {
        xmlComponent = skinGroup_->getChildByName(tagName);
    }
    else if ((skinFallback_1_ != nullptr) &&
             (skinFallback_1_->getChildByName(tagName) != nullptr))
    {
        xmlComponent = skinFallback_1_->getChildByName(tagName);
    }
    else if ((skinFallback_2_ != nullptr) &&
             (skinFallback_2_->getChildByName(tagName) != nullptr))
    {
        xmlComponent = skinFallback_2_->getChildByName(tagName);
    }
    else
    {
        Logger::outputDebugString(
            String("[Skin] XML element \"") +
            tagName +
            "\" not found");

        xmlComponent = nullptr;
    }

    return xmlComponent;
}


bool Skin::getBoolean(
    const XmlElement *xmlComponent,
    const String &attributeName,
    const bool defaultValue)
{
    if (xmlComponent == nullptr)
    {
        return defaultValue;
    }
    else
    {
        return xmlComponent->getBoolAttribute(attributeName, defaultValue);
    }
}


int Skin::getInteger(
    const XmlElement *xmlComponent,
    const String &attributeName,
    const int defaultValue)
{
    if (xmlComponent == nullptr)
    {
        return defaultValue;
    }
    else
    {
        return xmlComponent->getIntAttribute(attributeName, defaultValue);
    }
}


float Skin::getFloat(
    const XmlElement *xmlComponent,
    const String &attributeName,
    const float defaultValue)
{
    if (xmlComponent == nullptr)
    {
        return defaultValue;
    }
    else
    {
        double result = xmlComponent->getDoubleAttribute(
                            attributeName, defaultValue);

        return static_cast<float>(result);
    }
}


const String Skin::getString(
    const XmlElement *xmlComponent,
    const String &attributeName,
    const String &defaultValue)
{
    if (xmlComponent == nullptr)
    {
        return defaultValue;
    }
    else
    {
        return xmlComponent->getStringAttribute(attributeName, defaultValue);
    }
}


const Colour Skin::getColour(
    const XmlElement *xmlComponent,
    const Colour defaultColour,
    const String valuePrefix)
{
    if (xmlComponent == nullptr)
    {
        return Colours::white;
    }

    float hue = getFloat(xmlComponent,
                         valuePrefix + "hue",
                         defaultColour.getHue());

    float saturation = getFloat(xmlComponent,
                                valuePrefix + "saturation",
                                defaultColour.getSaturation());

    float brightness = getFloat(xmlComponent,
                                valuePrefix + "brightness",
                                defaultColour.getBrightness());

    float alpha = getFloat(xmlComponent,
                           valuePrefix + "alpha",
                           defaultColour.getFloatAlpha());

    // initialise HSBA colour
    return Colour(hue, saturation, brightness, alpha);
}


Image Skin::createBogusImage(
    const String &warningText,
    int width,
    int height)
{
    Image bogusImage(
        Image::PixelFormat::RGB,
        width,
        height,
        true);

    Graphics g(bogusImage);
    g.fillAll(Colours::red);

    g.setColour(Colours::white);
    g.drawFittedText(
        warningText,
        10,
        10,
        bogusImage.getWidth() - 10,
        bogusImage.getHeight() - 10,
        Justification::Flags::topLeft,
        1);

    return bogusImage;
}


void Skin::loadImage(
    const String &strFilename,
    Image &image)
{
    File fileImage = resourcePath_.getChildFile(strFilename);

    if (fileImage.existsAsFile())
    {
        image = ImageFileFormat::loadFrom(fileImage);
    }
    else
    {
        Logger::outputDebugString(
            String("[Skin] image file \"") +
            fileImage.getFullPathName() +
            "\" not found");

        image = createBogusImage("Image file not found", 200, 200);
    }
}


void Skin::setBackgroundImage(
    ImageComponent *background,
    AudioProcessorEditor *editor)
{
    Image imageBackground;

    if (skinGroup_ != nullptr)
    {
        XmlElement *xmlBackground = skinGroup_->getChildByName("background");

        if (xmlBackground != nullptr)
        {
            String strImageFilename = getString(xmlBackground,
                                                currentBackgroundName_);

            loadImage(strImageFilename, imageBackground);
        }
        else
        {
            Logger::outputDebugString(
                String("[Skin] XML element \"") +
                currentGroupName_ +
                "\" specifies no background image");
        }
    }

    if (!imageBackground.isValid())
    {
        imageBackground = createBogusImage("No background specified", 200, 200);
    }

    backgroundWidth_ = imageBackground.getWidth();
    backgroundHeight_ = imageBackground.getHeight();

    if (skinGroup_ != nullptr)
    {
        forEachXmlChildElementWithTagName(*skinGroup_,
                                          xmlMeterGraduation,
                                          "meter_graduation")
        {
            Image imageMeterGraduation;
            String strImageFilename = getString(xmlMeterGraduation,
                                                currentBackgroundName_);

            loadImage(strImageFilename, imageMeterGraduation);

            if (imageMeterGraduation.isValid())
            {
                int height = imageMeterGraduation.getHeight();
                Point<int> position = getPosition(xmlMeterGraduation, height);

                Graphics g(imageBackground);
                g.drawImageAt(imageMeterGraduation,
                              position.getX(), position.getY(),
                              false);
            }
        }
    }

    background->setImage(imageBackground);
    background->setBounds(0, 0, backgroundWidth_, backgroundHeight_);

    // moves background image to the back of the editor's z-plane so
    // that it doesn't overlay (and thus block) any other components
    background->toBack();

    editor->setSize(backgroundWidth_, backgroundHeight_);
}


Point<int> Skin::getPosition(
    const XmlElement *xmlComponent,
    const int componentHeight)
{
    if (document_ == nullptr)
    {
        return Point<int>(0, 0);
    }

    jassert(backgroundHeight_ != 0);

    int x = getInteger(xmlComponent, "x", 0);
    int y = getInteger(xmlComponent, "y", 0);

    if (originOfYIsBottom_)
    {
        y = backgroundHeight_ - (y + componentHeight);
    }

    return Point<int>(x, y);
}


Rectangle<int> Skin::getBounds(
    const XmlElement *xmlComponent,
    int width,
    int height)
{
    if (document_ == nullptr)
    {
        return Rectangle<int>(0, 0, 1, 1);
    }

    if (width < 0)
    {
        width = getInteger(xmlComponent, "width", 0);
    }

    if (height < 0)
    {
        height = getInteger(xmlComponent, "height", 0);
    }

    Point<int> position = getPosition(xmlComponent, height);
    Point<int> extends = position.translated(width, height);

    return Rectangle<int>(position, extends);
}


void Skin::placeComponent(
    const XmlElement *xmlComponent,
    Component *component)
{
    jassert(component != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    Rectangle<int> bounds = getBounds(xmlComponent);
    component->setBounds(bounds);
}


void Skin::placeMeterBar(
    const String &tagName,
    widgets::MeterBar *meterBar)
{
    jassert(meterBar != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);

    if (xmlComponent != nullptr)
    {
        int height = meterBar->getHeight();
        Point<int> position = getPosition(xmlComponent, height);

        meterBar->setTopLeftPosition(position);

        int segment_width = getInteger(xmlComponent, "segment_width", 0);
        bool isVertical = getBoolean(xmlComponent, "vertical", true);

        if (segment_width < 4)
        {
            Logger::outputDebugString(
                String("[Skin] segment width for \"") +
                tagName +
                "\" not set");

            segment_width = 8;
        }

        meterBar->setSegmentWidth(segment_width);

        // vertical bar
        if (isVertical)
        {
            meterBar->setOrientation(widgets::Orientation::vertical);
        }
        // horizontal bar
        else
        {
            meterBar->setOrientation(widgets::Orientation::horizontal);
        }
    }
}


void Skin::placeAndSkinButton(
    const String &tagName,
    ImageButton *button)
{
    jassert(button != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);

    if (xmlComponent != nullptr)
    {
        Image imageOn;
        String strImageFilenameOn = getString(xmlComponent, "image_on");

        loadImage(strImageFilenameOn, imageOn);

        Image imageOff;
        String strImageFilenameOff = getString(xmlComponent, "image_off");

        loadImage(strImageFilenameOff, imageOff);

        Image imageOver;
        String strImageFilenameOver = getString(xmlComponent, "image_over");

        // use "image_on" if "image_over" does not exist
        if (!strImageFilenameOver.isEmpty())
        {
            loadImage(strImageFilenameOver, imageOver);
        }
        else
        {
            imageOver = imageOn.createCopy();
            imageOver.multiplyAllAlphas(0.5f);
        }

        button->setImages(true, true, true,
                          imageOff, 1.0f, Colour(),
                          imageOver, 1.0f, Colour(),
                          imageOn, 1.0f, Colour(),
                          0.3f);

        int height = imageOn.getHeight();

        Point<int> position = getPosition(xmlComponent, height);
        button->setTopLeftPosition(position);
    }
}


void Skin::placeAndSkinSlider(
    const String &tagName,
    widgets::FrutSlider *slider)
{
    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);
    Colour sliderColour = getColour(xmlComponent);

    placeComponent(xmlComponent, slider);
    slider->setSliderColour(sliderColour);
}


void Skin::placeAndSkinSlider(
    const String &tagName,
    widgets::SliderCombined *slider)
{
    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);
    Colour sliderColour = getColour(xmlComponent);
    Colour toggleSwitchColour = getColour(xmlComponent, sliderColour, "toggle_");

    placeComponent(xmlComponent, slider);
    slider->setSliderColour(sliderColour);
    slider->setToggleSwitchColour(toggleSwitchColour);
}


void Skin::placeAndSkinNeedleMeter(
    const String &tagName,
    widgets::NeedleMeter *meter)
{
    jassert(meter != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);

    if (xmlComponent != nullptr)
    {
        Image imageBackground;
        String strImageFilenameBackground = getString(xmlComponent, "image");

        loadImage(strImageFilenameBackground, imageBackground);

        Image imageNeedle;
        String strImageFilenameNeedle = getString(xmlComponent, "image_needle");

        loadImage(strImageFilenameNeedle, imageNeedle);

        int spacing_left = getInteger(xmlComponent, "spacing_left", 0);
        int spacing_top = getInteger(xmlComponent, "spacing_top", 0);

        meter->setImages(imageBackground,
                         imageNeedle,
                         spacing_left,
                         spacing_top);

        int width = imageBackground.getWidth();
        int height = imageBackground.getHeight();

        Rectangle<int> bounds = getBounds(xmlComponent, width, height);
        meter->setBounds(bounds);
    }
}


void Skin::placeAndSkinLabel(
    const String &tagName,
    ImageComponent *label)
{
    jassert(label != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);

    if (xmlComponent != nullptr)
    {
        Image imageLabel;
        String strImageFilenameLabel = getString(xmlComponent, "image");

        loadImage(strImageFilenameLabel, imageLabel);
        label->setImage(imageLabel);

        int width = imageLabel.getWidth();
        int height = imageLabel.getHeight();

        Rectangle<int> bounds = getBounds(xmlComponent, width, height);
        label->setBounds(bounds);
    }
}


void Skin::placeAndSkinSignalLed(
    const String &tagName,
    widgets::SignalLed *label)
{
    jassert(label != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);

    if (xmlComponent != nullptr)
    {
        Image imageOff;
        String strImageFilenameOff = getString(xmlComponent, "image_off");

        loadImage(strImageFilenameOff, imageOff);

        Image imageLow;
        String strImageFilenameLow = getString(xmlComponent, "image_low");

        loadImage(strImageFilenameLow, imageLow);

        Image imageHigh;
        String strImageFilenameHigh = getString(xmlComponent, "image_high");

        loadImage(strImageFilenameHigh, imageHigh);

        label->setImages(imageOff, imageLow, imageHigh);

        int width = imageOff.getWidth();

        if ((width != imageLow.getWidth()) ||
                (width != imageHigh.getWidth()))
        {
            Logger::outputDebugString(
                String("[Skin] width of image files for \"") +
                tagName +
                "\" differs");
        }

        int height = imageOff.getHeight();

        if ((height != imageLow.getHeight()) ||
                (height != imageHigh.getHeight()))
        {
            Logger::outputDebugString(
                String("[Skin] height of image files for \"") +
                tagName +
                "\" differs");
        }

        Rectangle<int> bounds = getBounds(xmlComponent, width, height);
        label->setBounds(bounds);
    }
}


void Skin::placeAndSkinStateLabel(
    const String &tagName,
    widgets::StateLabel *label)
{
    jassert(label != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);

    if (xmlComponent != nullptr)
    {
        Image imageOff;
        String strImageFilenameOff = getString(xmlComponent, "image_off");

        loadImage(strImageFilenameOff, imageOff);

        Image imageOn;
        String strImageFilenameOn = getString(xmlComponent, "image_on");

        loadImage(strImageFilenameOn, imageOn);

        Image imageActive;
        String strImageFilenameActive = getString(xmlComponent, "image_active");

        // use "image_on" if "image_active" does not exist
        if (!strImageFilenameActive.isEmpty())
        {
            loadImage(strImageFilenameActive, imageActive);
        }
        else
        {
            imageActive = imageOn.createCopy();
        }

        int spacing_left = getInteger(xmlComponent, "spacing_left", 0);
        int spacing_top = getInteger(xmlComponent, "spacing_top", 0);
        int font_size = getInteger(xmlComponent, "font_size", 12);

        String strColourOff = getString(xmlComponent, "colour_off", "ffffff");
        String strColourOn = getString(xmlComponent, "colour_on", "ffffff");
        String strColourActive = getString(xmlComponent, "colour_active", "ffffff");

        label->setImages(imageOff,
                         imageOn,
                         imageActive,
                         strColourOff,
                         strColourOn,
                         strColourActive,
                         spacing_left,
                         spacing_top,
                         static_cast<float>(font_size));

        int width = imageOff.getWidth();

        if (width != imageActive.getWidth())
        {
            Logger::outputDebugString(
                String("[Skin] width of image files for \"") +
                tagName +
                "\" differs");
        }

        int height = imageOff.getHeight();

        if (height != imageActive.getHeight())
        {
            Logger::outputDebugString(
                String("[Skin] height of image files for \"") +
                tagName +
                "\" differs");
        }

        Rectangle<int> bounds = getBounds(xmlComponent, width, height);
        label->setBounds(bounds);
    }
}

}
}
