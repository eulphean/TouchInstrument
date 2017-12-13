// Author: Amay Kataria
// Date: 10/16/2017
// Description: Declares a TouchOscillator class, which is responsible to play an oscillating
// sample when a connection happens. This could be a sin, square or a triangle wave.

#pragma once

#include "ofxPDSP.h"

class TouchOscillator : public pdsp::Patchable{
    public:
        TouchOscillator();
        TouchOscillator(const TouchOscillator & other) { patch(); }
        void patch();
    
        // Inputs
        pdsp::Patchable& in_trig();
        pdsp::Patchable& in_pitch();
    
        // Outputs
        pdsp::Patchable& out_sin();
        pdsp::Patchable& out_saw();
        pdsp::Patchable& out_triangle();
        pdsp::Patchable& out_pulse();
  
        // Check to see if the oscillator is turned on.
        bool getIsOscOn();
        void setIsOscOn(bool flag);
    
    // Characteristics of this oscillator. 
    private:
        pdsp::Amp amp;
        pdsp::VAOscillator osc;
        pdsp::ADSR env;
        pdsp::CRSlew ampSlew;
        pdsp::CRSlew pitchSlew;
        bool isOn = false;
};
