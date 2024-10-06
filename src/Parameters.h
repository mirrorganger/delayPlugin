#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <chrono>

#include "Dsp.h"

using namespace std::chrono_literals;

namespace delay_plugin
{

    using seconds_t = std::chrono::duration<double>;
    using millis_t = std::chrono::duration<double, std::milli>;
    using micros_t = std::chrono::duration<double, std::micro>;


    static const juce::ParameterID gainParamID { "gain", 1 };
    static const juce::ParameterID delayParamID { "delay", 1 };
    static const juce::ParameterID mixParamID {"mix", 1};
    static const juce::ParameterID feedbackParamID {"feedback", 1};
    static const juce::ParameterID stereoParamID {"stereo", 1};

    static constexpr millis_t MIN_DELAY_TIME { 5.0};
    static constexpr millis_t MAX_DELAY_TIME {5000.0};


    using Vts = juce::AudioProcessorValueTreeState;

    template<typename T> struct get_param_type;

    template<> struct get_param_type<float>{using value = juce::AudioParameterFloat;};
    template<> struct get_param_type<bool>{using value = juce::AudioParameterBool;};

    template<typename BaseType>
    using get_param_type_v = get_param_type<BaseType>::value;


    struct ParameterList
    {
        ParameterList(Vts::ParameterLayout& layout) : _layout(layout){};

        template<typename T, typename ...Args>
        T* const add(Args&&... args){
            _list.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
            return _list.back().get();
        }

    private:
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> _list;
        Vts::ParameterLayout& _layout; 
    };

    template<typename Base>
    struct SmoothParam{
        using ParamaType = get_param_type_v<Base>;
        ParamaType* param;
        juce::LinearSmoothedValue<Base> smooth;
        juce::ParameterID id;    
        
        SmoothParam(juce::ParameterID id) : id(id) {}

        void getFromTree(Vts& vt){
            param = dynamic_cast<ParamaType*>(vt.getParameter(id.getParamID()));
        }
    };

    static juce::String stringFromDb(float db, int){
        return juce::String(db,1) + " dB";
    }

    static juce::String stringFromPercent(float percent, int){
        return juce::String(static_cast<int>(percent)) +  " %";
    }

    static juce::String stringFromTime(float ms, int){
        if(ms < 1000.0f){
            return juce::String(ms, 2) + " ms";
        }else{
           return juce::String(ms  * 0.001f,2) + " s";
        }
    }

    class DelayParameters
    {
    public:    

        DelayParameters(Vts &state)
        {
            _gain.getFromTree(state);
            _mix.getFromTree(state);
            _feedback.getFromTree(state);
            _stereo.getFromTree(state);
            _delay.getFromTree(state);
        }

        Vts::ParameterLayout createParameterLayout(){
            Vts::ParameterLayout layout;
            layout.add(
                std::make_unique<juce::AudioParameterFloat>(
                gainParamID,
                "Output Gain",
                juce::NormalisableRange<float>{-60.0f, 12.0f},
                0.0f,
                juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDb)
                ),
                std::make_unique<juce::AudioParameterFloat>(
                delayParamID,
                "Delay",
                juce::NormalisableRange<float>{MIN_DELAY_TIME.count(),MAX_DELAY_TIME.count(),0.001f,0.25f},
                100.0f,
                juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromTime)),
                std::make_unique<juce::AudioParameterFloat>(
                mixParamID,
                "mix",
                juce::NormalisableRange<float>{0.0f,100.0f,1.0f},
                100.0f,
                juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)),
                std::make_unique<juce::AudioParameterFloat>(
                feedbackParamID,
                "feedback",
                juce::NormalisableRange<float>{-100.0f,100.0f,1.0f},
                0.0f,
                juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)),
                std::make_unique<juce::AudioParameterFloat>(
                stereoParamID,
                "stereo",
                juce::NormalisableRange<float>{-100.0f,100.0f,1.0f},
                0.0f,
                juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent))
                );
            return layout;
        }
        
        void prepareToPlay(double sampleRate){ 
            _gain.smooth.reset(sampleRate, updatePeriod.count());
            _feedback.smooth.reset(sampleRate, updatePeriod.count());
            _mix.smooth.reset(sampleRate, updatePeriod.count());
            _stereo.smooth.reset(sampleRate, updatePeriod.count());
            _delay.smooth.reset(sampleRate,updatePeriod.count());
        }

        void update() noexcept{
            _gain.smooth.setTargetValue(juce::Decibels::decibelsToGain(_gain.param->get()));
            _mix.smooth.setTargetValue(_mix.param->get() * 0.01f);
            _feedback.smooth.setTargetValue(_feedback.param->get() * 0.01f);
            _stereo.smooth.setTargetValue(_stereo.param->get() * 0.01f);
            _delay.smooth.setTargetValue(_delay.param->get());
        }
        void reset() noexcept{
            _gain.smooth.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(_gain.param->get()));
            _feedback.smooth.setCurrentAndTargetValue(_feedback.param->get());
            _delay.smooth.setCurrentAndTargetValue(_delay.param->get());
            _pan = {1.0f, 0.0f};
        }

        float gain() noexcept{
            return _gain.smooth.getNextValue();
        }

        float delay() noexcept{
            return _delay.smooth.getNextValue();
        }

        float mix() noexcept{
            return _mix.smooth.getNextValue();
        }

        float feeback(){
            return _feedback.smooth.getNextValue();
        }
        std::tuple<float,float> pan(){
            _pan =  getPanningEqualPower(_stereo.smooth.getNextValue());
            return _pan;
        }

    private:
        SmoothParam<float> _gain{gainParamID};    
        SmoothParam<float> _mix{mixParamID};
        SmoothParam<float> _feedback{feedbackParamID};
        SmoothParam<float> _stereo{stereoParamID};
        SmoothParam<float> _delay{delayParamID};
        seconds_t updatePeriod {0.2};
        std::tuple<float,float> _pan = {1.0f,0.0f};
    };


} // namespace delay_plugin
