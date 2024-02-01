/*
  ==============================================================================
    File:           SMPLCompStandaloneInfoPopup.h
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

class GlobeLovelerInfoPopup : public Component
{
public:
    GlobeLovelerInfoPopup();
	~GlobeLovelerInfoPopup();

private:
    // Unique pointer to content component -KGK
    Label infoLabel, gnuLabel, resourcesLabel, globeInstituteLabel, smplCompLabel, juceLabel, sourceLabel;
    HyperlinkButton globeInstituteLink, gnuLink, smplCompLink, juceLink, sourceLink;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobeLovelerInfoPopup)
};

