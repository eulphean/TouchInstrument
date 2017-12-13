
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
  
    void setNextSample();
  
    // Oscillators
    void updateOscillators(float capRange);
    //void addOscillator();
    //void removeOscillator();
    //void addOscillatorEffect(OscillatorEffect effect);
    //void removeOscillatorEffect(OscillatorEffect effect);
  
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
    const float defaultOscillatorPitch = 45.0f;
    const int oscillatorCount = 3;

    // Samples
    State sampleState;
    vector<SampleEffect> currentSampleEffects;
  
    // Oscillators
    vector<TouchOscillator> oscillators;
    vector<OscillatorEffect> oscillatorEffects;
    vector<ofxPDSPTrigger> oscillatorTriggers;
  
    int sampleIdx;

    // PDSP parameters.
    ofxPDSPEngine engine;
  
    // Sampler
    pdsp::Sampler  sampler;
    vector<pdsp::SampleBuffer*> samples;
  
    // Amp
    pdsp::Amp  amp;
  
    pdsp::Amp oscillatorAmp;

    // Triggers.
    pdsp::ADSR      env;
    ofxPDSPTrigger envGate;
    ofxPDSPTrigger sampleTrig;
  
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
    