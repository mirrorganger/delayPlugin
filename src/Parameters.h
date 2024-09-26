#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace delay_plugin
{

    static const juce::ParameterID gainParamID { "gain", 1 };

    using Vts = juce::AudioProcessorValueTreeState;

    template<typename T> struct get_param_type;

    template<> struct get_param_type<float>{using ParamaType = juce::AudioParameterFloat;};
    template<> struct get_param_type<bool>{using ParamaType = juce::AudioParameterBool;};

    template<typename BaseType>
    using get_param_T = get_right_type<BaseType>::ParamType;

 
    template<typename Base>
    struct AudioParameter{
        using ParamaType = get_param_type<Base>;
        ParamaType* param;
        juce::LinearSmoothedValue<Base> smooth;
        Base* value;
        juce::ParameterID id;    
        
        void getFromTree(Vts& vt){
            param = dynamic_cast<ParamaType*>(vt.getParamter(id.getParamID()));
        }

    };



    class DelayParameters
    {

        DelayParameters::DelayParameters(Vts &state)
        {
            _gain.getFromTree(state);
        }

        Vts::ParameterLayout createParameterLayout(){
            
            Vts::ParameterLayout layout;
            layout.add(std::make_unique<juce::AudioParameterFloat>(
                gainParamID,
                "Output Gain",
                juce::NormalisableRange<float>{-12.0f, 12.0f},
                0.0f
            ));
            return layout;
        }


    private:
        AudioParameter<float> _gain;
    };


} // namespace delay_plugin
