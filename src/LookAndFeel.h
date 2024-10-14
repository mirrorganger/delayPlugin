#pragma once

#include <JuceHeader.h>

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


        namespace level_meter
        {
            const juce::Colour background{245, 240, 235};
            const juce::Colour tickLine{200, 200, 200};
            const juce::Colour tickLabel{80, 80, 80};
            const juce::Colour tooLoud{226, 74, 81};
            const juce::Colour levelOk{65, 206, 88};
        }

    }


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