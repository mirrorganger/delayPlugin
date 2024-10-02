#pragma once

#include "PluginProcessor.h"
#include "RotaryKnob.h"

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
  RotaryKnob _gainKnob{"Gain",_processorRef.getVts(),gainParamID,{70,110}};  
  RotaryKnob _mixKnob{"Mix",_processorRef.getVts(),mixParamID,{70,110}};  
  RotaryKnob _delayKnob{"Delay",_processorRef.getVts(),delayParamID,{70,110}};  
  RotaryKnob _feedbackKnob{"Feedback",_processorRef.getVts(),feedbackParamID,{70,110}};  
  juce::GroupComponent _delayGroup;
  juce::GroupComponent _feedbackGroup;
  juce::GroupComponent _outputGroup;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayPluginEditor)    

}; 


}


