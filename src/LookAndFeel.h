#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


namespace delay_plugin
{
    namespace colors
    {   
        const juce::Colour background { 245, 240, 235 };
        const juce::Colour header { 40, 40, 40 };

        namespace group
        {
            const juce::Colour label { 160, 155, 150 };
            const juce::Colour outline { 235, 230, 225 };
        }
        
        namespace knob {
        const juce::Colour trackBackground { 205, 200, 195 };
        const juce::Colour trackActive { 177, 101, 135 };
        const juce::Colour outline { 255, 250, 245 };
        const juce::Colour gradientTop { 250, 245, 240 };
        const juce::Colour gradientBottom { 240, 235, 230 };
        const juce::Colour dial { 100, 100, 100 };
        const juce::Colour dropShadow { 195, 190, 185 };
        const juce::Colour label { 80, 80, 80 };
        const juce::Colour textBoxBackground { 80, 80, 80 };
        const juce::Colour value { 240, 240, 240 };
        const juce::Colour caret { 255, 255, 255 };
        }

        namespace level_meter
        {
            const juce::Colour background{245, 240, 235};
            const juce::Colour tickLine{200, 200, 200};
            const juce::Colour tickLabel{80, 80, 80};
            const juce::Colour tooLoud{226, 74, 81};
            const juce::Colour levelMed{226, 154, 81};
            const juce::Colour levelOk{65, 206, 88};
        }

    }

    class RotaryKnobLookAndFeel : public juce::LookAndFeel_V4
    {
      public:
        RotaryKnobLookAndFeel();

        static RotaryKnobLookAndFeel* get(){
          static RotaryKnobLookAndFeel instance;
          return &instance;
        }

        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryAngle, float rotaryEndAngle, juce::Slider& slider) override;
    };


    class MainLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        MainLookAndFeel(){
            // setColour(juce::GroupComponent::textColourId, colors::group::label);
            // setColour(juce::GroupComponent::outlineColourId, colors::group::outline);
        }
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLookAndFeel)
    };


}
