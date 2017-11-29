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
    channelControlChangeRotary = 3;
    channelControlChangeDishes = 4;
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
                  sendMidiControlChangeDishes(0);
                }
              
                // Capacitive touch dish right.
                sensorVal2 = ofToInt(tokens[1]);
                if (sensorVal2 > sensorValMin) {
                  sendMidiControlChangeDishes(1);
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
        currentScene = 1;
      }
    }
    
    // Kick off Scene 2
    if (m.getAddress() == "/Scenes/4/2") {
      int val = m.getArgAsInt(0);
      if(val == 1) {
        sendMidiNoteOn(1);
        currentScene = 2;
      }
    }
    
    // Kick off Scene 3
    if (m.getAddress() == "/Scenes/4/3") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        sendMidiNoteOn(2);
        currentScene = 3;
      }
    }
    
    // Kick off Scene 4
    if (m.getAddress() == "/Scenes/4/4") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        sendMidiNoteOn(3);
        currentScene = 4;
      }
    }
    
    // Kick off Scene 5
    if (m.getAddress() == "/Scenes/4/5") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        sendMidiNoteOn(4);
        currentScene = 5;
      }
    }
    
    // Kick off Scene 6
    if (m.getAddress() == "/Scenes/4/6") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        sendMidiNoteOn(5);
        currentScene = 6;
      }
    }
    
    // ------------------------ Rotary Knobs -----------------------
    
    // Left rotary.
    if (m.getAddress() == "/RotaryLeft") {
      float val = m.getArgAsFloat(0);
      sendMidiControlChangeRotary(0, val);
    }
    
    // Right rotary.
    if (m.getAddress() == "/RotaryRight") {
      float val = m.getArgAsFloat(0);
      sendMidiControlChangeRotary(1, val);
    }
    
    // ------------------------ Fader Knobs -----------------------
    
    // Fader 1
    if (m.getAddress() == "/Scenes/fader1") {
      int val = m.getArgAsInt(0);
    }
    
    // Fader 2
    if (m.getAddress() == "/Scenes/fader2") {
      float val = m.getArgAsFloat(0);
    }
    
    // Fader 3
    if (m.getAddress() == "/Scenes/fader3") {
      float val = m.getArgAsFloat(0);
    }
    
    // Fader 4
    if (m.getAddress() == "/Scenes/fader4") {
      float val = m.getArgAsFloat(0);
    }
    
    // Fader 5
    if (m.getAddress() == "/Scenes/fader5") {
      float val = m.getArgAsFloat(0);
    }
  }
}

// Scene selection.
void ofApp::sendMidiNoteOn(int midiNote) {
  midiOut.sendNoteOn(channelMidiNote, midiNote, 64);
  // Print Midi channel and note associated with it.
  ofLogNotice() << "<Channel, Note>:<" << channelMidiNote << ", " << midiNote << ">";
}

// Rotary button mapping.
void ofApp::sendMidiControlChangeRotary(int device, float val) {
  //if (currentScene == 2 || currentScene == 3) { // Or any scene during which I want to use the dishes, put it here.
    // Map rotary values to Midi signals.
    int midiVal = ofMap(val, 0, 1, 0, 127, true);

    switch (device) {
      case 0: {
        // Channel, control, midi value
        midiOut.sendControlChange(channelControlChangeRotary, 10, midiVal);
        break;
      }
      
      case 1: {
        // Channel, control, midi value
        midiOut.sendControlChange(channelControlChangeRotary, 11, midiVal);
        break;
      }
      
      default:
        break;
    }
  //}
}

// Capacitance to MIDI. 
void ofApp::sendMidiControlChangeDishes(int device){
  switch (device) {
    case 0: {
      // Min sensor value
      unsigned int mapped = ofMap(sensorVal1, sensorValMin, sensorValMax, 0, 127, true);
      midiOut.sendControlChange(channelControlChangeDishes, 10, mapped);
      break;
    }
    
    case 1: {
      unsigned int mapped = ofMap(sensorVal2, sensorValMin, sensorValMax, 0, 127, true);
      midiOut.sendControlChange(channelControlChangeDishes, 11, mapped);
      break;
    }
    
    default:
      break;
  }
}

void ofApp::exit(){
  midiOut.closePort();
}
