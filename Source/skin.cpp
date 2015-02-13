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

#include "skin.h"


Skin::Skin(File &fileSkin, int nNumChannels, int nCrestFactor, int nMeterType)
{
    fileResourcePath = nullptr;
    xml = nullptr;

    updateSkin(nNumChannels, nCrestFactor, nMeterType);
    loadFromXml(fileSkin);
}


Skin::~Skin()
{
    if (fileResourcePath != nullptr)
    {
        delete fileResourcePath;
        fileResourcePath = nullptr;
    }

    if (xml != nullptr)
    {
        delete xml;
        xml = nullptr;
    }
}


bool Skin::loadFromXml(File &fileSkin)
{
    DBG(String("[Skin] loading file \"") + fileSkin.getFileName() + "\"");

    if (fileResourcePath != nullptr)
    {
        delete fileResourcePath;
        fileResourcePath = nullptr;
    }

    if (xml != nullptr)
    {
        delete xml;
        xml = nullptr;
    }

    xml = XmlDocument::parse(fileSkin);

    xmlSkinGroup = nullptr;
    xmlSkinFallback_1 = nullptr;
    xmlSkinFallback_2 = nullptr;

    if (xml == nullptr)
    {
        Logger::outputDebugString(String("[Skin] file \"") + fileSkin.getFullPathName() + "\" not found");
        return false;
    }
    else if ((!xml->hasTagName("trakmeter-skin")) || (xml->getChildByName("default") == nullptr))
    {
        Logger::outputDebugString("[Skin] XML file not valid");

        delete xml;
        xml = nullptr;

        return false;
    }
    else
    {
        xmlSkinGroup = xml->getChildByName(strSkinGroup);
        xmlSkinFallback_1 = xml->getChildByName(strSkinFallback_1);
        xmlSkinFallback_2 = xml->getChildByName("default");

        String strResourcePath = xml->getStringAttribute("path");
        fileResourcePath = new File(fileSkin.getSiblingFile(strResourcePath));

        if (!fileResourcePath->isDirectory())
        {
            Logger::outputDebugString(String("[Skin] directory \"") + fileResourcePath->getFullPathName() + "\" not found");

            delete fileResourcePath;
            fileResourcePath = nullptr;

            delete xml;
            xml = nullptr;

            return false;
        }

        return true;
    }
}


void Skin::updateSkin(int nNumChannels, int nCrestFactor, int nMeterType)
{
    jassert(nNumChannels > 0);
    nNumberOfChannels = nNumChannels;

    strBackgroundSelector = "image";

    if (nNumberOfChannels <= 2)
    {
        strSkinFallback_1 = "stereo";
    }
    else
    {
        strSkinFallback_1 = "multi";
    }

    if (nMeterType == TraKmeterPluginParameters::selSplitMeters)
    {
        strSkinFallback_1 += "_split";
    }
    else
    {
        strSkinFallback_1 += "_combined";
    }

    switch (nCrestFactor)
    {
    case 20:
        strSkinGroup = strSkinFallback_1 + "_k20";
        break;

    case 14:
        strSkinGroup = strSkinFallback_1 + "_k14";
        break;

    case 12:
        strSkinGroup = strSkinFallback_1 + "_k12";
        break;

    default:
        strSkinGroup = strSkinFallback_1 + "_normal";
        break;
    }

    if (xml != nullptr)
    {
        xmlSkinGroup = xml->getChildByName(strSkinGroup);
        xmlSkinFallback_1 = xml->getChildByName(strSkinFallback_1);
        xmlSkinFallback_2 = xml->getChildByName("default");
    }
    else
    {
        xmlSkinGroup = nullptr;
        xmlSkinFallback_1 = nullptr;
        xmlSkinFallback_2 = nullptr;
    }
}


