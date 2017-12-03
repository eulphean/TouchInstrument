#pragma once

#include "ofMain.h"

class Stripes {
  public:
    void setup();
    void update(float offset, float rotation, float blend);
    void draw();
  
    glm::vec2 center;

    void stripeZero();
    void stripeOne();
  
    // Const variables.
    const int lines = 400;
    const int lineHeight = 100;
  
    // Slider variables.
    float offset;
    float rotation;
    float blend;
  
    // FFT/Time modified variables.
    float scale;
    float elapsedTime;
  
    float stripeFbo;
};
