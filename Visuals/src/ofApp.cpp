#include "ofApp.h"
#include "CommonFFT.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(0);
  ofSetCircleResolution(80);
  
  // Setup GUI.
  gui.setup();
  
  // Setup fft.
  CommonFFT::instance().fft.setup();
  CommonFFT::instance().fft.setNormalize(true); // Give the volume range of each frequency between 0-1
  CommonFFT::instance().fft.setNumFFTBins(48);
  
  // FBO setup for different modules getting drawn.
  setupFbos();
  
  // Setup modules.
  stripeModule.setup();
  treeModule.setup();
  
  // Setup FX
  noiseFx.setup(&emptyNoiseFbo, settings);
  totalFx.setup(&totalFxFbo, settings);
  
  // Stripe mixer
  stripeMixer.setup("Stripes");
  stripeMixer.add(offset.setup("offset", 10, 0, 50));
  stripeMixer.add(rotation.setup("rotation", 3, -10, 10));
  stripeMixer.add(blend.setup("blend", 0, 0, 255));
  
  // Global mixer.
  mixer.setup("Global Mixer");
  mixer.add(treesAlpha.setup("Trees", 80.0, 0.0, 255.0));
  mixer.add(stripesAlpha.setup("Stripes", 0.0, 0.0, 255.0));
  mixer.add(noiseAlpha.setup("Noise", 100.0, 100.0, 255.0));
  
  // Add GUI mixers.
  gui.add(&mixer);
  gui.add(&stripeMixer);
}

void ofApp::setupFbos() {
  // FBO settings.
  settings.width = ofGetWidth();
  settings.height = ofGetHeight();
  settings.internalformat = GL_RGBA;
  
  // Allocate FBOs
  stripeFbo.allocate(settings);
  treeFbo.allocate(settings);
  emptyNoiseFbo.allocate(settings);
  totalFxFbo.allocate(settings);
}

void ofApp::updateFbos() {
  // Draw stripes.
  stripeFbo.begin();
    ofClear(255, 255, 255, 0);
    ofBackground(ofColor::black);
    stripeModule.draw();
  stripeFbo.end();
  
  // Draw trees.
  treeFbo.begin();
    ofClear(255, 255, 255, 0);
    ofBackground(ofColor::black);
    treeModule.draw();
  treeFbo.end();
  
  // Empty noise fbo.
  emptyNoiseFbo.begin();
  	ofClear(0, 0, 0, 255);
  emptyNoiseFbo.end();
  
  // -----------PARENT FBO----------- where everything is drawn.
  totalFxFbo.begin();
    ofPushStyle();
      ofClear(0, 0, 0, 255);
      ofEnableBlendMode(OF_BLENDMODE_ADD);
    
      // Noise.
      ofSetColor(ofColor::red, noiseAlpha);
      drawNoiseFbo();
      
      // Audio reactive Stripes.
      ofSetColor(255, stripesAlpha);
      stripeFbo.draw(0, 0);
      
      // Tree with pixels getting unveiled from bottom to up.
      ofSetColor(255, treesAlpha);
      treeFbo.draw(0, 0);
      
      ofEnableAlphaBlending();
    ofPopStyle();
  
    // Disable blend modes.
    ofDisableBlendMode();
    ofDisableAlphaBlending();
  totalFxFbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
  // Update fft.
  CommonFFT::instance().fft.update();
  
  // Update stripes.
  stripeModule.update(offset, rotation, blend);
  // Update trees.
  treeModule.update();
  
  // Update FBOs to draw content.
  updateFbos();
  
  // Setup noiseFx unit.
  noiseFx.getfxUnit(KSMR_FRAGFX_NOISE)->bEnable = true;
  noiseFx.getfxUnit(KSMR_FRAGFX_NOISE)->u_Volume = ofNoise(ofGetElapsedTimef())*5000.0;
  
  //fx switch with key bind
	totalFx.getfxUnit(KSMR_FRAGFX_NOISE)->bEnable		= ofGetKeyPressed('1');
	totalFx.getfxUnit(KSMR_FRAGFX_EDGEONTOP)->bEnable	= ofGetKeyPressed('2');
	totalFx.getfxUnit(KSMR_FRAGFX_FRINGE)->bEnable		= ofGetKeyPressed('3');
	totalFx.getfxUnit(KSMR_FRAGFX_INVERT)->bEnable		= ofGetKeyPressed('4');
	totalFx.getfxUnit(KSMR_FRAGFX_SLANTSHIFT)->bEnable	= ofGetKeyPressed('5');
	totalFx.getfxUnit(KSMR_FRAGFX_TEXCHIP)->bEnable		= ofGetKeyPressed('6');
	totalFx.getfxUnit(KSMR_FRAGFX_VERTNOISE)->bEnable	= ofGetKeyPressed('7');
	totalFx.getfxUnit(KSMR_FRAGFX_VERTSLIDE)->bEnable	= ofGetKeyPressed('8');

	//change uniform parameter
	//fx.getfxUnit(KSMR_FRAGFX_NOISE)->u_Volume = volume;
  // //ofNoise(ofGetElapsedTimef())*5000.0*volume;*/
}

//--------------------------------------------------------------
void ofApp::draw(){
  
  // KSMR effects
  totalFx.applyFx();
  totalFxFbo.draw(0, 0);
  
  gui.draw();
}

void ofApp::drawNoiseFbo() {
  ofPushStyle();
    ofDisableBlendMode();
    noiseFx.applyFx();
    emptyNoiseFbo.draw(0, 0);
  ofPopStyle();
}

// Setup sliders to fade between different effects and sort of overlap them.
// Effects should affected by sound
// Or by X-Y pad that I will setup in the Touch OSC interface.
// But for now, atleast get all the visuals that I have working, then incorporate
// ofxFX, ofxFlowTools, and ofxKrknShader.
