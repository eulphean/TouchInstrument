#pragma once

#include "ofMain.h"

class Stripes {
  public:
    glm::vec2 center;
    
    void setup(int type);
    void update();
    void draw();
    
    void stripeZero();
    void stripeOne();
    
    // 0 or 1
    int stripeType;
    
    int lines;
    int offset;
    int lineHeight;
    int scaleFactor;
    float rotationAngle;
    
    float scale;
    float elapsedTime;
  
    float phase;
};
