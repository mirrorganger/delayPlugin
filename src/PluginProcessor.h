#pragma once

#include "Delay.hpp"
#include "Measurement.h"
#include "Parameters.h"
#include "Tempo.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

namespace delay_plugin {
    class DelayPluginProcessor final : public juce::AudioProcessor {
    public:
        //==============================================================================
        DelayPluginProcessor();
        ~DelayPluginProcessor() override;

        //==============================================================================
        void prepareToPlay(double sampleRate, int samplesPerBlock) override;
        void releaseResources() override;

        bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

        void processBlock(juce::AudioBuffer<float> &,
                          juce::MidiBuffer &) override;
        using AudioProcessor::processBlock;

        //==============================================================================
        juce::AudioProcessorEditor *createEditor() override;
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
        void setCurrentProgram(int index) override;
        const juce::String getProgramName(int index) override;
        void changeProgramName(int index, const juce::String &newName) override;

        //==============================================================================
        void getStateInformation(juce::MemoryBlock &destData) override;
        void setStateInformation(const void *data, int sizeInBytes) override;

        Measurement<float> &getMaxL();
        Measurement<float> &getMaxR();

        Vts &getVts();

    private:
        Vts::ParameterLayout createParameterLayout();
        Vts _vts{*this, nullptr, "DealayParameters", createParameterLayout()};
        void updateDelay(float newDelay);
        void updateFade();
        DelayParameters _parameters;
        std::array<dsp::FractionalDelay, 2> _delayLine = {
                dsp::FractionalDelay{seconds_t(MAX_DELAY_TIME).count() * 48000.0F},
                dsp::FractionalDelay{seconds_t(MAX_DELAY_TIME).count() * 48000.0F}};
        float _feedbackL = 0.0f;
        float _feedbackR = 0.0f;
        std::array<juce::dsp::StateVariableTPTFilter<float>, 2U> _filterBank;
        std::array<float, 2U> _filterCutOffPrev = {-1.0f, -1.0f};
        Tempo _tempo;
        Measurement<float> _maxL;
        Measurement<float> _maxR;
        float _delayInSamples = 0.0f;
        float _targetDelay = 0.0f;
        float _fade = 0.0f;
        float _fadeTarget = 0.0f;
        float _coeff = 0.0f;
        float _wait = 0.0f;
        float _waitInc = 0.0f;
        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayPluginProcessor)
    };

}// namespace delay_plugin
