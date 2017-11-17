#pragma once

#include "ofMain.h"
#include "ofxPdsp.h"
#include "ofxGui.h"

using namespace std;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    void updatePitch();
  
    // Based on Capacitive Sense library, setting a threshold sensitivity value.
    // Change this value if touches are not identified. 
    const int sensorThreshold = 1000;
    
    // An instance of the serial object. One instance is needed for each active
    // serial connection.
    ofSerial serial;

    // A buffer to store our buffered data.
    string buffer;
  
    unsigned int sensorVal1;
    unsigned int sensorVal2;
  
    ofxPanel gui;
    ofParameterGroup osc1Group;
  
    ofParameterGroup osc2Group;
  
    ofxPDSPValue pitch1;
    ofxPDSPValue pitch2;
  
    pdsp::VAOscillator osc1;
    pdsp::VAOscillator osc2;
    pdsp::LFO           lfo;
  
    // pdsp modules
    ofxPDSPEngine   engine;
};
