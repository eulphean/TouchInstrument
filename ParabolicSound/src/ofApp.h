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
    
    // An instance of the serial object. One instance is needed for each active
    // serial connection.
    ofSerial serial;

    // A buffer to store our buffered data.
    string buffer;
  
    unsigned int capVal;
  
    //PDSP GUI.
    ofxPanel gui;
    ofxPDSPValue capRange;
    ofParameterGroup  group;
  
    // Osc
    Osc oscHandler;
};
