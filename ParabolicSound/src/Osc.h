#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "AudioPlayer.h"

#define PORT 8000

class Osc {
  public:
    void setup();
    void update();
    void processAudioSignals(ofxOscMessage &m);
    void processOscillatorSignals(ofxOscMessage &m);
    void processCapacitiveValues(int val);
    
  private:
    // Touch OSC parameters.
    ofxOscReceiver receive;
  
    AudioPlayer audioPlayer;
  
    // Based on Capacitive Sense library, setting a threshold sensitivity value.
    // Change this value if touches are not identified. 
    const int capValMax = 140;
    const int capValMin = 95;
};
