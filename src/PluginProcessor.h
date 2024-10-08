#pragma once 

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "Parameters.h"

namespace delay_plugin
{
class DelayPluginProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    DelayPluginProcessor();
    ~DelayPluginProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    Vts& getVts();

private:
    Vts::ParameterLayout createParameterLayout();
    Vts _vts{*this,nullptr, "DealayParameters", createParameterLayout()};
    DelayParameters _parameters;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> _delayLine;
    float _feedbackL = 0.0f;
    float _feedbackR = 0.0f;
    std::array<juce::dsp::StateVariableTPTFilter<float>,2U> _filterBank;    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginProcessor)
};


}

