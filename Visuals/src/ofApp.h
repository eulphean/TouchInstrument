#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxProcessFFT.h"
#include "ofxKsmrFragmentFx.h"
#include "Stripes.h"
#include "Trees.h"
#include "ofxOsc.h"

#define PORT 12345

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
  
    void setupFbos();
    void updateFbos();
    void drawNoiseFbo();
    void keyPressed(int key);
    void processOSCMessages();
    void updateKsmrFx();
  
    // Master mixer.
    ofxPanel gui;
    ofxGuiGroup mixer;
    ofxFloatSlider treesAlpha, stripesAlpha, noiseAlpha, clockAlpha;
  
    // Stripes mixer.
    ofxGuiGroup stripeMixer;
    ofxFloatSlider offset, rotation, blend;
    
    // -------- Visuals -----------
  
    // Stripes.
    Stripes stripeModule;
    // Trees.
    Trees treeModule;
  
    // FBOs
    ofFbo::Settings settings;
    ofFbo stripeFbo;
    ofFbo treeFbo;
    ofFbo emptyNoiseFbo;
    ofFbo totalFxFbo;
  
    // Noise FX
    ofxKsmrFragmentFx noiseFx;
  
    // Total FX.
    ofxKsmrFragmentFx totalFx;
  
    // Clock video.
    ofVideoPlayer clockPlayer;
    ofFbo clockFbo;
    ofFbo clockMaskFbo;
  
    bool enableNoise;
    bool edgeOnTop;
    bool fringe;
    bool invert;
    bool slantShift;
    bool texChip;
    bool vertNoise;
    bool vertSlide;
    bool water;
  
    float noiseVolume = 0.1;
    float edgeVolume =0.1;
    float fringeVolume = 0.1;
    float invertVolume = 0.1;
    float slantVolume =0.1;
    float texVolume = 0.1;
    float vertNoiseVolume = 0.1;
    float vertSlideVolume = 0.1;
    float waterVolume = 0.1;
    
    // Touch OSC
    ofxOscReceiver receiver;
  
    bool hideGui = false;
};
