// Author: Amay Kataria
// Date: 10/16/2017
// Description: Definition of the TouchOscillator and its capabilities. 

#include "TouchOscillator.h"

// Constructor of Touch Oscillator. 
TouchOscillator::TouchOscillator() {
    patch();
}

void TouchOscillator::patch() {
    // Add inputs / outputs with these methods
    addModuleInput("trig", env.in_trig()); // arguments are tag and the Unit in/out to link to that tag
    addModuleInput("pitch", osc.in_pitch());
    addModuleOutput("signal", amp); // if in/out is not selected default in/out is used
    
    // Patching
    env.set(0.0f, 70.0f, 0.3f, 100.0f);
    env >> amp.in_mod();

    // Do this for enabling smoothing.
    pitchSlew.set(500.0f) >> osc.in_pitch(); // 250 millisecond slew
    ampSlew.set(100.0f)  >> amp.in_mod(); // 100 millisecond slew
    
    // Default output patched is a sine wave.
    osc.out_sine() >> amp;
}

pdsp::Patchable& TouchOscillator::in_trig(){
    return in("trig");
}

pdsp::Patchable& TouchOscillator::in_pitch(){
    return in("pitch");
}

pdsp::Patchable& TouchOscillator::out_sin(){
    osc.out_sine() >> amp;
    return out("signal");
}

pdsp::Patchable& TouchOscillator::out_saw(){
    osc.out_saw() >> amp;
    return out("signal");
}

pdsp::Patchable& TouchOscillator::out_triangle(){
    osc.out_triangle() >> amp;
    return out("signal");
}

pdsp::Patchable& TouchOscillator::out_pulse(){
    osc.out_pulse() >> amp;
    return out("signal");
}
