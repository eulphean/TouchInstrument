#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Arduino to talk to.
    serial.setup("/dev/cu.usbmodem1421", 9600);
  
    // Setup OSC.
    receive.setup(PORT);
  
    // Setup Gui.
    gui.setup();
  
    // MIDI setup.
    midiOut.openVirtualPort("ofxMidiOut"); // open a virtual port
    channelMidiNote = 2;
    channelControlChange = 3;
}

//--------------------------------------------------------------
void ofApp::update(){
    // Service Osc messages. 
    processOscMessages();
  
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
                // Capacitive touch dish Left.
                sensorVal1 = ofToInt(tokens[0]);
                if (sensorVal1 > sensorValMin) {
                  sendMidiControlChange(0);
                }
              
                // Capacitive touch dish right.
                sensorVal2 = ofToInt(tokens[1]);
                if (sensorVal2 > sensorValMin) {
                  sendMidiControlChange(1);
                }
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

void ofApp::processOscMessages() {
  // Touch OSC updates.
  while (receive.hasWaitingMessages()) {
    ofxOscMessage m;
    // Set the next message.
    #pragma warning(disable: WARNING_CODE)
    receive.getNextMessage(&m);
    
    // Notes can range from 0 - 127. Make sure no two note
    // numbers are same.
    
    // Kick off Scene 1
    if (m.getAddress() == "/Scenes/4/1") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        sendMidiNoteOn(0);
      }
    }
    
    // Kick off Scene 2
    if (m.getAddress() == "/Scenes/4/2") {
      int val = m.getArgAsInt(0);
      if(val == 1) {
        sendMidiNoteOn(1);
      }
    }
    
    // Kick off Scene 3
    if (m.getAddress() == "/Scenes/4/3") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        sendMidiNoteOn(2);
      }
    }
    
    // Kick off Scene 4
    if (m.getAddress() == "/Scenes/4/4") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        sendMidiNoteOn(3);
      }
    }
    
    // Kick off Scene 5
    if (m.getAddress() == "/Scenes/4/5") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        sendMidiNoteOn(4);
      }
    }
    
    // Kick off Scene 6
    if (m.getAddress() == "/Scenes/4/6") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        sendMidiNoteOn(5);
      }
    }
  }
}

void ofApp::sendMidiNoteOn(int midiNote) {
  midiOut.sendNoteOn(channelMidiNote, midiNote, 64);
  // Print Midi channel and note associated with it.
  ofLogNotice() << "<Channel, Note>:<" << channelMidiNote << ", " << midiNote << ">";
}

void ofApp::sendMidiControlChange(int deviceId){
  switch (deviceId) {
    case 0: {
      // Min sensor value
      unsigned int mapped = ofMap(sensorVal1, sensorValMin, sensorValMax, 0, 127, true);
      midiOut.sendControlChange(channelControlChange, 10, mapped);
      break;
    }
    
    case 1: {
      unsigned int mapped = ofMap(sensorVal2, sensorValMin, sensorValMax, 0, 127, true);
      midiOut.sendControlChange(channelControlChange, 11, mapped);
      break;
    }
    
    default:
      break;
  }
}

void ofApp::exit(){
  midiOut.closePort();
}
