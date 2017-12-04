#pragma once
#include "ofMain.h"
#include "ofxOsc.h"

#define PORT 8000
#define HOST "10.73.36.26" // Update this IP address to the IP of the other MAC.
#define PORTTOSEND 12345

class Osc {
  public:
    void setup();
    void update();
    void processAudioMessages(ofxOscMessage &m);
    void processVideoMessages(ofxOscMessage &m);
    
  private:
    // Touch OSC parameters.
    ofxOscReceiver receive;
  
    // To send OSC messages to another machine.
    ofxOscSender sender;
};
