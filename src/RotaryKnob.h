#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace delay_plugin
{

struct RotaryKnob : public juce::Component
{
  struct Size{
    int width;
    int height;
  };

  RotaryKnob(const juce::String& name, 
             juce::AudioProcessorValueTreeState& vts,
             const juce::ParameterID & paramId,
             const Size& size);
  RotaryKnob (const RotaryKnob&) = delete;
  RotaryKnob& operator= (const RotaryKnob&) = delete;
  ~RotaryKnob() override = default;

  void resized() override;

  juce::Slider _slider;
  juce::Label _label;
  juce::AudioProcessorValueTreeState::SliderAttachment _attachment;
};

}