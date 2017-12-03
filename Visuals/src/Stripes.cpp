#include "Stripes.h"
#include "CommonFFT.h"

void Stripes::setup(int type) {
  stripeType = type;
  lines = 400;
  offset = 10;
  lineHeight = 100;
  phase = 0;
}

void Stripes::update() {
  center = glm::vec2(ofGetWidth()/2, ofGetHeight()/2);
  float band = CommonFFT::instance().fft.getLoudBand();
  float maxVol = CommonFFT::instance().fft.getSmoothedUnScaledLoudestValue();
  
  elapsedTime = ofGetElapsedTimef();
  scale = ofSignedNoise(elapsedTime) * maxVol/10;
}

void Stripes::draw() {
  switch (stripeType) {
    case 0: {
      stripeZero();
      break;
    }
    
    case 1: {
      stripeOne();
      break;
    }
  }
}

// Architectural stripes.
void Stripes::stripeZero() {
  ofPushMatrix();
    ofPushStyle();
      ofTranslate(center.x, center.y);
      for (int x = -lines; x <= lines; x++) {
        ofPushMatrix();
          ofTranslate(x * offset, 0);
          ofRotateZDeg(x * 3);
          ofScale(scale, scale);
        
          int color = ofMap(sin(elapsedTime), -1, 1, 0, 255);
          ofColor hue = ofColor::fromHsb(color, 255, 255);
          ofSetColor(hue);
        
          ofSetLineWidth(2);
          ofDrawLine(x, -lineHeight, x, lineHeight);
        ofPopMatrix();
      }
    ofPopStyle();
  ofPopMatrix();
}

// Sonic stripes.
void Stripes::stripeOne() {
  ofSetLineWidth(2);
  ofSetColor(ofColor::black);
  for(int i =-lines; i<lines; i++){
    // Stack the current coordinate system.
    ofPushMatrix();
    ofTranslate(i * 20, 0);

    // Draw lines along the base curve.
    //drawLine();
    
    // Reset the coordinate system.
    ofPopMatrix();
  }
}
/*
void Stripes::drawLine() {
  // Potentially read this line from some real time data.
  float length;
  float hue;
  
  
  if (useConstantLength) {
    length = defaultLength;
    hue = ofMap(ofGetHeight(), 0, ofGetHeight(), 0, 255);
  } else {
    length = ofRandom(0, ofGetHeight());
    hue = ofMap(length, 0, ofGetHeight(), 0, 255);
  }
  
  ofSetColor(ofColor::fromHsb(hue, 255, 255));
  ofDrawLine(0, length, 0, -length);
}
*/
