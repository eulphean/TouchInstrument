#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxProcessFFT.h"
#include "ofxKsmrFragmentFx.h"
#include "Stripes.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
  
    //ofImage img;
    //ofVideoGrabber grabber;
  
    ofVideoPlayer player;
  
    ofxPanel gui;
    ofxGuiGroup mixer;
    ofxFloatSlider imageAlpha, playerAlpha, grabberAlpha;
  
    ofxGuiGroup stripeMixer;
    ofxFloatSlider offset, rotation, blend;
    ofFbo stripeFbo;
  
    ofxGuiGroup ksmrShader;
    ofxFloatSlider volume;
    
    // Visuals
    Stripes stripeModule;
  
    ofFbo::Settings		setting;
    ofFbo				original;
    ofxKsmrFragmentFx	fx;
    //ofImage				image;
};
