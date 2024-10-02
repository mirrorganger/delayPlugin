#include "PluginEditor.h"

namespace delay_plugin
{

  DelayPluginEditor::DelayPluginEditor(
      DelayPluginProcessor &p)
      : AudioProcessorEditor(&p), _processorRef(p)
  {
    juce::ignoreUnused(_processorRef);
    
    _delayGroup.setText("Delay");
    _delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    _delayGroup.addAndMakeVisible(_delayKnob);
    addAndMakeVisible(_delayGroup);

    _feedbackGroup.setText("Feeback");
    _feedbackGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    addAndMakeVisible(_feedbackGroup);

    _outputGroup.setText("Output");
    _outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    _outputGroup.addAndMakeVisible(_mixKnob);
    _outputGroup.addAndMakeVisible(_gainKnob);
    addAndMakeVisible(_outputGroup);

    setSize(500, 330);
  }

  DelayPluginEditor::~DelayPluginEditor() {}

  void DelayPluginEditor::paint(juce::Graphics &g)
  {
    g.fillAll(juce::Colours::darkgrey);
  }

  void DelayPluginEditor::resized()
  {
    auto bounds = getLocalBounds();

    auto y = 10;
    auto fullHeight = bounds.getHeight() - 20;

    // Position the groups
    _delayGroup.setBounds(10, y, 110, fullHeight);
    _outputGroup.setBounds(bounds.getWidth() - 160, y, 150, fullHeight);
    _feedbackGroup.setBounds(_delayGroup.getRight() + 10, y,
                             _outputGroup.getX() - _delayGroup.getRight() - 20,
                             fullHeight);

    _delayKnob.setTopLeftPosition(20, 20);
    _gainKnob.setTopLeftPosition(20, 20);
    _mixKnob.setTopLeftPosition(_gainKnob.getX(), _gainKnob.getBottom() + 10);
  }

}
