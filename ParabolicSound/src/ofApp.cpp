#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  
    // Arduino to talk to.
    serial.setup("/dev/ttyACM0", 9600);
    
    // Setup the GUI.
    gui.setup("Cap Range");
    group.add(capRange.set( "range", 0.0f, 0.0f, 1.0f)); // Virtual mapping the capacity range after filtering.
    gui.add(group);
  
    // Setup OSC.
    oscHandler.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    gui.draw();
  
    // Update OSC. 
    oscHandler.update();
  
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
            capVal = ofToInt(buffer);
            std::cout << ofToString(capVal) << std::endl;
            oscHandler.processCapacitiveValues(capVal);
            
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

