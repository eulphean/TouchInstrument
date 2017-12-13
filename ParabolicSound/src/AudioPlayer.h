
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include  "ofxGui.h"
#include "TouchOscillator.h"

enum State {
  playing = 0,
  paused,
  stopped
};

// Sampler effects.
enum SampleEffect {
  sDelay = 0,
  sPitch,
  sDecimation
};

// Oscillator effects.
enum OscillatorEffect {
  oPitch = 0,
  oDelay
};

enum Oscillator {
  sine = 0,
  square,
  triangle
};

class AudioPlayer : public pdsp::Patchable {
    
public:
    AudioPlayer() { patch(); } 
    AudioPlayer( const AudioPlayer & other ) { patch(); }
  
    // Uber update method to do any sort of updates of oscillators and audio samples. 
    void update(float capRange);
  
    void addSample(string path);
  
    // Update sample audio if any effects are applied.
    void addAudioEffect(SampleEffect effect);
    void removeAudioEffect(SampleEffect effect);
    void setAudioSampleGain(float oscVal);
  
    void setNextSample();
  
    // Oscillators
    void updateOscillators(float capRange);
    void addOscillatorEffect(OscillatorEffect effect);
    void removeOscillatorEffect(OscillatorEffect effect);
    void setOscillatorGain(float oscVal);
  
    // Play, Pause, and Stop
    void play();
    void pause();
    void stop();
  
    // Oscillators. 
    void startOscillator(Oscillator osc);
    void stopOscillator(Oscillator osc);
  
    State getPlaybackState();
    
private:
    const int totalSamples = 1;
    const vector<float> defaultOscillatorPitches = { 45.0f, 15.0f, 5.0f };
    const int oscillatorCount = 3;

    // Samples
    State sampleState;
    vector<SampleEffect> currentSampleEffects;
  
    // Oscillators
    vector<TouchOscillator> oscillators;
    vector<OscillatorEffect> currentOscillatorEffects;
    vector<ofxPDSPTrigger> oscillatorTriggers;
    pdsp::DampedDelay oscDelay;
  
    int sampleIdx;

    // PDSP parameters.
    ofxPDSPEngine engine;
  
    // Sampler
    pdsp::Sampler  sampler;
    vector<pdsp::SampleBuffer*> samples;
  
    // Sample amp
    pdsp::Amp  amp;
    pdsp::Amp sampleGainAmp;
    pdsp::ADSR env;
    ofxPDSPTrigger envGate;
    ofxPDSPTrigger sampleTrig;
  
    // Oscillator amp
    pdsp::Amp oscAmp;
    pdsp::ADSR oscAmpEnv;
    ofxPDSPTrigger oscAmpTrigger;
  
    // Effects.
    pdsp::Decimator  decimator;
    pdsp::DampedDelay delay;
  
    void patch();
  
    // Query for playhead position.
    float getMeterPosition();
  
    void updateSampleAudio(float capRange);
    void updateOscillator(float capRange);
  
    void initOscillators();
};   
    
