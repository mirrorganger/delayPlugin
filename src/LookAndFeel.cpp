#include "LookAndFeel.h"

namespace delay_plugin {


  RotaryKnobLookAndFeel::RotaryKnobLookAndFeel(){
    setColour(juce::Label::textColourId, colors::knob::label);
    setColour(juce::Slider::textBoxTextColourId, colors::knob::label);
    setColour(juce::Slider::rotarySliderFillColourId, colors::knob::trackActive);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    setColour(juce::CaretComponent::caretColourId, colors::knob::caret);
  }

  void RotaryKnobLookAndFeel::drawRotarySlider(juce::Graphics&g,int x, int y, 
        int width, int heigth, float slidePos,
        float rotrayStartAngle, float rotaryEndAngle,
        juce::Slider& slider){}


}
