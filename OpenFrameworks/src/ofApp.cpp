#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    serial.setup("/dev/cu.usbmodem1421", 9600);
  
    //--------PATCHING-------
    osc.out_triangle() * 0.25f >> engine.audio_out(0);
    osc.out_triangle() * 0.25f >> engine.audio_out(1);
  
    // Default
    60.0f >> osc.in_pitch();
  
    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(1); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup( 44100, 512, 2);
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
                sensorVal1 = ofToInt(tokens[0]);
                sensorVal2 = ofToInt(tokens[1]);
                if (sensorVal1 > 200 || sensorVal2 > 200) {
                  updatePitch();
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

void ofApp::updatePitch() {
  // Update pitch of the sound.
  float newPitch = ofMap (sensorVal1, 200, 1500, 60.0f, 110.0f, true);
  newPitch >> osc.in_pitch();
  
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofDrawBitmapString(osc.meter_pitch(), 10, 10);
    // Creates squares on the screen.
  /*if (switchA) {
    // A successful touch has happened.
    glm::vec2 point (ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
    ofSetColor(ofColor::red);
    ofFill();
    ofDrawRectangle(point, 100, 100);
  }
  
  // Creates circles on the screen.
  if (switchB) {
    glm::vec2 point (ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
    ofSetColor(ofColor::blue);
    ofFill();
    ofDrawCircle(point, 100);
  }*/
}
