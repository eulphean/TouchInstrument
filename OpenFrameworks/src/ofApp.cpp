#include "ofApp.h"
#include "Midi.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Arduino to talk to.
    serial.setup("/dev/cu.usbmodem14241", 9600);
  
    // Setup Osc.
    oscHandler.setup();
  
    // Setup Midi.
    Midi::instance().setup();
  
    // Setup Gui.
    gui.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
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
                  unsigned int mapped = ofMap(sensorVal1, sensorValMin, sensorValMax, 0, 127, true);
                  Midi::instance().sendMidiControlChangeDishes(0, mapped);
                }
              
                // Capacitive touch dish right.
                sensorVal2 = ofToInt(tokens[1]);
                if (sensorVal2 > sensorValMin) {
                  // Map sensor value to Midi before sending to Midi handler.
                  unsigned int mapped = ofMap(sensorVal2, sensorValMin, sensorValMax, 0, 127, true);
                  Midi::instance().sendMidiControlChangeDishes(1, mapped);
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

void ofApp::exit(){
  Midi::instance().exit();
}