XmlElement *Skin::getComponentFromXml(String strXmlTag)
{
    XmlElement *xmlComponent;

    // suppress unnecessary warnings and save some time
    if (xml == nullptr)
    {
        xmlComponent = nullptr;
    }
    else if ((xmlSkinGroup != nullptr) && (xmlSkinGroup->getChildByName(strXmlTag) != nullptr))
    {
        xmlComponent = xmlSkinGroup->getChildByName(strXmlTag);
    }
    else if ((xmlSkinFallback_1 != nullptr) && (xmlSkinFallback_1->getChildByName(strXmlTag) != nullptr))
    {
        xmlComponent = xmlSkinFallback_1->getChildByName(strXmlTag);
    }
    else if ((xmlSkinFallback_2 != nullptr) && (xmlSkinFallback_2->getChildByName(strXmlTag) != nullptr))
    {
        xmlComponent = xmlSkinFallback_2->getChildByName(strXmlTag);
    }
    else
    {
        Logger::outputDebugString(String("[Skin] XML element \"") + strXmlTag + "\" not found");
        xmlComponent = nullptr;
    }

    return xmlComponent;
}


void Skin::placeAndSkinButton(ImageButton *button, String strXmlTag)
{
    jassert(button != nullptr);

    XmlElement *xmlButton = getComponentFromXml(strXmlTag);

    if (xmlButton != nullptr)
    {
        int x = xmlButton->getIntAttribute("x", -1);
        int y = xmlButton->getIntAttribute("y", -1);

        String strImageOn = xmlButton->getStringAttribute("image_on");
        File fileImageOn = fileResourcePath->getChildFile(strImageOn);
        Image imageOn;

        if (!fileImageOn.existsAsFile())
        {
            Logger::outputDebugString(String("[Skin] image file \"") + fileImageOn.getFullPathName() + "\" not found");
            imageOn = Image();
        }
        else
        {
            imageOn = ImageFileFormat::loadFrom(fileImageOn);
        }

        String strImageOff = xmlButton->getStringAttribute("image_off");
        File fileImageOff = fileResourcePath->getChildFile(strImageOff);
        Image imageOff;

        if (!fileImageOff.existsAsFile())
        {
            Logger::outputDebugString(String("[Skin] image file \"") + fileImageOff.getFullPathName() + "\" not found");
            imageOff = Image();
        }
        else
        {
            imageOff = ImageFileFormat::loadFrom(fileImageOff);
        }

        button->setImages(true, true, true,
                          imageOff, 1.0f, Colour(),
                          imageOn, 0.5f, Colour(),
                          imageOn, 1.0f, Colour(),
                          0.3f);
        button->setTopLeftPosition(x, y);
    }
}


void Skin::placeAndSkinLabel(ImageComponent *label, String strXmlTag)
{
    jassert(label != nullptr);

    XmlElement *xmlLabel = getComponentFromXml(strXmlTag);

    if (xmlLabel != nullptr)
    {
        int x = xmlLabel->getIntAttribute("x", -1);
        int y = xmlLabel->getIntAttribute("y", -1);
        int width = xmlLabel->getIntAttribute("width", -1);
        int height = xmlLabel->getIntAttribute("height", -1);

        String strImage = xmlLabel->getStringAttribute("image");
        File fileImage = fileResourcePath->getChildFile(strImage);
        Image imageLabel;

        if (!fileImage.existsAsFile())
        {
            Logger::outputDebugString(String("[Skin] image file \"") + fileImage.getFullPathName() + "\" not found");
            imageLabel = Image();
        }
        else
        {
            imageLabel = ImageFileFormat::loadFrom(fileImage);
        }

        label->setImage(imageLabel);
        label->setBounds(x, y, width, height);
    }
}


