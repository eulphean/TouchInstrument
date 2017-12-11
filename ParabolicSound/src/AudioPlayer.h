
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include  "ofxGui.h"

enum State {
  playing = 0,
  paused,
  stopped
};

class AudioPlayer : public pdsp::Patchable {
    
public:
    AudioPlayer() { patch(); } 
    AudioPlayer( const AudioPlayer & other ) { patch(); }
  
    void update(float capRange);
    // Update sound for the current audio effect.
    void updateSound(float capRange);
    void addSample(string path);
  
    void setNextEffect();
    void setNextSample();
  
    // Play, Pause, and Stop
    void play();
    void pause();
    void stop();
  
    State getPlaybackState();
    
private:
    // State of the system.
    State sampleState;
  
    int currentEffect, sampleIdx;

    const int totalEffects = 3;
    const int totalSamples = 1;
  
    // PDSP parameters.
    ofxPDSPEngine engine;
  
    // Sampler
    pdsp::Sampler  sampler;
    vector<pdsp::SampleBuffer*> samples;
  
    // Amp
    pdsp::Amp  amp;

    // Triggers.
    pdsp::ADSR      env;
    ofxPDSPTrigger envGate;
    ofxPDSPTrigger sampleTrig;
  
    // Effects.
    pdsp::Decimator  decimator;
    pdsp::DampedDelay delay;
  
    void patch();
    // Get playhead position.
    float getMeterPosition();
};   
    
