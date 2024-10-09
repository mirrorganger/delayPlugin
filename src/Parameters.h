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
    static const juce::ParameterID lowCutParamID {"lowcut",1};
    static const juce::ParameterID highCutParamID {"highcut",1};
    static const juce::ParameterID tempoSyncParamID {"tempoSync",1};
    static const juce::ParameterID delayNoteParamID {"delayNote",1};
    static constexpr millis_t MIN_DELAY_TIME { 5.0};
    static constexpr millis_t MAX_DELAY_TIME {5000.0};

    static const juce::StringArray noteLengths = {
        "1/32",
        "1/16 trip",
        "1/32 dot",
        "1/16",
        "1/8 trip",
        "1/16 dot",
        "1/8",
        "1/4 trip",
        "1/8 dot",
        "1/4",
        "1/2 trip",
        "1/4 dot",
        "1/2",
        "1/1 trip",
        "1/2 dot",
        "1"
    };



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

    static juce::String stringFromHz(float hz, int){
       if(hz  < 1000.0f){
        return juce::String(static_cast<int>(hz)) +  " Hz";
       }
       if(hz  < 10000.0){
        return juce::String((hz) / 1000.0,2) +   " KHz";
       }
       return juce::String((hz) / 1000.0,1) +   " KHz";
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
            _lowCut.getFromTree(state);
            _highCut.getFromTree(state);
            _tempoSyncParam = dynamic_cast<juce::AudioParameterBool*>(state.getParameter(tempoSyncParamID.getParamID()));
            _delayNoteParam = dynamic_cast<juce::AudioParameterChoice*>(state.getParameter(delayNoteParamID.getParamID()));
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
                juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)),
                std::make_unique<juce::AudioParameterFloat>(
                lowCutParamID,
                "lowCut",
                juce::NormalisableRange<float>{20.0f,20000.0f,1.0f,0.3f},
                20.0f,
                juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromHz)),
                std::make_unique<juce::AudioParameterFloat>(
                highCutParamID,
                "highCut",
                juce::NormalisableRange<float>{20.0f,20000.0f,1.0f,0.3f},
                20000.0f,
                juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromHz)),
                std::make_unique<juce::AudioParameterChoice>(
                delayNoteParamID,
                "Delay Note",
                noteLengths,
                noteLengths.size()),
                std::make_unique<juce::AudioParameterBool>(
                tempoSyncParamID,
                "Tempo Sync",
                false));
            return layout;
        }
        
        void prepareToPlay(double sampleRate){ 
            _gain.smooth.reset(sampleRate, updatePeriod.count());
            _feedback.smooth.reset(sampleRate, updatePeriod.count());
            _mix.smooth.reset(sampleRate, updatePeriod.count());
            _stereo.smooth.reset(sampleRate, updatePeriod.count());
            _delay.smooth.reset(sampleRate,updatePeriod.count());
            _lowCut.smooth.reset(sampleRate,updatePeriod.count());
            _highCut.smooth.reset(sampleRate,updatePeriod.count());
        }

        void update() noexcept{
            _gain.smooth.setTargetValue(juce::Decibels::decibelsToGain(_gain.param->get()));
            _mix.smooth.setTargetValue(_mix.param->get() * 0.01f);
            _feedback.smooth.setTargetValue(_feedback.param->get() * 0.01f);
            _stereo.smooth.setTargetValue(_stereo.param->get() * 0.01f);
            _delay.smooth.setTargetValue(_delay.param->get());
            _lowCut.smooth.setTargetValue(_lowCut.param->get());
            _highCut.smooth.setTargetValue(_highCut.param->get());
            _delayNote = _delayNoteParam->getIndex();
            _tempoSync = _tempoSyncParam->get();
        }
        void reset() noexcept{
            *_lowCut.param = 20.0f;
            _lowCut.smooth.setCurrentAndTargetValue(_lowCut.param->get());
            *_highCut.param = 2000.0f;
            _highCut.smooth.setCurrentAndTargetValue(_highCut.param->get());
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

        float lowCut(){
            return _lowCut.smooth.getNextValue();
        }   

        float highCut(){
            return _highCut.smooth.getNextValue();
        }

        float tempoSync(){
            return _tempoSync;
        }

        float delayNote(){
            return _delayNote;
        }

    private:
        SmoothParam<float> _gain{gainParamID};    
        SmoothParam<float> _mix{mixParamID};
        SmoothParam<float> _feedback{feedbackParamID};
        SmoothParam<float> _stereo{stereoParamID};
        SmoothParam<float> _delay{delayParamID};
        SmoothParam<float> _lowCut{lowCutParamID};
        SmoothParam<float> _highCut{highCutParamID};
        juce::AudioParameterBool* _tempoSyncParam;
        bool _tempoSync = false;
        juce::AudioParameterChoice* _delayNoteParam;
        float _delayNote = 0.0f;
        seconds_t updatePeriod {0.2};
        std::tuple<float,float> _pan = {1.0f,0.0f};
    };


} // namespace delay_plugin
