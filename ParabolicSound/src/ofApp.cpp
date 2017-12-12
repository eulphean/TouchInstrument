#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Arduino to talk to.
    serial.setup("/dev/cu.usbmodem14241", 9600);
    
    // Setup the GUI.
    gui.setup("Cap Range");
    group.add(capRange.set( "range", 0.0f, 0.0f, 1.0f)); // Virtual mapping the capacity range after filtering.
    gui.add(group);
  
    // TODO: Add more samples.
    audioPlayer.addSample("/Users/amaykataria/Documents/of_v20170714_osx_release/apps/TouchInstrument/ParabolicSound/bin/data/1.wav");
    audioPlayer.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    gui.draw();
  
    // Update audioPlayer slider value. TODO. Get this from the metal
    // dish sensor values and call update. 
    float capVal = capRange.get();
    audioPlayer.update(capVal);
  
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
                  // Map sensor value to Midi before sending to Midi handler.
                }
              
                // Capacitive touch dish right.
                sensorVal2 = ofToInt(tokens[1]);
                if (sensorVal2 > sensorValMin) {
                  // Map sensor value to Midi before sending to Midi handler.
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

void ofApp::keyPressed(int key) {
  if (key == 'a') {
    audioPlayer.setNextEffect();
  }
  
  if (key == 'b') {
    audioPlayer.setNextSample();
  }
  
  if (key == 'c') {
    audioPlayer.startOscillator();
  }
  
  if (key == 'd') {
    audioPlayer.stopOscillator();
  }
}

