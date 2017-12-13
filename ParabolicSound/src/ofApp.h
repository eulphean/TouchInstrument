#pragma once

#include "ofMain.h"
#include "AudioPlayer.h"
#include "Osc.h"

using namespace std;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
  
    // Based on Capacitive Sense library, setting a threshold sensitivity value.
    // Change this value if touches are not identified. 
    const int sensorValMax = 200;
    const int sensorValMin = 100;
    
    // An instance of the serial object. One instance is needed for each active
    // serial connection.
    ofSerial serial;

    // A buffer to store our buffered data.
    string buffer;
  
    unsigned int sensorVal1;
    unsigned int sensorVal2;
  
    //PDSP GUI.
    ofxPanel gui;
    ofxPDSPValue capRange;
    ofParameterGroup  group;
  
    // Osc
    Osc oscHandler;
};
