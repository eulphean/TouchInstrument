#pragma once
#include "ofMain.h"
#include "ofxOsc.h"

#define PORT 8000

class Osc {
  public:
    void setup();
    void update();
    void processAudioSignals(ofxOscMessage &m);
    void processOscillatorSignals(ofxOscMessage &m);
    
  private:
    // Touch OSC parameters.
    ofxOscReceiver receive;
};
