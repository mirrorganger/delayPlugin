#pragma once

#include "Common.h"
#include <juce_audio_processors/juce_audio_processors.h>

namespace delay_plugin
{

constexpr double DEFAULT_TEMPO = 120.0;

class Tempo
{
public:
    void reset();
    void update(const juce::AudioPlayHead* head);
    millis_t getNoteLength(int index) const noexcept;    
    double getTempo() const;
private:
    double _bpm = DEFAULT_TEMPO;
};


} // namespace delay_plugin