void Skin::placeAndSkinStateLabel(StateLabel *label, String strXmlTag)
{
    jassert(label != nullptr);

    XmlElement *xmlLabel = getComponentFromXml(strXmlTag);

    if (xmlLabel != nullptr)
    {
        int x = xmlLabel->getIntAttribute("x", -1);
        int y = xmlLabel->getIntAttribute("y", -1);
        int width = xmlLabel->getIntAttribute("width", -1);
        int height = xmlLabel->getIntAttribute("height", -1);

        int spacing_left = xmlLabel->getIntAttribute("spacing_left", 0);
        int spacing_top = xmlLabel->getIntAttribute("spacing_top", 0);
        int font_size = xmlLabel->getIntAttribute("font_size", 12);

        String strColourOn = xmlLabel->getStringAttribute("colour_on", "ffffff");
        String strColourActive = xmlLabel->getStringAttribute("colour_active", "ffffff");

        String strImageOff = xmlLabel->getStringAttribute("image_off");
        File fileImageOff = fileResourcePath->getChildFile(strImageOff);
        Image imageOff;

        if (!fileImageOff.existsAsFile())
        {
            Logger::outputDebugString(String("[Skin] image file \"") + fileImageOff.getFullPathName() + "\" not found");
            imageOff = Image();
        }
        else
        {
            imageOff = ImageFileFormat::loadFrom(fileImageOff);
        }

        String strImageOn = xmlLabel->getStringAttribute("image_on");
        File fileImageOn = fileResourcePath->getChildFile(strImageOn);
        Image imageOn;

        if (!fileImageOn.existsAsFile())
        {
            Logger::outputDebugString(String("[Skin] image file \"") + fileImageOn.getFullPathName() + "\" not found");
            imageOn = Image();
        }
        else
        {
            imageOn = ImageFileFormat::loadFrom(fileImageOn);
        }

        // will use "image_on" if "image_active" does not exist
        String strImageActive = xmlLabel->getStringAttribute("image_active", strImageOn);
        File fileImageActive = fileResourcePath->getChildFile(strImageActive);
        Image imageActive;

        if (!fileImageActive.existsAsFile())
        {
            Logger::outputDebugString(String("[Skin] image file \"") + fileImageActive.getFullPathName() + "\" not found");
            imageActive = Image();
        }
        else
        {
            imageActive = ImageFileFormat::loadFrom(fileImageActive);
        }

        label->setImages(imageOff, imageOn, imageActive, strColourOn, strColourActive, spacing_left, spacing_top, font_size);
        label->setBounds(x, y, width, height);
    }
}


void Skin::placeComponent(Component *component, String strXmlTag)
{
    jassert(component != nullptr);

    XmlElement *xmlComponent = getComponentFromXml(strXmlTag);

    if (xmlComponent != nullptr)
    {
        int x = xmlComponent->getIntAttribute("x", -1);
        int y = xmlComponent->getIntAttribute("y", -1);
        int width = xmlComponent->getIntAttribute("width", -1);
        int height = xmlComponent->getIntAttribute("height", -1);

        component->setBounds(x, y, width, height);
    }
}


void Skin::setBackgroundImage(ImageComponent *background, AudioProcessorEditor *editor)
{
    if (xmlSkinGroup != nullptr)
    {
        Image imageBackground;

        XmlElement *xmlBackground = xmlSkinGroup->getChildByName("background");

        if (xmlBackground == nullptr)
        {
            Logger::outputDebugString(String("[Skin] XML element \"") + strSkinGroup + "\" specifies no background image");
            imageBackground = Image();
        }
        else
        {
            String strImage = xmlBackground->getStringAttribute(strBackgroundSelector);
            File fileImage = fileResourcePath->getChildFile(strImage);

            if (!fileImage.existsAsFile())
            {
                Logger::outputDebugString(String("[Skin] image file \"") + fileImage.getFullPathName() + "\" not found");
                imageBackground = Image();
            }
            else
            {
                imageBackground = ImageFileFormat::loadFrom(fileImage);
            }
        }

        XmlElement *xmlMeterGraduation = nullptr;

        // get rid of the "unused variable" warning
        (void) xmlMeterGraduation;

        forEachXmlChildElementWithTagName(*xmlSkinGroup, xmlMeterGraduation, "meter_graduation")
        {
            String strImage = xmlMeterGraduation->getStringAttribute(strBackgroundSelector);
            File fileImage = fileResourcePath->getChildFile(strImage);

            if (!fileImage.existsAsFile())
            {
                Logger::outputDebugString(String("[Skin] image file \"") + fileImage.getFullPathName() + "\" not found");
            }
            else
            {
                Image imageMeterGraduation = ImageFileFormat::loadFrom(fileImage);

                int x = xmlMeterGraduation->getIntAttribute("x", -1);
                int y = xmlMeterGraduation->getIntAttribute("y", -1);

                Graphics g(imageBackground);
                g.drawImageAt(imageMeterGraduation, x, y, false);
            }
        }

        int nBackgroundWidth = imageBackground.getWidth();
        int nBackgroundHeight = imageBackground.getHeight();

        background->setImage(imageBackground);
        background->setBounds(0, 0, nBackgroundWidth, nBackgroundHeight);

        // moves background image to the back of the editor's z-plane
        // so that it doesn't overlay (and thus block) any other
        // components
        background->toBack();

        editor->setSize(nBackgroundWidth, nBackgroundHeight);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
