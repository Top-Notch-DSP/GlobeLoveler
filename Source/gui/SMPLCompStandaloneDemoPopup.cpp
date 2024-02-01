/*
  ==============================================================================
    File:           SMPLCompStandaloneDemoPopup.cpp
    Developers:     D. Robert Hoover and Kris Keillor
    Repository URL: https://github.com/Top-Notch-DSP/GlobeLoveler
    Date:           2024 Feb 1
    Forked From:    p-hlp
    Original URL:   https://github.com/p-hlp/SMPLComp/tree/master
    License:        GNU General Public License, version 3.0 (GPL-3.0)
  ==============================================================================
*/

#include "include/SMPLCompStandaloneDemoPopup.h"

//==============================================================================
GlobeLovelerDemoPopup::GlobeLovelerDemoPopup()
{
    setSize(Constants::Containers::WindowWidth, Constants::Containers::WindowHeight);

    // Initialize the title text labels -KGK
    addAndMakeVisible(titleLabel);
    titleLabel.setText("GlobeLoveler Demonstration Version", dontSendNotification);
    titleLabel.setJustificationType(Justification::centredTop);
    titleLabel.setFont(titleLabel.getFont().withHeight(32.0f));

    // Initialize the body text label -KGK
    addAndMakeVisible(bodyLabel);
    bodyLabel.setJustificationType(Justification::centredTop);
    bodyLabel.setText("GlobeLoveler was developed by D. Robert Hoover and Kristopher G. Keillor for the Globe Institute of the Sound Healing Center.\nYou are currently using a demonstration version of the software, with a limited ten-minute evaluation period.\n\nContact Sound Healing Institute for the\nfull version of the software.", dontSendNotification);

    // Initialize the link to the Globe Institute -KGK
    addAndMakeVisible(GlobeInstituteLink);
    GlobeInstituteLink.setButtonText("Globe Institute webpage");
    GlobeInstituteLink.setURL(juce::URL("https://soundhealingcenter.com/globe-institute/"));
    GlobeInstituteLink.onClick = [this] { openGlobeInstituteLink(); };

    // Start a 10 minute timer for the demo version -KGK
    startTimer(1000 * 60 * 10); // 10 minutes in ms
    //startTimer(1000 * 5); // 5 seconds in ms
}

//==============================================================================
GlobeLovelerDemoPopup::~GlobeLovelerDemoPopup()
{
	// Remove the title text label -KGK
	removeChildComponent(&titleLabel);

	// Remove the body text label -KGK
	removeChildComponent(&bodyLabel);

	// Stop the timer -KGK
	stopTimer();
}

//==============================================================================
void GlobeLovelerDemoPopup::resized()
{
    // Set the bounds for the title label
    titleLabel.setBounds(40, 40, getWidth() - 80, 40);

    // Set the bounds for the body label
    bodyLabel.setBounds(40, 60, getWidth() - 80, 100);

    GlobeInstituteLink.setBounds(20, 180, getWidth() - 40, 40);
}

//==============================================================================
void GlobeLovelerDemoPopup::paint(juce::Graphics& g)
{
    // Set the background color or draw any content for the opaque component
    g.fillAll(juce::Colours::black.withAlpha(0.7f)); // Adjust alpha for opacity
}

//==============================================================================
void GlobeLovelerDemoPopup::timerCallback()
{
	// If the timer has expired, open the popup -KGK
	setVisible(true);
	stopTimer();
}

//==============================================================================
void GlobeLovelerDemoPopup::openGlobeInstituteLink()
{
    bool launched = false;

    juce::URL globeInstituteURL(GlobeInstituteLink.getURL());
    if (globeInstituteURL.isWellFormed()) {
        launched = globeInstituteURL.launchInDefaultBrowser();
    }

    DBG(String::formatted("Launched Globe Institute URL: %s", launched ? "true" : "false"));
}