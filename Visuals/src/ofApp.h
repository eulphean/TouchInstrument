#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxProcessFFT.h"
#include "Stripes.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
  
    /*ofTexture img;
    ofVideoPlayer player;
    ofVideoGrabber grabber;*/
  
    ofxPanel gui;
    ofxGuiGroup mixer;
    ofxFloatSlider imageAlpha, playerAlpha, grabberAlpha;
  
    ofxGuiGroup stripeMixer;
    ofxFloatSlider offset, rotation;
  
    // Visuals
    Stripes stripeOne;
};
