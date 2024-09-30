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
}

void DelayPluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DelayPluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void DelayPluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    _parameters.update();

    auto dataR = buffer.getWritePointer(0);
    auto dataL = buffer.getWritePointer(1);
    const auto sampleRate = static_cast<float>(getSampleRate());

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
            auto delayInSamples = (_parameters.delay() / 1000.0f) * sampleRate; 
            _delayLine.setDelay(delayInSamples);  
            
            auto dryR = dataR[sample];
            auto dryL = dataL[sample];
            
            _delayLine.pushSample(0, dryL);
            _delayLine.pushSample(1, dryL);
            
            auto wetR = _delayLine.popSample(0);
            auto wetL = _delayLine.popSample(1);
            
            auto gain = _parameters.gain();
            auto mix = _parameters.mix();
            dataR[sample] = (dryR + wetR * _parameters.mix()) * gain;
            dataL[sample] = (dryL + wetL * _parameters.mix()) * gain;
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

