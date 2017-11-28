#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMidi.h"
#include "ofxOsc.h"

using namespace std;

#define PORT 8000

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    void exit();
    void sendMidiNoteOn(int midiNote);
    void sendMidiControlChange(int deviceId);
    void processOscMessages();
  
    // Based on Capacitive Sense library, setting a threshold sensitivity value.
    // Change this value if touches are not identified. 
    const int sensorValMax = 180;
    const int sensorValMin = 100;
    
    // An instance of the serial object. One instance is needed for each active
    // serial connection.
    ofSerial serial;

    // A buffer to store our buffered data.
    string buffer;
  
    unsigned int sensorVal1;
    unsigned int sensorVal2;
  
    ofxPanel gui;
  
    // Midi integration.
    ofxMidiOut midiOut;
    int channelMidiNote, channelControlChange;
  
    // Touch OSC parameters.
    ofxOscReceiver receive;
};
