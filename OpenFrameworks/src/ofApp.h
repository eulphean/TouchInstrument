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
  
    // OSC utils.
    void processOscMessages();
  
    // Midi utils.
    void sendMidiNoteOn(int midiNote);
    void sendMidiControlChangeDishes(int device);
    void sendMidiControlChangeRotary(int device, float val);
  
    // Based on Capacitive Sense library, setting a threshold sensitivity value.
    // Change this value if touches are not identified. 
    const int sensorValMax = 400;
    const int sensorValMin = 150;
    
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
    int channelMidiNote, channelControlChangeDishes, channelControlChangeRotary;
  
    // Touch OSC parameters.
    ofxOscReceiver receive;
  
    // Ableton specific logic.
    int currentScene;
     
};
