#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "LookAndFeel.h"

namespace delay_plugin {

    struct SmoothDbLevel {

        static constexpr float CLAMP_DB = -120.0F;
        static constexpr float CLAMP_LEVEL = 0.00001F;

        SmoothDbLevel(std::atomic<float> &refToUse, float decay) : ref(refToUse), decay(decay) {};
        void update() {
            auto newLevel = ref.load();
            if (newLevel > smooth) {
                smooth = newLevel;
            } else {
                smooth += (newLevel - smooth) * decay;
            }
            if (smooth > CLAMP_LEVEL) {
                val = juce::Decibels::gainToDecibels(smooth);
            } else {
                val = CLAMP_DB;
            }
        }
        std::atomic<float> &ref;
        float decay;
        float val;
        float smooth = CLAMP_LEVEL;
    };

    class Meter : public juce::Component, public juce::Timer {
    public:
        Meter(std::atomic<float> &left, std::atomic<float> &right);
        void resized() override;
        void paint(juce::Graphics &graphics) override;
        void timerCallback() override;
    private:
        static constexpr float MAX_DB = 6.0F;
        static constexpr float MIN_DB = -60.0F;
        static constexpr float STEP_DB = 6.0F;
        static constexpr int REFRESH_RATE = 60;
        static constexpr float _decay{1.0F - std::exp(-1.0f / (static_cast<float>(REFRESH_RATE) * 0.2F))};

        void drawLevel(juce::Graphics &g, float level, int x, int width) const;

        float _maxPos = 4.0f;
        float _minPos;

        SmoothDbLevel _left;
        SmoothDbLevel _right;
    };


}// namespace delay_plugin
