#include "Meter.h"

namespace delay_plugin
{

    Meter::Meter(std::atomic<float>& left, std::atomic<float>& right)
     : _left(left), _right(right) {
        setOpaque(true);
        startTimerHz(15);
    };

    void Meter::resized() {
        _maxPos = 4.0f;
        _minPos = static_cast<float>(getHeight()) - 4.0f;
    }

    void Meter::paint(juce::Graphics &g)
    {
        auto width = getLocalBounds().getWidth();

        g.fillAll(colors::level_meter::background);
        drawLevel(g,_dbLeft,0,7);
        drawLevel(g,_dbRight,9,7);
        
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
        _dbLeft = juce::Decibels::gainToDecibels(_left.load(),CLAMP_DB);
        _dbRight = juce::Decibels::gainToDecibels(_right.load(),CLAMP_DB);
        repaint();
     }

    void Meter::drawLevel(juce::Graphics& g, float level, int x, int width){
        int y = static_cast<int>(std::round(juce::jmap(level, MAX_DB, MIN_DB, _maxPos, _minPos)));
        const float highLimit = 0.0f;
        const float midLimit  = -12.0f;
        const auto yMax = static_cast<int>(std::round(juce::jmap(highLimit, MAX_DB, MIN_DB, _maxPos, _minPos)));
        const auto yMed = static_cast<int>(std::round(juce::jmap(midLimit, MAX_DB, MIN_DB, _maxPos, _minPos)));

        if(level > highLimit){
            g.setColour(colors::level_meter::tooLoud);
            g.fillRect(x,y, width,yMax-y);
            g.setColour(colors::level_meter::levelMed);
            g.fillRect(x,yMax, width,yMed-yMax);
            g.setColour(colors::level_meter::levelOk);
            g.fillRect(x,yMed, width,getHeight()-yMed);
        }else if(level > midLimit){
            g.setColour(colors::level_meter::levelMed);
            g.fillRect(x,y, width,yMed-y);
            g.setColour(colors::level_meter::levelOk);
            g.fillRect(x,yMed, width,getHeight()-yMed);
        }else if(y < getHeight()){
            g.setColour(colors::level_meter::levelOk);
            g.fillRect(x,y, width,getHeight()-y);
        }
    } 


}