/*
  ==============================================================================
    File:           SMPLCompStandaloneDemoPopup.h
    Developers:     D. Robert Hoover and Kris Keillor
    Repository URL: https://github.com/Top-Notch-DSP/GlobeLoveler
    Date:           2024 Feb 1
    Forked From:    p-hlp
    Original URL:   https://github.com/p-hlp/SMPLComp/tree/master
    License:        GNU General Public License, version 3.0 (GPL-3.0)
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Source/util/Constants.h"

//==============================================================================

class GlobeLovelerDemoPopup : public Component, Timer
{
public:
    // Constructor and destructor -KGK
    GlobeLovelerDemoPopup();
    ~GlobeLovelerDemoPopup();

    // Paint methods -KGK
    void resized() override;
    void paint(Graphics& g) override;

    // Timer callback -KGK
    void timerCallback() override;

private:
    // Labels for title and body text -KGK
    Label titleLabel;
    Label bodyLabel;
    HyperlinkButton GlobeInstituteLink;
    void openGlobeInstituteLink();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobeLovelerDemoPopup)
};