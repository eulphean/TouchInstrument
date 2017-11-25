#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    serial.setup("/dev/cu.usbmodem1421", 9600);
    gui.setup();
  
    // MIDI setup.
    midiOut.openVirtualPort("ofxMidiOut"); // open a virtual port
  
    channel = 2;
}

//--------------------------------------------------------------
void ofApp::update(){
    // As long as there is serial data available to read, repeat.
    while (serial.available() > 0)
    {
        // [Summary] Data is sent from the Arduino byte by byte.
        // In this while loop, we read the data byte by byte until
        // there is incoming data. Every character we are sending from
        // there is a byte wide. We keep constructing the buffer until
        // we reach the new line character, which determines that we are
        // done reading the first set of data.
    
        // Read the byte.
        char b = serial.readByte();
        
        // End of line character.
        if (b == '\n')
        {
            // Split the buffer on the commas.
            std::vector<std::string> tokens = ofSplitString(buffer, ",");
          
            // The number of tokens in our packet is 2, here we check to make
            // sure that our packet is correctly formed.
            if (tokens.size() == 2)
            {
                // Midi device 0.
                sensorVal1 = ofToInt(tokens[0]);
                if (sensorVal1 > sensorValMin) {
                  postMidi(0);
                }
              
                // Midi device 1.
                sensorVal2 = ofToInt(tokens[1]);
                if (sensorVal2 > sensorValMin) {
                  postMidi(1);
                }
              
                // TODO: Add another metal MIDI
                // device.
            }
          
            std::cout << ofToString(tokens) << std::endl;
          
            // Empty the buffer.
            buffer = "";
        }
        else
        {
            // If it's not the line feed character, add it to the buffer.
            buffer += b;
        }
    }
}

void ofApp::draw(){
  gui.draw();
}

void ofApp::postMidi(int deviceId){
  switch (deviceId) {
    case 0: {
      // Min sensor value
      unsigned int mapped = ofMap(sensorVal1, sensorValMin, sensorValMax, 0, 127, true);
      midiOut.sendControlChange(channel, 10, mapped);
      break;
    }
    
    case 1: {
      unsigned int mapped = ofMap(sensorVal2, sensorValMin, sensorValMax, 0, 127, true);
      midiOut.sendControlChange(channel, 11, mapped);
      break;
    }
    
    default:
      break;
  }
}

void ofApp::exit(){
  midiOut.closePort();
}
