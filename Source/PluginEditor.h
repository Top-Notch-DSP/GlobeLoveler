/*
  ==============================================================================
    File:           PluginEditor.h
    Developers:     D. Robert Hoover and Kris Keillor
    Repository URL: https://github.com/Top-Notch-DSP/GlobeLoveler
    Date:           2024 Feb 1
    Forked From:    p-hlp
    Original URL:   https://github.com/p-hlp/SMPLComp/tree/master
    License:        GNU General Public License, version 3.0 (GPL-3.0)
  ==============================================================================
*/

#pragma once

#define SMPL_COMP_STANDALONE_DEMO_MODE 0

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "gui/include/LabeledSlider.h"
#include "gui/include/Meter.h"
#include "gui/include/MeterBackground.h"
#include "gui/include/SMPLCompLookAndFeel.h"

#if SMPL_COMP_STANDALONE_DEMO_MODE
    #include "gui/include/SMPCompStandaloneDemoPopup.h"
#endif

//==============================================================================
class GlobeLovelerEditor 
    : public AudioProcessorEditor, Timer, juce::AudioProcessorListener
{
public:
    GlobeLovelerEditor(GlobeLoveler& p, AudioProcessorValueTreeState& vts);
    ~GlobeLovelerEditor();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void initWidgets();

    //==============================================================================
    void audioProcessorChanged(AudioProcessor* source, const ChangeDetails& details) override;
    void audioProcessorParameterChanged(AudioProcessor* processor, int parameterIndex, float newValue) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GlobeLoveler& processor;
    AudioProcessorValueTreeState& valueTreeState;

#if GLOBE_LOVELER_DEMO_MODE
private:
    void initDemoMode();    // TODO: Make unique_ptr -KGK
    GlobeLovelerDemoPopup demoPopup;
#endif

    SMPLCompLookAndFeel LAF;

    Colour backgroundApp;

    // Widgets
    // * Compressor
    MeterBackground meterbg;
    Meter meter;
    LabeledSlider inGainLSlider;
    LabeledSlider makeupGainLSlider;
    LabeledSlider threshLSlider;
    LabeledSlider ratioLSlider;
    LabeledSlider kneeLSlider;
    LabeledSlider attackLSlider;
    LabeledSlider releaseLSlider;
    LabeledSlider mixLSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobeLovelerEditor)
};
