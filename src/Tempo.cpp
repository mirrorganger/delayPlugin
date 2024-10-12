#include "Tempo.h"


namespace delay_plugin
{

constexpr millis_t MILLISECONDS_PER_MINUTE = {seconds_t{60.0}};

// The number are factors with respect to one quarter note (duration of a beat)
constexpr std::array<double, 16> NOTE_LENGTHS = {
    1.0 / 8.0,    //  1/32 (quarter note / 8)
    0.5 / 3.0,    //  1 = 1/16 triplet
    1.5 / 8.0,    //  2 = 1/32 dotted
    1.0 / 4.0,    //  3 = 1/16 (quarter note / 4)
    1.0 / 3.0,    //  4 = 1/8 triplet, 
    1.5 / 4.0,    //  5 = 1/16 dotted
    1.0 / 2.0,    //  6 = 1/8 (quarter note / 2)
    2.0 / 3.0,    //  7 = 1/4 triplet
    1.5 / 2.0,    //  8 = 1/8 dotted
    1.0,          //  9 = 1/4
    4.0 / 3.0,    // 10 = 1/2 triplet
    1.5 * 1.0,    // 11 = 1/4 dotted
    2.0,          // 12 = 1/2
    8.0 / 3.0,    // 13 = 1/1 triplet
    1.5 * 2.0,    // 14 = 1/2 dotted
    4.0 * 1.0     // 15 = 1/1
};

void Tempo::reset()
{
    _bpm = DEFAULT_TEMPO;
}

void Tempo::update(const juce::AudioPlayHead* audioPlayHead){   
    if(audioPlayHead == nullptr) return;
    if(const auto pos =  audioPlayHead->getPosition(); pos.hasValue()){
        if(const auto bpm = pos->getBpm(); bpm.hasValue()){
            _bpm = *bpm;
        }
    }
}

millis_t Tempo::getNoteLength(int index) const noexcept {
    return (MILLISECONDS_PER_MINUTE / _bpm) * NOTE_LENGTHS[index];    
}

double Tempo::getTempo() const{
    return _bpm;
}



}

