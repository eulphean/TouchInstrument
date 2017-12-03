#include "Stripes.h"
#include "CommonFFT.h"

void Stripes::setup() {
  offset = 10;
  rotation = 3;
  ofEnableAlphaBlending();
  
  //fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F);
  //fbo.begin();
    //  ofBackground(0);
  //fbo.end();
}

void Stripes::update(float newOffset, float newRotation, float newBlend) {
  center = glm::vec2(ofGetWidth()/2, ofGetHeight()/2);
  float band = CommonFFT::instance().fft.getLoudBand();
  float maxVol = CommonFFT::instance().fft.getSmoothedUnScaledLoudestValue();
  
  elapsedTime = ofGetElapsedTimef();
  scale = ofSignedNoise(elapsedTime) * maxVol;
  
  // Update values from Sliders.
  offset = newOffset;
  rotation = newRotation;
  blend = newBlend;
}

void Stripes::draw() {
  /*ofPushStyle();
    ofEnableAlphaBlending();
      ofSetColor(0, 0, 0, 2);
      ofFill();
      ofRect(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
      ofSetColor(50, 50, 255, 255);
      ofEnableBlendMode(OF_BLENDMODE_ADD);
      ofCircle(ofRandomWidth()/2, ofRandomHeight(), 50);
    stripeZero();
    fbo.begin();*/
  
  /*ofPushStyle();
      ofEnableBlendMode(OF_BLENDMODE_ADD);
      ofSetColor (255, blend1);
      stripeZero();
      ofSetColor (255, blend2);
      stripeOne();
      ofEnableAlphaBlending();
      ofEnableBlendMode(OF_BLENDMODE_ADD);
  ofPopStyle();
  */
  stripeZero();
  stripeOne();
  
}

// Architectural stripes.
void Stripes::stripeZero() {
  ofPushMatrix();
    ofPushStyle();
      ofTranslate(center.x, center.y);
      for (int x = -lines; x <= lines; x++) {
        ofPushMatrix();
          ofTranslate(x * offset, 0);
          ofRotateZDeg(x * rotation);
          ofScale(scale, scale);
        
          int color = ofMap(sin(elapsedTime), -1, 1, 0, 255);
          ofColor hue = ofColor::fromHsb(color, 255, 255);
          ofSetColor(hue, blend);
        
          ofSetLineWidth(2);
          ofDrawLine(x, -lineHeight, x, lineHeight);
        ofPopMatrix();
      }
    ofPopStyle();
  ofPopMatrix();
}

// Sonic stripes.
void Stripes::stripeOne() {
  // Represent fft bars with stripes.
  
  vector<float> fftSpectrum = CommonFFT::instance().fft.getSpectrum();
  int numOfBins = CommonFFT::instance().fft.getNumFFTbins();
  
  ofPushStyle();
    ofPushMatrix();
      ofTranslate(numOfBins/2, center.y);
      float offset = ofGetWidth() / numOfBins;
      for (int x = 0; x < fftSpectrum.size(); x++) {
        ofPushMatrix();
          ofTranslate(x*offset, 0);
          int color = ofMap(cos(elapsedTime), -1, 1, 0, 255);
          ofColor hue = ofColor::fromHsb(color, 255, 255);
          ofSetColor(hue, 255-blend);
          
          // Since fft values are normalized, we range it from -0-1
          float stripeHeight = ofMap(fftSpectrum[x], 0, 1, 0, ofGetHeight() -50);
        
          ofSetLineWidth(5);
          ofDrawLine(0, -stripeHeight/2, 0, stripeHeight/2);
        ofPopMatrix();
      }
    ofPopMatrix();
  ofPopStyle();
}
