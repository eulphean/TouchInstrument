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
  
  center = glm::vec2(ofGetWidth()/2, ofGetHeight()/2);
  
  drawFbo.begin();
    ofClear(0, 0, 0, 0);
    int w = img.getWidth();
    ofPushMatrix();
      ofTranslate(center.x - w, 0);
      // TODO: Set scale for Apple TV
      ofScale(1, 1.1);
      drawMesh();
    ofPopMatrix();
  drawFbo.end();
  
  maskFbo.begin();
    ofClear(0, 0, 0, 0);
    ofPushStyle();
      ofSetColor(255);
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
    ofNoFill();
    ofSetColor(ofColor::white);
  
    for (int i = 0; i < mesh.getVertices().size(); i++) {
      glm::vec2 point(mesh.getVertices()[i].x, mesh.getVertices()[i].y);
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
  ofResetElapsedTimeCounter();6
}
