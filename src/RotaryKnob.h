#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace delay_pluygin
{

struct RotaryKnob : public juce::Component
{
  RotaryKnob(const juce::String& name, 
             juce::AudioProcessorValueTreeState& vts,
             const juce::ParameterID & paramId);
  RotaryKnob (const RotaryKnob&) = delete;
  RotaryKnob& operator= (const RotaryKnob&) = delete;


  juce::Slider _slider;
  juce::Label _label;
  juce::AudioProcessorValueTreeState _attachment;
};

}