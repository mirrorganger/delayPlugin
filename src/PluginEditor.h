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
  DelayPluginProcessor& processorRef;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayPluginEditor)    

}; 


}


