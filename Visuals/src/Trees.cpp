#include "Trees.h"

void Trees::setup() {
  img.load("tree.png");
  img.setImageType(OF_IMAGE_GRAYSCALE);
  mesh.setMode(OF_PRIMITIVE_POINTS);
  img.resize(400, 360);
  ofEnableSmoothing();
  
  // Setup the mesh.
  int pixelSkip = 5;
  for (int x = 0; x < img.getWidth(); x += pixelSkip) {
    for (int y = 0; y < img.getHeight(); y += pixelSkip) {
      float brightness = img.getColor(x, y).getBrightness();
      if (brightness > 20) {
        glm::vec3 pos (x*2, y*2, 0);
        mesh.addVertex(pos);
      }
    }
  }
  
  maskY = ofGetHeight();
  
  // Mask fbo's height will change with time.
  drawFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
  maskFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
}

void Trees::update() {
  // MaskY should decrease over time.
  maskY = ofMap(ofGetElapsedTimeMillis(), 0, endTimeForMask * 1000, ofGetHeight(), 0, true);
  float brightness = 255;
  
  if (ofGetElapsedTimeMillis() >= endTimeForMask * 1000) {
    // Put the mask back slowly and hide the thing completely.
    float startTime = endTimeForMask*1000;
    float endTime = startTime + endTimeForEnding*1000;
    brightness = ofMap(ofGetElapsedTimeMillis(), startTime, endTime, 255, 0);
  }

  center = glm::vec2(ofGetWidth()/2, ofGetHeight()/2);
  
  drawFbo.begin();
    ofClear(0, 0, 0, 0);
    int w = img.getWidth();
    ofPushMatrix();
      ofTranslate(center.x - w - 185, 0);
      // TODO: Set scale for Apple TV
      ofScale(1.5, 1.6);
      drawMesh();
    ofPopMatrix();
  drawFbo.end();
  
  maskFbo.begin();
    ofClear(0, 0, 0, 0);
    ofPushStyle();
      ofSetColor(brightness);
      ofDrawRectangle(0, maskY, ofGetWidth(), ofGetHeight());
    ofPopStyle();
  maskFbo.end();
  
  drawFbo.getTexture().setAlphaMask(maskFbo.getTexture());
}

void Trees::draw() {
  // Since we are rescaling the vertices.
  drawFbo.draw(0, 0);
}

void Trees::drawMesh() {
  ofPushStyle();
    ofSetColor(ofColor::white);
    float noiseX = 0;
    float noiseY = 0;
  
    for (int i = 0; i < mesh.getVertices().size(); i++) {
      if (ofGetElapsedTimeMillis() >= endTimeForMask * 1000) {
        float startTime = endTimeForMask*1000;
        float endTime = startTime + endTimeForEnding*1000;
        float mappedRandomness = ofMap(ofGetElapsedTimeMillis(), startTime, endTime, 5, 450);
        noiseX = ofMap(ofSignedNoise(ofRandom(200)), -1, 1, -mappedRandomness, mappedRandomness);
        noiseY = ofMap(ofSignedNoise(ofRandom(100)), -1, 1, -mappedRandomness, mappedRandomness);
        ofFill();
      } else {
        ofNoFill();
      }
      
      glm::vec2 point(mesh.getVertices()[i].x + noiseX, mesh.getVertices()[i].y + noiseY);
      ofPushMatrix();
        ofTranslate(point);
        ofRotateZDeg(45);
        ofDrawRectangle(0, 0, 6, 6);
      ofPopMatrix();
    }
  ofPopStyle();
}

void Trees::resetMaskPosition() {
  maskY = ofGetHeight();
  ofResetElapsedTimeCounter();
}
