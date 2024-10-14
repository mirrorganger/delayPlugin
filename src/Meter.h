#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace delay_plugin
{


class Meter : public juce::Component, public juce::Timer
{
public:
    Meter(std::atomic<float>& left, std::atomic<float>& right);
    void paint(juce::Graphics& graphics)override;
    void timerCallback() override;
private:
    std::atomic<float>& _left;
    std::atomic<float>& _right;
    float _dbLeft = 0.0f;
    float _dbRight = 0.0f;
};


}
