#include "Meter.h"

namespace delay_plugin
{

    Meter::Meter(std::atomic<float>& left, std::atomic<float>& right)
     : _left(left), _right(right) {
        setOpaque(true);
        startTimerHz(1);
    };

    void Meter::paint(juce::Graphics &g)
    {
        // do painting
    }

    void Meter::timerCallback()
    {
        DBG("timerCallback. left: " << _left.load() << " right:" << _right.load());
     }
}
