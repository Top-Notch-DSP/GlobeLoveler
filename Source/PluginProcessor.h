/*
  ==============================================================================
    File:           PluginProcessor.h
    Developers:     D. Robert Hoover and Kris Keillor
    Repository URL: https://github.com/Top-Notch-DSP/GlobeLoveler
    Date:           2024 Feb 1
    Forked From:    p-hlp
    Original URL:   https://github.com/p-hlp/SMPLComp/tree/master
    License:        GNU General Public License, version 3.0 (GPL-3.0)
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "dsp/include/Compressor.h"
#include "dsp/include/LevelEnvelopeFollower.h"

//==============================================================================
class GlobeLoveler : public AudioProcessor, public AudioProcessorValueTreeState::Listener, juce::ChangeBroadcaster
{
public:
    //==============================================================================
    GlobeLoveler();
    ~GlobeLoveler();

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif
    void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    const String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    // Get reverb object -KGK

    //==============================================================================
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void parameterChanged(const String& parameterID, float newValue) override;

    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    //==============================================================================
    Atomic<float> gainReduction;
    Atomic<float> currentInput;
    Atomic<float> currentOutput;

//==============================================================================
private:
    BusesProperties Properties;     // Declare BusesProperities member (unitialized) -KGK
    BusesLayout Layouts;            // Declare the BusesLayouts member (unused) -KGK

    double globeSampleRate = 48000;
    int globeSamplesPerBlock = 480;

    // Parameters must be initialized AFTER parameter names -KGK
    AudioProcessorValueTreeState parameters;

    //==============================================================================
    Compressor compressor;

    // Reverb object -KGK
    #if GLOBE_REVERB
        std::unique_ptr<Hooverb> reverb;
    #endif
    LevelEnvelopeFollower inLevelFollower;
    LevelEnvelopeFollower outLevelFollower;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobeLoveler)
};