#pragma once

#include "PluginProcessor.h"

namespace delay_plugin
{

class DelayPluginEditor : public juce::AudioProcessorEditor
{
public:
  explicit DelayPluginEditor(DelayPluginProcessor&);
  ~DelayPluginEditor() override;

  void paint(juce::Graphics&) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  DelayPluginProcessor& _processorRef;
  juce::Slider _slider;
  juce::Label _label;
  Vts::SliderAttachment _attachment {_processorRef.getVts(),gainParamID.getParamID(),_slider};
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayPluginEditor)    

}; 


}


