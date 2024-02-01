/*
  ==============================================================================
    File:           PluginEditor.cpp
    Developers:     D. Robert Hoover and Kris Keillor
    Repository URL: https://github.com/Top-Notch-DSP/GlobeLoveler
    Date:           2024 Feb 1
    Forked From:    p-hlp
    Original URL:   https://github.com/p-hlp/SMPLComp/tree/master
    License:        GNU General Public License, version 3.0 (GPL-3.0)
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GlobeLovelerEditor::GlobeLovelerEditor(GlobeLoveler& p, AudioProcessorValueTreeState& vts)
    #pragma region Initializers
        : AudioProcessorEditor(&p), processor(p), valueTreeState(vts), backgroundApp(Colour(Constants::Colors::bg_App)),
          inGainLSlider(this), makeupGainLSlider(this),
          threshLSlider(this), ratioLSlider(this), kneeLSlider(this), attackLSlider(this), releaseLSlider(this),
          mixLSlider(this)
    #pragma endregion
{
    // Add this editor as a listener to the audio processor -KGK
    p.addListener(this);
    // Set the look and feel -KGK
    setLookAndFeel(&LAF);
    // Init widgets -KGK
    initWidgets();
    // Set window size
    setSize(Constants::Containers::WindowWidth, Constants::Containers::WindowHeight);

    #if GLOBE_LOVELER_DEMO_MODE
        // Start demo mode
        initDemoMode();
    #endif

    startTimerHz(60);
}

//==============================================================================
GlobeLovelerEditor::~GlobeLovelerEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void GlobeLovelerEditor::paint(Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(backgroundApp);
}

//==============================================================================
void GlobeLovelerEditor::resized()
{
    auto area = getLocalBounds().reduced(Constants::Margins::big);
    const auto areaHeight = area.getHeight();
    const auto areaWidth = area.getWidth();
    // Rectangles created from area in bottom-to-top order -KGK
    #if GLOBE_REVERB
        #ifdef JUCE_DEBUG
            auto combAreaClusterToggles = area.removeFromBottom(Constants::Containers::ToggleHeight);
            auto combAreaSecondRow = area.removeFromBottom(Constants::Containers::ButtonHeight).reduced(Constants::Margins::small);
            auto combAreaPrimaryRow = area.removeFromBottom(Constants::Containers::ButtonHeight).reduced(Constants::Margins::small);
            auto allPassAreaButtons = area.removeFromBottom(Constants::Containers::ToggleHeight).reduced(Constants::Margins::small);
        #endif
        auto reverbBtnArea = area.removeFromBottom(Constants::Containers::ButtonHeight).reduced(Constants::Margins::small);
    #endif
    auto lBtnArea = area.removeFromLeft(Constants::Containers::ButtonWidth).reduced(Constants::Margins::small);
    auto rBtnArea = area.removeFromRight(Constants::Containers::ButtonWidth).reduced(Constants::Margins::small);
    auto botBtnArea = area.removeFromBottom(Constants::Containers::ButtonHeight).reduced(Constants::Margins::medium);

    const FlexItem::Margin knobMargin = FlexItem::Margin(Constants::Margins::small);
    const FlexItem::Margin knobMarginSmall = FlexItem::Margin(Constants::Margins::medium);

    FlexBox leftBtnBox;
    leftBtnBox.flexWrap = FlexBox::Wrap::noWrap;
    leftBtnBox.flexDirection = FlexBox::Direction::column;
    leftBtnBox.justifyContent = FlexBox::JustifyContent::spaceAround;
    leftBtnBox.items.add(FlexItem(attackLSlider).withFlex(1).withMargin(knobMarginSmall));
    leftBtnBox.items.add(FlexItem(releaseLSlider).withFlex(1).withMargin(knobMarginSmall));
    leftBtnBox.items.add(FlexItem(inGainLSlider).withFlex(1).withMargin(knobMarginSmall));
    leftBtnBox.performLayout(lBtnArea.toFloat());

    FlexBox rightBtnBox;
    rightBtnBox.flexWrap = FlexBox::Wrap::noWrap;
    rightBtnBox.flexDirection = FlexBox::Direction::column;
    rightBtnBox.justifyContent = FlexBox::JustifyContent::spaceAround;
    rightBtnBox.items.add(FlexItem(kneeLSlider).withFlex(1).withMargin(knobMarginSmall));
    rightBtnBox.items.add(FlexItem(ratioLSlider).withFlex(1).withMargin(knobMarginSmall));
    rightBtnBox.items.add(FlexItem(mixLSlider).withFlex(1).withMargin(knobMarginSmall));
    rightBtnBox.performLayout(rBtnArea.toFloat());

    FlexBox botBtnBox;
    botBtnBox.flexWrap = FlexBox::Wrap::noWrap;
    botBtnBox.flexDirection = FlexBox::Direction::row;
    botBtnBox.justifyContent = FlexBox::JustifyContent::spaceAround;
    botBtnBox.items.add(FlexItem(threshLSlider).withFlex(1));
    botBtnBox.items.add(FlexItem(makeupGainLSlider).withFlex(1));
    botBtnBox.performLayout(botBtnArea.toFloat());

    FlexBox meterBox;
    meterBox.flexWrap = FlexBox::Wrap::noWrap;
    meterBox.justifyContent = FlexBox::JustifyContent::spaceAround;
    meterBox.items.add(FlexItem(meter).withFlex(1).withMargin(Constants::Margins::big));
    meterBox.performLayout(area.toFloat());
}

//==============================================================================
void GlobeLovelerEditor::timerCallback()
{
    int m = meter.getMode();
    switch (m)
    {
    case Meter::Mode::IN:
        //DBG("IN: " << processor.currentInput.get());
        meter.update(processor.currentInput.get());
        break;
    case Meter::Mode::OUT:
        //DBG("OUT: " << processor.currentOutput.get());
        meter.update(processor.currentOutput.get());
        break;
    case Meter::Mode::GR:
        //DBG("GR: " << processor.gainReduction.get());
        meter.update(processor.gainReduction.get());
        break;
    default:
        break;
    }
}

// audioProcessorChanged and audioProcessorParameterChanged methods -KGK
//==============================================================================
// Triggered in globbeLoveler.setStateInformation() -KGK
void GlobeLovelerEditor::audioProcessorChanged(AudioProcessor* source, const ChangeDetails& details) {
    // TODO: check details fields and flags -KGK
}

//==============================================================================
// NON-OPERATIVE -KGK
void GlobeLovelerEditor::audioProcessorParameterChanged(AudioProcessor* processor,
    int parameterIndex, float newValue)
{
    return;
}

//==============================================================================
void GlobeLovelerEditor::initWidgets()
{
    // Compressor Widgets
    addAndMakeVisible(inGainLSlider);
    inGainLSlider.reset(valueTreeState, "inputgain");
    inGainLSlider.setLabelText("Input");

    addAndMakeVisible(makeupGainLSlider);
    makeupGainLSlider.reset(valueTreeState, "makeup");
    makeupGainLSlider.setLabelText("Makeup");

    addAndMakeVisible(threshLSlider);
    threshLSlider.reset(valueTreeState, "threshold");
    threshLSlider.setLabelText("Threshold");

    addAndMakeVisible(ratioLSlider);
    ratioLSlider.reset(valueTreeState, "ratio");
    ratioLSlider.setLabelText("Ratio");

    addAndMakeVisible(kneeLSlider);
    kneeLSlider.reset(valueTreeState, "knee");
    kneeLSlider.setLabelText("Knee");

    addAndMakeVisible(attackLSlider);
    attackLSlider.reset(valueTreeState, "attack");
    attackLSlider.setLabelText("Attack");

    addAndMakeVisible(releaseLSlider);
    releaseLSlider.reset(valueTreeState, "release");
    releaseLSlider.setLabelText("Release");

    addAndMakeVisible(mixLSlider);
    mixLSlider.reset(valueTreeState, "mix");
    mixLSlider.setLabelText("Mix");

    addAndMakeVisible(meter);
    meter.setMode(Meter::Mode::GR);
}
