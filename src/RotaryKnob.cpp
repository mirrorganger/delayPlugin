#include "RotaryKnob.h"
#include "LookAndFeel.h"

namespace delay_plugin
{

    RotaryKnob::RotaryKnob(const juce::String &name,
                           juce::AudioProcessorValueTreeState &vts,
                           const juce::ParameterID &paramId,
                           const Size & size,
                           bool drawFromMiddle)
        : _attachment(vts, paramId.getParamID(),_slider) 

    {
        _slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        _slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
        _slider.setBounds(0, 0, 70, 86);
        addAndMakeVisible(_slider);
        _label.setText(name, juce::NotificationType::dontSendNotification);
        _label.setJustificationType(juce::Justification::horizontallyCentred);
        _label.setBorderSize(juce::BorderSize<int>(0, 0, 2, 0));
        _label.attachToComponent(&_slider, false);
        addAndMakeVisible(_label);
        setSize(size.width, size.height);
        setLookAndFeel(RotaryKnobLookAndFeel::get());
        _slider.getProperties().set("drawFromMiddle",drawFromMiddle);
    }

    void RotaryKnob::resized(){
    _slider.setTopLeftPosition(0,24);
    }

}
