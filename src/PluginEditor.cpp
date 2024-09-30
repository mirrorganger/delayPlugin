#include "PluginEditor.h"

namespace delay_plugin
{

DelayPluginEditor::DelayPluginEditor(
    DelayPluginProcessor& p)
    : AudioProcessorEditor(&p), _processorRef(p) {
  juce::ignoreUnused(_processorRef);
  _slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
  _slider.setTextBoxStyle(juce::Slider::TextBoxBelow,false, 70,16);
  _slider.setBounds(0,0,70,86);
  _label.setText("Gain",juce::NotificationType::dontSendNotification);
  _label.setJustificationType(juce::Justification::horizontallyCentred);
  _label.setBorderSize(juce::BorderSize<int>(0,0,2,0));
  _label.attachToComponent(&_slider,false);
  addAndMakeVisible(_slider);
  setSize(500, 330);
}

DelayPluginEditor::~DelayPluginEditor() {}

void DelayPluginEditor::paint(juce::Graphics& g) {
  g.fillAll(juce::Colours::darkgrey);
}

void DelayPluginEditor::resized() {
  _slider.setTopLeftPosition(215,125);
}

}
