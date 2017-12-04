#pragma once

#include "ofMain.h"

class Trees {
  public:
    void setup();
    void update();
    void draw();
    void drawMesh();
  
    ofImage img;
    ofMesh mesh;
    glm::vec2 center;
  
  
};

