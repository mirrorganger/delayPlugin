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
    void drawLevel(juce::Graphics& g, float level, int x, int width);
private:
    static constexpr float MAX_DB = 6.0F;
    static constexpr float MIN_DB = -60.0F;
    static constexpr float STEP_DB =  6.0F; 
    static constexpr float CLAMP_DB = -120.0F;
    static constexpr float CLAMP_LEVEL = 0.00001F;
    float _maxPos = 4.0f;
    float _minPos;

    std::atomic<float>& _left;
    std::atomic<float>& _right;
    float _dbLeft = CLAMP_DB;
    float _dbRight = CLAMP_DB;

};


}
