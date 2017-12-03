#pragma once

#include "ofMain.h"
#include "ofxProcessFFT.h"

class CommonFFT {
  public:
    static CommonFFT &instance();
    ProcessFFT fft;
  private:
    static CommonFFT m;
};
