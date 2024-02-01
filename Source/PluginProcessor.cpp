/*
  ==============================================================================
    File:           PluginProcessor.cpp
    Developers:     D. Robert Hoover and Kris Keillor
    Repository URL: https://github.com/Top-Notch-DSP/GlobeLoveler
    Date:           2024 Feb 1
    Forked From:    p-hlp
    Original URL:   https://github.com/p-hlp/SMPLComp/tree/master
    License:        GNU General Public License, version 3.0 (GPL-3.0)
  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "util/Constants.h"
#include <juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h>

GlobeLoveler::GlobeLoveler()
    : AudioProcessor(),
    parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
    // Compressor parameters -KGK
    // Add parameter listeners
    parameters.addParameterListener("inputgain", this);
    parameters.addParameterListener("makeup", this);
    parameters.addParameterListener("threshold", this);
    parameters.addParameterListener("ratio", this);
    parameters.addParameterListener("knee", this);
    parameters.addParameterListener("attack", this);
    parameters.addParameterListener("release", this);
    parameters.addParameterListener("mix", this);

    gainReduction.set(0.0f);
    currentInput.set(-std::numeric_limits<float>::infinity());
    currentOutput.set(-std::numeric_limits<float>::infinity());
}

//==============================================================================
GlobeLoveler::~GlobeLoveler()
{
}

//==============================================================================
const String GlobeLoveler::getName() const
{
    return JucePlugin_Name;
}

//==============================================================================
bool GlobeLoveler::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

//==============================================================================
bool GlobeLoveler::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

//==============================================================================
bool GlobeLoveler::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

//==============================================================================
double GlobeLoveler::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================
int GlobeLoveler::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

//==============================================================================
int GlobeLoveler::getCurrentProgram()
{
    return 0;
}

//==============================================================================
void GlobeLoveler::setCurrentProgram(int index)
{
}

//==============================================================================
const String GlobeLoveler::getProgramName(int index)
{
    return {};
}

//==============================================================================
void GlobeLoveler::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void GlobeLoveler::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    globeSampleRate = sampleRate;
    globeSamplesPerBlock = samplesPerBlock;
    DBG(String::formatted("Sample rate set to %f", globeSampleRate));
    DBG(String::formatted("Samples per block set to %f", globeSamplesPerBlock));

    // Prepare dsp classes
    compressor.prepare({sampleRate, static_cast<uint32>(samplesPerBlock), 2});
    inLevelFollower.prepare(sampleRate);
    outLevelFollower.prepare(sampleRate);

    // Set envelope follower for level meter to measure over 300ms time frame
    inLevelFollower.setPeakDecay(0.3f);
    outLevelFollower.setPeakDecay(0.3f);
}

//==============================================================================
void GlobeLoveler::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

//==============================================================================
#ifndef JucePlugin_PreferredChannelConfigurations
bool GlobeLoveler::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Support mono, stereo, and disabled output channels -KGK
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::disabled())
        return false;
    return true;
}
#endif

//==============================================================================
void GlobeLoveler::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const auto numSamples = buffer.getNumSamples();

    // Cull output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Map mono input to stereo input -KGK
    if (totalNumOutputChannels == 2 && totalNumInputChannels == 1) {
        buffer.setSize(2, numSamples, true, true, false);
        buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);
        DBG("Copied channel 0 into channel 1");
    }

    // Update input peak metering
    inLevelFollower.updatePeak(buffer.getArrayOfReadPointers(), totalNumInputChannels, numSamples);
    currentInput.set(Decibels::gainToDecibels(inLevelFollower.getPeak()));

    // Do compressor processing
    compressor.process(buffer);

    // Update gain reduction metering
    gainReduction.set(compressor.getMaxGainReduction());

    // Update output peak metering
    outLevelFollower.updatePeak(buffer.getArrayOfReadPointers(), totalNumInputChannels, numSamples);
    currentOutput = Decibels::gainToDecibels(outLevelFollower.getPeak());
}

//==============================================================================
AudioProcessorEditor* GlobeLoveler::createEditor()
{
    return new GlobeLovelerEditor(*this, parameters);
}

//==============================================================================
bool GlobeLoveler::hasEditor() const
{
    return true;
}

//==============================================================================
void GlobeLoveler::getStateInformation(MemoryBlock& destData)
{
    // Prepare to read XML data
    auto state = parameters.copyState();
    // Create the root element
    std::unique_ptr<juce::XmlElement> rootElement = std::make_unique<juce::XmlElement>("GlobeLovelerState");
    
    // Create the status element
    std::unique_ptr<juce::XmlElement> statusElement = std::make_unique<juce::XmlElement>("STATUS");
    // * Add version number to status
    statusElement.get()->setAttribute("version", JUCEApplication::getInstance()->getApplicationVersion());
    // * Add release type to status, "Release" or "Debug" (should change to "Development" -KGK
    #ifndef JUCE_DEBUG
        statusElement.get()->setAttribute("release", "Release");
    #else
        statusElement.get()->setAttribute("release", "Debug");
    #endif
    // * Add status element to root element
    rootElement->addChildElement(statusElement.release());

    // Create the parameters element & add to the root element 
    std::unique_ptr<juce::XmlElement> parametersElement(state.createXml());
    rootElement->addChildElement(parametersElement.release());

    // Convert XML data to binary
    MemoryBlock binaryData;
    copyXmlToBinary(*rootElement, binaryData);

    // Resize dest block to fit binary data plus null terminator
    const int dataSize = binaryData.getSize();
    destData.setSize(dataSize + 1);
    // * Copy data and write null terminator
    memcpy(destData.getData(), binaryData.getData(), dataSize);
    destData[dataSize] = '\0';
}

//==============================================================================
void GlobeLoveler::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr) {
        if (auto statusXml = xmlState->getChildByName("STATUS"))
        {
            auto versionString = statusXml->getStringAttribute("version", "0.0.0");
        }

        if (auto parametersXml = xmlState->getChildByName("PARAMETERS"))
        {
            if (parametersXml->hasTagName(parameters.state.getType())) {
                parameters.replaceState(juce::ValueTree::fromXml(*parametersXml));
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GlobeLoveler();
}

//==============================================================================
void GlobeLoveler::parameterChanged(const String& parameterID, float newValue)
{
    // Compressor parameters
    if (parameterID == "inputgain") compressor.setInput(newValue);
    else if (parameterID == "threshold") compressor.setThreshold(newValue);
    else if (parameterID == "ratio") compressor.setRatio(newValue);
    else if (parameterID == "knee") compressor.setKnee(newValue);
    else if (parameterID == "attack") compressor.setAttack(newValue);
    else if (parameterID == "release") compressor.setRelease(newValue);
    else if (parameterID == "makeup") compressor.setMakeup(newValue);
    else if (parameterID == "mix") compressor.setMix(newValue);
	DBG("Received unknown parameter " + parameterID);
}

//==============================================================================
AudioProcessorValueTreeState::ParameterLayout GlobeLoveler::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back(std::make_unique<AudioParameterBool>("power", "Power", true));

    params.push_back(std::make_unique<AudioParameterFloat>("inputgain", "Input",
                                                           NormalisableRange<float>(
                                                               Constants::Parameter::inputStart,
                                                               Constants::Parameter::inputEnd,
                                                               Constants::Parameter::inputInterval), 0.0f,
                                                           String(),
                                                           AudioProcessorParameter::genericParameter,
                                                           [](float value, float)
                                                           {
                                                               return String(value, 1) + " dB";
                                                           }));

    params.push_back(std::make_unique<AudioParameterFloat>("threshold", "Tresh",
                                                           NormalisableRange<float>(
                                                               Constants::Parameter::thresholdStart,
                                                               Constants::Parameter::thresholdEnd,
                                                               Constants::Parameter::thresholdInterval), -40.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, float maxStrLen)
                                                           {
                                                               return String(value, 1) + " dB";
                                                           }));

    params.push_back(std::make_unique<AudioParameterFloat>("ratio", "Ratio",
                                                           NormalisableRange<float>(
                                                               Constants::Parameter::ratioStart,
                                                               Constants::Parameter::ratioEnd,
                                                               Constants::Parameter::ratioInterval, 0.5f), 2.0f,
                                                           String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, float)
                                                           {
                                                               if (value > 23.9f)return String("Infinity") + ":1";
                                                               return String(value, 1) + ":1";
                                                           }));

    params.push_back(std::make_unique<AudioParameterFloat>("knee", "Knee",
                                                           NormalisableRange<float>(
                                                               Constants::Parameter::kneeStart,
                                                               Constants::Parameter::kneeEnd,
                                                               Constants::Parameter::kneeInterval),
                                                           6.0f, String(), AudioProcessorParameter::genericParameter,
                                                           [](float value, float)
                                                           {
                                                               return String(value, 1) + " dB";
                                                           }));

    params.push_back(std::make_unique<AudioParameterFloat>("attack", "Attack",
                                                           NormalisableRange<float>(
                                                               Constants::Parameter::attackStart,
                                                               Constants::Parameter::attackEnd,
                                                               Constants::Parameter::attackInterval, 0.5f), 2.0f,
                                                           "ms",
                                                           AudioProcessorParameter::genericParameter,
                                                           [](float value, float)
                                                           {
                                                               if (value == 100.0f) return String(value, 0) + " ms";
                                                               return String(value, 2) + " ms";
                                                           }));

    params.push_back(std::make_unique<AudioParameterFloat>("release", "Release",
                                                           NormalisableRange<float>(
                                                               Constants::Parameter::releaseStart,
                                                               Constants::Parameter::releaseEnd,
                                                               Constants::Parameter::releaseInterval, 0.35f),
                                                           140.0f,
                                                           String(),
                                                           AudioProcessorParameter::genericParameter,
                                                           [](float value, float)
                                                           {
                                                               if (value <= 100) return String(value, 2) + " ms";
                                                               if (value >= 1000)
                                                                   return String(value * 0.001f, 2) + " s";
                                                               return String(value, 1) + " ms";
                                                           }));

    params.push_back(std::make_unique<AudioParameterFloat>("makeup", "Makeup",
                                                           NormalisableRange<float>(
                                                               Constants::Parameter::makeupStart,
                                                               Constants::Parameter::makeupEnd,
                                                               Constants::Parameter::makeupInterval), 10.0f,
                                                           String(),
                                                           AudioProcessorParameter::genericParameter,
                                                           [](float value, float)
                                                           {
                                                               return String(value, 1) + " dB ";
                                                           }));

    params.push_back(std::make_unique<AudioParameterFloat>("mix", "Mix",
                                                           NormalisableRange<float>(
                                                               Constants::Parameter::mixStart,
                                                               Constants::Parameter::mixEnd,
                                                               Constants::Parameter::mixInterval),
                                                           1.0f, "%", AudioProcessorParameter::genericParameter,
                                                           [](float value, float)
                                                           {
                                                               return String(value * 100.0f, 1) + " %";
                                                           }));
   
    return {params.begin(), params.end()};
}
