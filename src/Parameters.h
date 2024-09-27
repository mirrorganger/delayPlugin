#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <chrono>

using namespace std::chrono_literals;

namespace delay_plugin
{

    static const juce::ParameterID gainParamID { "gain", 1 };

    using Vts = juce::AudioProcessorValueTreeState;

    template<typename T> struct get_param_type;

    template<> struct get_param_type<float>{using value = juce::AudioParameterFloat;};
    template<> struct get_param_type<bool>{using value = juce::AudioParameterBool;};

    template<typename BaseType>
    using get_param_type_v = get_param_type<BaseType>::value;

    template<typename Base>
    struct AudioParameter{
        using ParamaType = get_param_type_v<Base>;
        ParamaType* param;
        juce::LinearSmoothedValue<Base> smooth;
        juce::ParameterID id;    
        
        AudioParameter(juce::ParameterID id) : id(id) {}

        void getFromTree(Vts& vt){
            param = dynamic_cast<ParamaType*>(vt.getParameter(id.getParamID()));
        }
    };

    using seconds_t = std::chrono::duration<double>;
    using millis_t = std::chrono::duration<double, std::milli>;
    using micros_t = std::chrono::duration<double, std::micro>;

    class DelayParameters
    {
    public:    
        DelayParameters(Vts &state)
        {
            _gain.getFromTree(state);
        }

        Vts::ParameterLayout createParameterLayout(){
            Vts::ParameterLayout layout;
            layout.add(std::make_unique<juce::AudioParameterFloat>(
                gainParamID,
                "Output Gain",
                juce::NormalisableRange<float>{-60.0f, 12.0f},
                0.0f
            ));
            return layout;
        }
        
        void prepareToPlay(double sampleRate){ 
            _gain.smooth.reset(sampleRate, updatePeriod.count());
        }

        void update() noexcept{
            _gain.smooth.setTargetValue(juce::Decibels::decibelsToGain(_gain.param->get()));
        }

        void reset() noexcept{
            _gain.smooth.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(_gain.param->get()));
        }

        float gain() noexcept{
            return _gain.smooth.getNextValue();
        }
    private:
        AudioParameter<float> _gain{gainParamID};    
        seconds_t updatePeriod {0.002};
    };


} // namespace delay_plugin
