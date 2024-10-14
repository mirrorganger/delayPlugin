#pragma once

#include "PluginProcessor.h"
#include "RotaryKnob.h"
#include "Meter.h"
#include "LookAndFeel.h"


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
  RotaryKnob _stereoKnob{"Stereo",_processorRef.getVts(),stereoParamID,{70,110}};  
  RotaryKnob _lowCutKnob{"Low Cut",_processorRef.getVts(),lowCutParamID,{70,110}};  
  RotaryKnob _highCutKnob{"High Cut",_processorRef.getVts(),highCutParamID,{70,110}};  
  RotaryKnob _delayNoteKnob{"Note",_processorRef.getVts(),delayNoteParamID,{70,110}};
  Meter _levelMeter;  
  juce::TextButton _tempoSyncButton;  
  juce::GroupComponent _delayGroup;
  juce::GroupComponent _feedbackGroup;
  juce::GroupComponent _outputGroup;
  MainLookAndFeel _lookAndFeel;

  juce::AudioProcessorValueTreeState::ButtonAttachment _tempoSyncAttachment {_processorRef.getVts(),tempoSyncParamID.getParamID(),_tempoSyncButton};
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayPluginEditor)    

}; 


}


