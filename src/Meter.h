#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "LookAndFeel.h"

namespace delay_plugin
{

class Meter : public juce::Component, public juce::Timer
{
public:
    Meter(std::atomic<float>& left, std::atomic<float>& right);
    void resized() override;
    void paint(juce::Graphics& graphics)override;
    void timerCallback() override;
private:
    static constexpr float MAX_DB = 6.0F;
    static constexpr float MIN_DB = -60.0F;
    static constexpr float STEP_DB =  6.0F; 
    
    float _maxPos = 4.0f;
    float _minPos;

    std::atomic<float>& _left;
    std::atomic<float>& _right;
    float _dbLeft = 0.0f;
    float _dbRight = 0.0f;

};


}
