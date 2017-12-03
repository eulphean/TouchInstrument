#pragma once

#include "ofMain.h"

class Stripes {
  public:
    void setup(int type);
    void update(float offset, float rotation);
    void draw();
  
    glm::vec2 center;
  
    // 0 or 1
    int stripeType;
    void stripeZero();
    void stripeOne();
  
    // Const variables.
    const int lines = 400;
    const int lineHeight = 100;
  
    // Slider variables.
    float offset;
    float rotation;
  
    // FFT/Time modified variables.
    float scale;
    float elapsedTime;
};
