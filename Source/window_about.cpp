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

#include "window_about.h"


WindowAbout::WindowAbout(Component *pEditorWindow)
    : DocumentWindow(String("About ") + ProjectInfo::projectName, Colours::white, 0, true)
    // create new window child
{
    int nWidth = 270;
    int nHeight = 540;

    // set dimensions to those passed to the function ...
    setSize(nWidth, nHeight + getTitleBarHeight());

    // ... center window on editor ...
    centreAroundComponent(pEditorWindow, getWidth(), getHeight());

    // ... and keep the new window on top
    setAlwaysOnTop(true);

    // this window does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    // empty windows are boring, so let's prepare a space for some
    // window components
    contentComponent = new Component("Window Area");
    setContentOwned(contentComponent, false);

    // create a word-wrapping read-only text editor component with
    // multiple lines for displaying information about the plug-in
    TextEditorAbout = new TextEditor("About Text");
    TextEditorAbout->setMultiLine(true, true);
    TextEditorAbout->setReadOnly(true);

    // initialise colours of the text editor component
    TextEditorAbout->setColour(TextEditor::backgroundColourId, Colours::black.withAlpha(0.25f));
    TextEditorAbout->setColour(TextEditor::textColourId, Colours::black);
    TextEditorAbout->setColour(TextEditor::highlightColourId, Colours::black.withAlpha(0.15f));
    TextEditorAbout->setColour(TextEditor::highlightedTextColourId, Colours::black);

    // set up two fonts, one for headlines and one for regular text
    Font fontHeadline(16.0f, Font::bold);
    Font fontRegular(15.0f, Font::plain);

    // display plug-in name and version number
    TextEditorAbout->setFont(fontHeadline);
    TextEditorAbout->insertTextAtCaret(
        String(ProjectInfo::projectName) + " " + JucePlugin_VersionString + "\n"
    );

    // display plug-in description
    TextEditorAbout->setFont(fontRegular);
    TextEditorAbout->insertTextAtCaret(
        String(JucePlugin_Desc) + ".\n\n"
    );

    // display copyright notice
    TextEditorAbout->setFont(fontHeadline);
    TextEditorAbout->insertTextAtCaret(
        "Copyright\n"
    );

    TextEditorAbout->setFont(fontRegular);
    TextEditorAbout->insertTextAtCaret(
        "(c) 2012-2015 Martin Zuther\n\n"
    );

    // display the contributors
    TextEditorAbout->setFont(fontHeadline);
    TextEditorAbout->insertTextAtCaret(
        "Contributors\n"
    );

    TextEditorAbout->setFont(fontRegular);
    TextEditorAbout->insertTextAtCaret(
        L"Filipe Coelho\n"
        L"Bram de Jong\n\n"
    );

    // display the beta testers
    TextEditorAbout->setFont(fontHeadline);
    TextEditorAbout->insertTextAtCaret(
        "Beta testing\n"
    );

    TextEditorAbout->setFont(fontRegular);
    TextEditorAbout->insertTextAtCaret(
        L"Rickard (Interfearing Sounds)\n\n"
    );

    // display thanks
    TextEditorAbout->setFont(fontHeadline);
    TextEditorAbout->insertTextAtCaret(
        "Thanks\n"
    );

    TextEditorAbout->setFont(fontRegular);
    TextEditorAbout->insertTextAtCaret(
        L"I want to thank all contributors "
        L"and beta testers and the open source "
        L"community at large!\n\n"
        L"Thank you for using free software!\n\n"
    );

    // display used libraries
    TextEditorAbout->setFont(fontHeadline);
    TextEditorAbout->insertTextAtCaret(
        "Libraries\n"
    );

    TextEditorAbout->setFont(fontRegular);
    TextEditorAbout->insertTextAtCaret(
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
        L"\n"
    );

#if (JUCE_USE_VSTSDK_2_4 != 0)

    // display trademarks (but only when necessary)
    TextEditorAbout->setFont(fontHeadline);
    TextEditorAbout->insertTextAtCaret(
        "Trademarks\n"
    );

    TextEditorAbout->setFont(fontRegular);
    TextEditorAbout->insertTextAtCaret(
        "VST PlugIn Technology by Steinberg\n\n"
    );

#endif

#if (JUCE_ASIO != 0)

    // display trademarks (but only when necessary)
    TextEditorAbout->setFont(fontHeadline);
    TextEditorAbout->insertTextAtCaret(
        "Trademarks\n"
    );

    TextEditorAbout->setFont(fontRegular);
    TextEditorAbout->insertTextAtCaret(
        "ASIO Technology by Steinberg Media Technologies GmbH\n\n"
    );

#endif

    // display license summary
    TextEditorAbout->setFont(fontHeadline);
    TextEditorAbout->insertTextAtCaret(
        "License\n"
    );

    TextEditorAbout->setFont(fontRegular);
    TextEditorAbout->insertTextAtCaret(
        L"This program is free software: you can redistribute it and/or modify "
        L"it under the terms of the GNU General Public License as published by "
        L"the Free Software Foundation, either version 3 of the License, or "
        L"(at your option) any later version.\n\n"

        L"This program is distributed in the hope that it will be useful, "
        L"but WITHOUT ANY WARRANTY; without even the implied warranty of "
        L"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
        L"GNU General Public License for more details.\n\n"

        L"You should have received a copy of the GNU General Public License "
        L"along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n"

        L"Thank you for using free software!\n"
    );

    // in case the text has become too long to fit into the text
    // editor component, scroll to the beginning
    TextEditorAbout->setCaretPosition(0);
    TextEditorAbout->scrollEditorToPositionCaret(0, 0);

    // hide cursor
    TextEditorAbout->setCaretVisible(false);

    // finally, position and display the text editor component
    TextEditorAbout->setBounds(0, 0, nWidth, nHeight - 47);
    contentComponent->addAndMakeVisible(TextEditorAbout);

    // create and position an "close" button
    ButtonClose = new TextButton("Close");
    ButtonClose->setBounds(nWidth - 70, nHeight - 34, 60, 20);
    ButtonClose->setColour(TextButton::buttonColourId, Colours::yellow);
    ButtonClose->setColour(TextButton::buttonOnColourId, Colours::yellow);

    // add "about" window as button listener and display the button
    ButtonClose->addListener(this);
    contentComponent->addAndMakeVisible(ButtonClose);

    // create and position the image button which opens the license
    // text in a web browser
    ButtonGpl = new ImageButton("GPL Link");
    ButtonGpl->setBounds(6, nHeight - 41, 64, 32);
    ButtonGpl->setImages(true, false, true,
                         ImageCache::getFromMemory(
                             resources::button_gpl_normal_png,
                             resources::button_gpl_normal_pngSize),
                         1.0f, Colour(),
                         ImageCache::getFromMemory(
                             resources::button_gpl_down_png,
                             resources::button_gpl_down_pngSize),
                         1.0f, Colour(),
                         ImageCache::getFromMemory(
                             resources::button_gpl_down_png,
                             resources::button_gpl_down_pngSize),
                         1.0f, Colour());

    // add "about" window as button listener and display the button
    ButtonGpl->addListener(this);
    contentComponent->addAndMakeVisible(ButtonGpl);

    // finally, display window
    setVisible(true);
}


WindowAbout::~WindowAbout()
{
    // delete all children of the window; "contentComponent" will be
    // deleted by the base class, so please leave it alone!
    contentComponent->deleteAllChildren();
}


void WindowAbout::buttonClicked(Button *button)
{
    // find out which button has been clicked
    if (button == ButtonClose)
    {
        // close window by making it invisible
        setVisible(false);
    }
    else if (button == ButtonGpl)
    {
        // open license text in default web browser
        URL("http://www.gnu.org/licenses/gpl-3.0.html").launchInDefaultBrowser();
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
