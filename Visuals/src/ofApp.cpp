#include "ofApp.h"
#include "CommonFFT.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(0);
  ofSetCircleResolution(80);
  
  // Setup fft.
  CommonFFT::instance().fft.setup();
  CommonFFT::instance().fft.setNormalize(true); // Give the volume range of each frequency between 0-1
  CommonFFT::instance().fft.setNumFFTBins(48);
  
  // Setup stripes
  stripeModule.setup();
  
  gui.setup();
  
  // Stripe mixer
  stripeMixer.setup("Stripes");
  stripeMixer.add(offset.setup("offset", 10, 0, 50));
  stripeMixer.add(rotation.setup("rotation", 3, -10, 10));
  //stripeMixer.add(rotation.setup("blend1", 0, 0, 255));
  stripeMixer.add(blend.setup("blend", 0, 0, 255));
  
  
  /*ofLoadImage (img, "prussik.jpg");
  player.loadMovie("ableton.mov");
  grabber.setup(ofGetWidth(), ofGetHeight());
  player.play();*/
  
  // Setup
  mixer.setup("Global Mixer");
  mixer.add(imageAlpha.setup("image", 100.0, 0.0, 255.0));
  //mixer.add(playerAlpha.setup("video", 200.0, 0.0, 255.0));
  //mixer.add(grabberAlpha.setup("camera", 100.0, 0.0, 255.0));
  
  gui.add(&mixer);
  
  gui.add(&stripeMixer);
}

//--------------------------------------------------------------
void ofApp::update(){
  // Update fft.
  CommonFFT::instance().fft.update();
  
  stripeModule.update(offset, rotation, blend);
  /*player.update();
  grabber.update();
  if (grabber.isFrameNew()){
    // Do something with pixels from the grabber.
  }*/
}

//--------------------------------------------------------------
void ofApp::draw(){
glDepthFunc( GL_LEQUAL);
  gui.draw();
  
  stripeModule.draw();
  
  /*ofEnableBlendMode(OF_BLENDMODE_ADD);
  
  ofSetColor (255, imageAlpha);
  img.draw(0, 0, ofGetWidth(), ofGetHeight());
  
  ofSetColor (255, playerAlpha);
  player.draw(0, 0, ofGetWidth(), ofGetHeight());
  
  ofSetColor(255, grabberAlpha);
  grabber.draw(0, 0);
  
  ofEnableAlphaBlending();*/
}

// Setup sliders to fade between different effects and sort of overlap them.
// Effects should affected by sound
// Or by X-Y pad that I will setup in the Touch OSC interface.
// But for now, atleast get all the visuals that I have working, then incorporate
// ofxFX, ofxFlowTools, and ofxKrknShader.
