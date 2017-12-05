#include "ofApp.h"
#include "CommonFFT.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(0);
  ofSetCircleResolution(80);
  
  // Touch OSC setup.
  receiver.setup(PORT);
  
  // Load clock video.
  clockPlayer.load("clock.mov");
  clockPlayer.play();
  
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
  mixer.add(treesAlpha.setup("Trees", 130.0, 0.0, 255.0));
  mixer.add(clockAlpha.setup("Clock", 130.0, 0.0, 255));
  mixer.add(stripesAlpha.setup("Stripes", 0.0, 0.0, 255.0));
  mixer.add(noiseAlpha.setup("Noise", 150.0, 80.0, 255.0));
  
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
  clockFbo.allocate(settings);
  clockMaskFbo.allocate(settings);
  totalFxFbo.allocate(settings);
}

void ofApp::updateFbos() {
  // Draw stripes.
  stripeFbo.begin();
    ofClear(255, 255, 255, 0);
    ofBackground(ofColor::black);
    stripeModule.draw();
  stripeFbo.end();
  
  // Mask and update clock FBO.
  clockFbo.begin();
    ofClear(0, 0, 0, 0);
    clockPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
  clockFbo.end();
  
  clockMaskFbo.begin();
    ofClear(0, 0, 0, 0);
    ofPushStyle();
    ofFill();
    //float midVal = CommonFFT::instance().fft.getMidVal();
    //float radius = ofMap(midVal, 0, 1, 300, 350);
    //ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, radius);
    float start = ofGetWidth()/4;
    float end = (ofGetWidth() * 3/4) + 50;
    float xOffset = (end - start)/4;
    float yOffset = (ofGetHeight())/4;
    for (float i = start; i < end; i += xOffset) {
      for (float j = 0; j < ofGetHeight(); j += yOffset) {
        ofSetColor(255);
        ofDrawRectangle(i, j, xOffset - 10, yOffset - 10);
      }
    }
    ofPopStyle();
  clockMaskFbo.end();
  
  clockFbo.getTexture().setAlphaMask(clockMaskFbo.getTexture());
  
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
  
      // Clock video.
      ofSetColor(255, clockAlpha);
      clockFbo.draw(0, 0);
      
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
  // Update video.
  clockPlayer.update();
  
  // Process Touch OSC messages.
  processOSCMessages();
  
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
	updateKsmrFx();
}

void ofApp::updateKsmrFx() {
  // Enable/disable FX.
  totalFx.getfxUnit(KSMR_FRAGFX_NOISE)->bEnable = enableNoise;
	totalFx.getfxUnit(KSMR_FRAGFX_EDGEONTOP)->bEnable	= edgeOnTop;
	totalFx.getfxUnit(KSMR_FRAGFX_FRINGE)->bEnable = fringe;
	totalFx.getfxUnit(KSMR_FRAGFX_INVERT)->bEnable = invert;
	totalFx.getfxUnit(KSMR_FRAGFX_SLANTSHIFT)->bEnable = slantShift;
	totalFx.getfxUnit(KSMR_FRAGFX_TEXCHIP)->bEnable	= texChip;
	totalFx.getfxUnit(KSMR_FRAGFX_VERTNOISE)->bEnable	= vertNoise;
	totalFx.getfxUnit(KSMR_FRAGFX_VERTSLIDE)->bEnable	= vertSlide;
  totalFx.getfxUnit(KSMR_FRAGFX_WATER)->bEnable	= water;
  
  // Uniform parameters
  totalFx.getfxUnit(KSMR_FRAGFX_NOISE)->u_Volume = noiseVolume;
  totalFx.getfxUnit(KSMR_FRAGFX_EDGEONTOP)->u_Volume = edgeOnTop;
  totalFx.getfxUnit(KSMR_FRAGFX_FRINGE)->u_Volume = fringeVolume;
  totalFx.getfxUnit(KSMR_FRAGFX_INVERT)->u_Volume = invertVolume;
  totalFx.getfxUnit(KSMR_FRAGFX_SLANTSHIFT)->u_Volume = slantVolume;
  totalFx.getfxUnit(KSMR_FRAGFX_TEXCHIP)->u_Volume = texVolume;
  totalFx.getfxUnit(KSMR_FRAGFX_VERTNOISE)->u_Volume = vertNoiseVolume;
  totalFx.getfxUnit(KSMR_FRAGFX_VERTSLIDE)->u_Volume = vertSlideVolume;
  totalFx.getfxUnit(KSMR_FRAGFX_WATER)->u_Volume = waterVolume;
}

// Draw function - Extremely simple and straightforward.
void ofApp::draw(){
  
  // KSMR effects
  totalFx.applyFx();
  totalFxFbo.draw(0, 0);
  
  if (!hideGui) {
    gui.draw();
  }
}

void ofApp::drawNoiseFbo() {
  ofPushStyle();
    ofDisableBlendMode();
    noiseFx.applyFx();
    emptyNoiseFbo.draw(0, 0);
  ofPopStyle();
}

