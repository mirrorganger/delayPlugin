#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace delay_plugin
{

class DelayParameters{

    

private:
    juce::AudioParameterFloat* _gainParam;
    juce::LinearSmoothedValue<float> _gainSmooth;
    float gain = 0.0;
};

} // namespace delay_plugin
