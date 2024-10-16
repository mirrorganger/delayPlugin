#include "PluginEditor.h"

namespace delay_plugin
{

  DelayPluginEditor::
  DelayPluginEditor(
      DelayPluginProcessor &p)
      : AudioProcessorEditor(&p), _processorRef(p)
  {
    juce::ignoreUnused(_processorRef);
    
    _delayGroup.setText("Delay");
    _delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    _delayGroup.addAndMakeVisible(_delayKnob);
    _delayGroup.addAndMakeVisible(_delayNoteKnob);
    _tempoSyncButton.setButtonText("Sync");
    _tempoSyncButton.setClickingTogglesState(true);
    _tempoSyncButton.setBounds(0, 0, 70, 27);
    _tempoSyncButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::red);
    _delayGroup.addAndMakeVisible(_tempoSyncButton); 
    addAndMakeVisible(_delayGroup);

    _feedbackGroup.setText("Feeback");
    _feedbackGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    _feedbackGroup.addAndMakeVisible(_feedbackKnob);
    _feedbackGroup.addAndMakeVisible(_stereoKnob);
    _feedbackGroup.addAndMakeVisible(_lowCutKnob);
    _feedbackGroup.addAndMakeVisible(_highCutKnob);
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
    _outputGroup.setBounds(bounds.getWidth() - 160, y, 110, fullHeight);
    _feedbackGroup.setBounds(_delayGroup.getRight() + 10, y,
                             _outputGroup.getX() - _delayGroup.getRight() - 20,
                             fullHeight);

    _delayKnob.setTopLeftPosition(20, 20);
    _tempoSyncButton.setTopLeftPosition(20, _delayKnob.getBottom() + 10);
    _delayNoteKnob.setTopLeftPosition(20, _tempoSyncButton.getBottom());
    
    _feedbackKnob.setTopLeftPosition(20, 20);
    _stereoKnob.setTopLeftPosition(_feedbackKnob.getRight()+20,20);
    _lowCutKnob.setTopLeftPosition(_feedbackKnob.getX(),_feedbackKnob.getBottom()+10);
    _highCutKnob.setTopLeftPosition(_lowCutKnob.getRight()+20,_lowCutKnob.getY());
 
    _gainKnob.setTopLeftPosition(20, 20);
    _mixKnob.setTopLeftPosition(_gainKnob.getX(), _gainKnob.getBottom() + 10);


  }

}
