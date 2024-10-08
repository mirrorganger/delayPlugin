#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace delay_plugin
{

DelayPluginProcessor::DelayPluginProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ),
    _parameters(_vts)
{
}

DelayPluginProcessor::~DelayPluginProcessor()
{
}

//==============================================================================
const juce::String DelayPluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayPluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayPluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayPluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayPluginProcessor::getCurrentProgram()
{
    return 0;
}

void DelayPluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String DelayPluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void DelayPluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void DelayPluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (samplesPerBlock);
    _parameters.prepareToPlay(sampleRate);
    _parameters.reset();
    _delayLine.prepare({sampleRate,juce::uint32(samplesPerBlock),2});
    _delayLine.setMaximumDelayInSamples(static_cast<int>(std::ceil(seconds_t(MAX_DELAY_TIME).count() * sampleRate)));
    _delayLine.reset();
    _feedbackL = 0.0f;
    _feedbackR = 0.0f;
}

void DelayPluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DelayPluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    auto stereo = &juce::AudioChannelSet::stereo;
    auto mono = &juce::AudioChannelSet::mono;
    auto mainIn = layouts.getMainInputChannelSet();
    auto mainOut = layouts.getMainOutputChannelSet();

    if (mainIn == mono() && mainOut == mono()){ return true;}
    if (mainIn == stereo() && mainOut == stereo()){return true;}
    if (mainIn == mono() && mainOut == stereo()){return true;}
    return false;
}

void DelayPluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    _parameters.update();

    auto mainInput = getBusBuffer(buffer,true,0);
    auto mainOutput = getBusBuffer(buffer,false,0);

    float const * inputDataL = mainInput.getReadPointer(0);
    float const * inputDataR = mainInput.getReadPointer(mainInput.getNumChannels() > 1? 1 : 0);

    float* outputDataL = mainOutput.getWritePointer(0);
    float* outputDataR = mainOutput.getWritePointer(mainInput.getNumChannels() > 1? 1 : 0);

    const auto sampleRate = static_cast<float>(getSampleRate());

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
            auto delayInSamples = (_parameters.delay() / 1000.0f) * sampleRate; 
            _delayLine.setDelay(delayInSamples);  
            
            auto dryR = inputDataL[sample];
            auto dryL = inputDataR[sample];
            
            auto mono = (dryL + dryR)* .5f;    

            const auto [panL, panR] = _parameters.pan();

            _delayLine.pushSample(0, mono * panL + _feedbackR);
            _delayLine.pushSample(1, mono * panR + _feedbackL) ;

            auto wetR = _delayLine.popSample(0);
            auto wetL = _delayLine.popSample(1);
            
            _feedbackR = wetR * _parameters.feeback();
            _feedbackL = wetL * _parameters.feeback();

            auto gain = _parameters.gain();
            auto mix = _parameters.mix();
            
            outputDataL[sample] = (dryR + wetR * _parameters.mix()) * gain;
            outputDataR[sample] = (dryL + wetL * _parameters.mix()) * gain;
        }
}

//==============================================================================
bool DelayPluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayPluginProcessor::createEditor()
{
    return new DelayPluginEditor (*this);
}

//==============================================================================
void DelayPluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    DBG(_vts.copyState().toXmlString());
    copyXmlToBinary(*_vts.copyState().createXml(),destData);
}

void DelayPluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if(xml.get()!=nullptr && xml->hasTagName(_vts.state.getType())){
        _vts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

Vts::ParameterLayout DelayPluginProcessor::createParameterLayout()
{
    return _parameters.createParameterLayout();
}

Vts& DelayPluginProcessor::getVts(){
    return _vts;
}

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new delay_plugin::DelayPluginProcessor();
}

