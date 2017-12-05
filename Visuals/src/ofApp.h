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
  
  
    bool enableNoise = false;
    bool edgeOnTop = false;
    bool fringe = false;
    bool invert = false;
    bool slantShift = false;
    bool texChip = false;
    bool vertNoise = false;
    bool vertSlide = false;
    bool water = false;
  
    float noiseVolume;
    float edgeVolume;
    float fringeVolume;
    float invertVolume;
    float slantVolume;
    float texVolume;
    float vertNoiseVolume;
    float vertSlideVolume;
    float waterVolume;
    
    // Touch OSC
    ofxOscReceiver receiver;
  
    bool hideGui = false;
};
