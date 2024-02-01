/*
  ==============================================================================
    File:           SMPLCompStandaloneInfoPopup.cpp
    Developers:     D. Robert Hoover and Kris Keillor
    Repository URL: https://github.com/Top-Notch-DSP/GlobeLoveler
    Date:           2024 Feb 1
    Forked From:    p-hlp
    Original URL:   https://github.com/p-hlp/SMPLComp/tree/master
    License:        GNU General Public License, version 3.0 (GPL-3.0)
  ==============================================================================
*/

#include "include/SMPLCompStandaloneInfoPopup.h"
#include "../Source/util/Constants.h"

//==============================================================================
GlobeLovelerInfoPopup::GlobeLovelerInfoPopup()
    : Component("GlobeLoveler Information"),
    // Labels
    infoLabel("InfoPopupLabel", "GlobeLoveler (C) D. Robert Hoover and Kristopher G. Keillor 2023-2024."),
        gnuLabel("GnuLabel", "Registered under the GNU General Public License, Version 3, 29 June 2007:"),
        resourcesLabel("ResourcesLink", "The following resources were used:"),
        globeInstituteLabel("GlobeInstituteLabel", "Globe Institute of the Sound Healing Center:"),
        smplCompLabel("SmplCompLabel", "SmplComp Software by p-hlp:"),
        juceLabel("JuceLabel", "JUCE Framework by ROLI Ltd. (v7.0.9):"),
        sourceLabel("SourceLabel", "Source code available at:"),
    // Links
        globeInstituteLink("Globe Institute", juce::URL("https://soundhealingcenter.com/globe-institute/")),
        gnuLink("GNU Public License", juce::URL("https://www.gnu.org/licenses/gpl-3.0.en.html")),
        smplCompLink("SMPLComp", juce::URL("https://github.com/p-hlp/SMPLComp")),
        juceLink("JUCE", juce::URL("https://juce.com/")),
        sourceLink("GitHub", juce::URL("https://github.com/Top-Notch-DSP/GlobeLoveler"))
{
    // Set a fixed size in the center of the screen
    setSize(Constants::Containers::PopupWidth, Constants::Containers::PopupHeight);
    centreWithSize(getWidth(), getHeight());
    setVisible(true);
    // Bring to front 
    setAlwaysOnTop(true);

    // Initialize the info text labels -KGK
    addAndMakeVisible(infoLabel);
    infoLabel.setJustificationType(Justification::centredTop);

    // Initialize the license text label and link, and resources text -KGK
    addAndMakeVisible(gnuLabel);
    addAndMakeVisible(&gnuLink);
    addAndMakeVisible(resourcesLabel);
    resourcesLabel.setJustificationType(Justification::centredTop);

    // Initialize the body text label and link -KGK
    addAndMakeVisible(globeInstituteLabel);
    addAndMakeVisible(&globeInstituteLink);

    // Initialize the body text label and link -KGK
    addAndMakeVisible(smplCompLabel);
    addAndMakeVisible(&smplCompLink);

    // Initialize the body text label and link -KGK
    addAndMakeVisible(&juceLabel);
    addAndMakeVisible(&juceLink);

    // Initialize the source text label and link -KGK
    addAndMakeVisible(&sourceLabel);
    addAndMakeVisible(&sourceLink);

    // Position the links and labels evenly throughout the window with a buffer set by: Constants::Margins::big -KGK
    // * infoLabel
    infoLabel.setBounds(Constants::Margins::big, Constants::Margins::big, getWidth() - Constants::Margins::big * 2, 36);
    // * gnuLabel and gnuLink
    gnuLabel.setBounds(Constants::Margins::big, infoLabel.getBottom() + Constants::Margins::big, getWidth() - Constants::Margins::big * 2, 36);
    gnuLink.setBounds(Constants::Margins::big, gnuLabel.getBottom(), getWidth() - Constants::Margins::big * 2, 20);
    // * resourcesLabel
    resourcesLabel.setBounds(Constants::Margins::big, gnuLink.getBottom() + Constants::Margins::big, getWidth() - Constants::Margins::big * 2, 16);
    // * globeInstituteLabel and globeInstituteLink
    globeInstituteLabel.setBounds(Constants::Margins::big, resourcesLabel.getBottom() + Constants::Margins::small, getWidth() - Constants::Margins::big * 2, 16);
    globeInstituteLink.setBounds(Constants::Margins::big, globeInstituteLabel.getBottom(), getWidth() - Constants::Margins::big * 2, 20);
    // * smplCompLabel and smplCompLink
    smplCompLabel.setBounds(Constants::Margins::big, globeInstituteLink.getBottom() + Constants::Margins::small, getWidth() - Constants::Margins::big * 2, 16);
    smplCompLink.setBounds(Constants::Margins::big, smplCompLabel.getBottom(), getWidth() - Constants::Margins::big * 2, 20);
    // * juceLabel and juceLink
    juceLabel.setBounds(Constants::Margins::big, smplCompLink.getBottom() + Constants::Margins::small, getWidth() - Constants::Margins::big * 2, 16);
    juceLink.setBounds(Constants::Margins::big, juceLabel.getBottom(), getWidth() - Constants::Margins::big * 2, 20);
    // * sourceLabel and sourceLink
    sourceLabel.setBounds(Constants::Margins::big, juceLink.getBottom() + Constants::Margins::small, getWidth() - Constants::Margins::big * 2, 16);
    sourceLink.setBounds(Constants::Margins::big, sourceLabel.getBottom(), getWidth() - Constants::Margins::big * 2, 20);
}

//==============================================================================
GlobeLovelerInfoPopup::~GlobeLovelerInfoPopup()
{
}