void ofApp::keyPressed(int key) {
  if (key == 'a') {
    treeModule.resetMaskPosition();
  }
  if (key == 'b') {
    hideGui = !hideGui;
  }
}

void ofApp::processOSCMessages() {
    
  while(receiver.hasWaitingMessages()){
      // get the next message
      ofxOscMessage m;
      receiver.getNextMessage(m);
    
      // Stripes
      if(m.getAddress() == "/Video/stripes/blend"){
          float val = m.getArgAsFloat(0);
          blend = ofMap(val, 0, 1, 0, 255, true);
      }
      else if(m.getAddress() == "/Video/stripes/offset"){
          float val = m.getArgAsFloat(0);
          offset = ofMap(val, 0, 1, 0, 50, true);
      }
      else if(m.getAddress() == "/Video/stripes/rotation"){
          float val = m.getArgAsFloat(0);
          rotation = ofMap(val, 0, 1, -10, 10, true);
      } //Global Mixer
      else if(m.getAddress() == "/Video/global/noise"){
          float val = m.getArgAsFloat(0);
          noiseAlpha = ofMap(val, 0, 1, 0, 255, true);
      }
      else if(m.getAddress() == "/Video/global/stripes"){
          float val = m.getArgAsFloat(0);
          stripesAlpha = ofMap(val, 0, 1, 0, 255, true);
      }
      else if(m.getAddress() == "/Video/global/clock"){
          float val = m.getArgAsFloat(0);
          clockAlpha = ofMap(val, 0, 1, 0, 255, true);
      }
      else if(m.getAddress() == "/Video/global/tree"){
          float val = m.getArgAsFloat(0);
          treesAlpha = ofMap(val, 0, 1, 0, 255, true);
      } // KSMR FX.
      else if(m.getAddress() == "/Video/ksmr/noise"){
          int val = m.getArgAsInt(0);
          enableNoise = val;
      }
      else if(m.getAddress() == "/Video/ksmr/edge"){
          int val = m.getArgAsInt(0);
          edgeOnTop = val;
      }
      else if(m.getAddress() == "/Video/ksmr/fringe"){
          int val = m.getArgAsInt(0);
          fringe = val;
      }
      else if(m.getAddress() == "/Video/ksmr/invert"){
          int val = m.getArgAsInt(0);
          invert = val;
      }
      else if(m.getAddress() == "/Video/ksmr/slant"){
          int val = m.getArgAsInt(0);
          slantShift = val;
      }
      else if(m.getAddress() == "/Video/ksmr/tex"){
          int val = m.getArgAsInt(0);
          texChip = val;
      }
      else if(m.getAddress() == "/Video/ksmr/vertN"){
          int val = m.getArgAsInt(0);
          vertNoise = val;
      }
      else if(m.getAddress() == "/Video/ksmr/vertS"){
          int val = m.getArgAsInt(0);
          vertSlide = val;
      }
      else if(m.getAddress() == "/Video/ksmr/water"){
          int val = m.getArgAsInt(0);
          water = val;
      } // Uniform volume parameters for
      else if(m.getAddress() == "/Video/ksmr/noiseVolume"){
          float val = m.getArgAsFloat(0);
          noiseVolume = val;
      }
      else if(m.getAddress() == "/Video/ksmr/edgeVolume"){
          float val = m.getArgAsFloat(0);
          edgeVolume = val;
      }
      else if(m.getAddress() == "/Video/ksmr/fringeVolume"){
          float val = m.getArgAsFloat(0);
          fringeVolume = val;
      }
      else if(m.getAddress() == "/Video/ksmr/invertVolume"){
          float val = m.getArgAsFloat(0);
          invertVolume = val;
      }
      else if(m.getAddress() == "/Video/ksmr/slantVolume"){
          float val = m.getArgAsFloat(0);
          slantVolume = val;
      }
      else if(m.getAddress() == "/Video/ksmr/texVolume"){
          float val = m.getArgAsFloat(0);
          texVolume = val;
      }
      else if(m.getAddress() == "/Video/ksmr/vertnVolume"){
          float val = m.getArgAsFloat(0);
          vertNoiseVolume = val;
      }
      else if(m.getAddress() == "/Video/ksmr/vertsVolume"){
          float val = m.getArgAsFloat(0);
          vertSlideVolume = val;
      }
      else if(m.getAddress() == "/Video/ksmr/waterVolume"){
          float val = m.getArgAsFloat(0);
          waterVolume = val;
      }
      else if(m.getAddress() == "/Video/reset"){
          float val = m.getArgAsInt(0);
          if (val) {
            treeModule.resetMaskPosition();
          }
      }
  }
}
// Setup sliders to fade between different effects and sort of overlap them.
// Effects should affected by sound
// Or by X-Y pad that I will setup in the Touch OSC interface.
// But for now, atleast get all the visuals that I have working, then incorporate
// ofxFX, ofxFlowTools, and ofxKrknShader.
