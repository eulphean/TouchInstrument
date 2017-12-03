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
  
  stripeFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
  
  gui.setup();
  
  // Stripe mixer
  stripeMixer.setup("Stripes");
  stripeMixer.add(offset.setup("offset", 10, 0, 50));
  stripeMixer.add(rotation.setup("rotation", 3, -10, 10));
  //stripeMixer.add(rotation.setup("blend1", 0, 0, 255));
  stripeMixer.add(blend.setup("blend", 0, 0, 255));
  
  player.load("ableton.mov");
  player.play();
  /*ofLoadImage (img, "prussik.jpg");
  grabber.setup(ofGetWidth(), ofGetHeight());
  player.play();*/
  
  // Ksmr shader
  ksmrShader.setup("Ksmr shader");
  ksmrShader.add(volume.setup("Volume", 1.0, 0.0, 1.0));
  
  mixer.setup("Global Mixer");
  //mixer.add(imageAlpha.setup("image", 100.0, 0.0, 255.0));
  mixer.add(playerAlpha.setup("video", 0.0, 0.0, 255.0));
  //mixer.add(grabberAlpha.setup("camera", 100.0, 0.0, 255.0));
  
  gui.add(&mixer);
  
  gui.add(&stripeMixer);
  
  gui.add(&ksmrShader);
  
  setting.width  = 512;
	setting.height = 512;

	original.allocate(setting);

	//ksmrFX setup
	fx.setup(&original, setting);
}

//--------------------------------------------------------------
void ofApp::update(){
  // Update fft.
  CommonFFT::instance().fft.update();
  
  stripeModule.update(offset, rotation, blend);
  
  // Draw the module in fbo.
  stripeFbo.begin();
    ofClear(255, 255, 255, 0);
    ofBackground(ofColor::black);
    stripeModule.draw();
  stripeFbo.end();
  
  player.update();
  /*grabber.update();
  if (grabber.isFrameNew()){
    // Do something with pixels from the grabber.
  }*/
  
  //fx switch with key bind
	fx.getfxUnit(KSMR_FRAGFX_NOISE)->bEnable		= ofGetKeyPressed('1');
	fx.getfxUnit(KSMR_FRAGFX_EDGEONTOP)->bEnable	= ofGetKeyPressed('2');
	fx.getfxUnit(KSMR_FRAGFX_FRINGE)->bEnable		= ofGetKeyPressed('3');
	fx.getfxUnit(KSMR_FRAGFX_INVERT)->bEnable		= ofGetKeyPressed('4');
	fx.getfxUnit(KSMR_FRAGFX_SLANTSHIFT)->bEnable	= ofGetKeyPressed('5');
	fx.getfxUnit(KSMR_FRAGFX_TEXCHIP)->bEnable		= ofGetKeyPressed('6');
	fx.getfxUnit(KSMR_FRAGFX_VERTNOISE)->bEnable	= ofGetKeyPressed('7');
	fx.getfxUnit(KSMR_FRAGFX_VERTSLIDE)->bEnable	= ofGetKeyPressed('8');

	//change uniform parameter
	fx.getfxUnit(KSMR_FRAGFX_VERTNOISE)->u_Volume = volume;
  // //ofNoise(ofGetElapsedTimef())*5000.0*volume;
}

//--------------------------------------------------------------
void ofApp::draw(){

  //draw src buffer
	original.begin();{
		ofClear(0, 0, 0, 255);

		ofSetColor(255);
		//player.draw(0, 0, ofGetWidth(), ofGetHeight());
    stripeFbo.draw(0, 0);
	}original.end();


	ofSetColor(255);
	//draw original buffer
	original.draw(0, 0);

	//apply active Effects
	fx.applyFx();

	//draw applied buffer
	original.draw(512, 0);

  /*ofEnableBlendMode(OF_BLENDMODE_ADD);
  
  ofSetColor(255, 255-playerAlpha);
  stripeFbo.draw(0, 0);
  
  
  ofSetColor (255, playerAlpha);
  player.draw(0, 0, ofGetWidth(), ofGetHeight());
  
  ofEnableAlphaBlending();*/
  
  
  gui.draw();
}

// Setup sliders to fade between different effects and sort of overlap them.
// Effects should affected by sound
// Or by X-Y pad that I will setup in the Touch OSC interface.
// But for now, atleast get all the visuals that I have working, then incorporate
// ofxFX, ofxFlowTools, and ofxKrknShader.
