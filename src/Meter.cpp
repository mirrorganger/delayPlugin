#include "Meter.h"

namespace delay_plugin
{

    Meter::Meter(std::atomic<float>& left, std::atomic<float>& right)
     : _left(left), _right(right) {
        setOpaque(true);
        startTimerHz(1);
    };

    void Meter::resized() {
        _maxPos = 4.0f;
        _minPos = static_cast<float>(getHeight()) - 4.0f;
    }

    void Meter::paint(juce::Graphics &g)
    {
        auto width = getLocalBounds().getWidth();

        g.fillAll(colors::level_meter::background);

        
        for(float db = MAX_DB; db >= MIN_DB; db-=STEP_DB){
            auto tickPosition = static_cast<int>(std::round(juce::jmap(db, MAX_DB, MIN_DB, _maxPos, _minPos)));
            g.setColour(colors::level_meter::tickLine);
            g.fillRect(0,tickPosition,16,1); 
            g.setColour(colors::level_meter::tickLabel);
            g.drawSingleLineText(juce::String(static_cast<int>(db)), width, tickPosition +3, juce::Justification::right); 
        }

    }

    void Meter::timerCallback()
    {
        DBG("timerCallback. left: " << _left.load() << " right:" << _right.load());
     }
}
