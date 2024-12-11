#include "LookAndFeel.h"
#include "juce_graphics/juce_graphics.h"
#include <cmath>

namespace delay_plugin {


  RotaryKnobLookAndFeel::RotaryKnobLookAndFeel(){
    setColour(juce::Label::textColourId, colors::knob::label);
    setColour(juce::Slider::textBoxTextColourId, colors::knob::label);
    setColour(juce::Slider::rotarySliderFillColourId, colors::knob::trackActive);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    setColour(juce::CaretComponent::caretColourId, colors::knob::caret);
  }

  void RotaryKnobLookAndFeel::drawRotarySlider(juce::Graphics&g,int x, int y, 
        int width, int heigth, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle,
        juce::Slider& slider){
    auto bounds = juce::Rectangle<int>(x,y,width,width).toFloat();
    auto boundsRect = bounds.reduced(10.0f,10.0f);
    auto path = juce::Path();
    path.addEllipse(boundsRect);
    _dropShadow.drawForPath(g,path);
    g.setColour(colors::knob::outline);
    g.fillEllipse(boundsRect);    
    auto innerRect = boundsRect.reduced(2.0f,2.0f);
    auto gradient = juce::ColourGradient(colors::knob::gradientTop,0.0f,innerRect.getY(),colors::knob::gradientBottom,0.0f,innerRect.getBottom(),false);
    g.setGradientFill(gradient);
    g.fillEllipse(innerRect);
    auto center = bounds.getCentre();
    auto radius = bounds.getWidth()/2.0f;
    auto lineWidth = 3.0f;
    auto arcRadius = radius - lineWidth /2.0f;
    juce::Path backGroundArc;
    backGroundArc.addCentredArc(center.x, center.y, arcRadius,arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle,true);
    g.setColour(colors::knob::trackBackground);
    g.strokePath(backGroundArc, juce::PathStrokeType(lineWidth,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
    auto dialRadius = innerRect.getHeight() / 2.0f - lineWidth;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    juce::Point<float> dialStart(center.x + 10.f * std::sin(toAngle), center.y - 10.f * std::cos(toAngle));
    juce::Point<float> dialEnd(center.x + dialRadius * std::sin(toAngle), center.y - dialRadius * std::cos(toAngle));
    juce::Path dialPath;
    dialPath.startNewSubPath(dialStart);
    dialPath.lineTo(dialEnd);
    g.setColour(colors::knob::dial);
    g.strokePath(dialPath,juce::PathStrokeType(lineWidth,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
    if(slider.isEnabled()){
      float fromAngle = rotaryStartAngle;
      if(slider.getProperties()["drawFromMiddle"]){
        fromAngle += (rotaryEndAngle - rotaryStartAngle) / 2.0f;
      }
      juce::Path valueArc;
      valueArc.addCentredArc(center.x, center.y, arcRadius,arcRadius,0.0f, fromAngle,toAngle,true);
      g.setColour(colors::knob::trackActive);
      g.strokePath(valueArc, juce::PathStrokeType(lineWidth,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
    }    
  }


}
