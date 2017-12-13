#include "Osc.h"

void Osc::setup() {
  // Setup OSC.
  receive.setup(PORT);
}

void Osc::update() {
    // Touch OSC updates.
  while (receive.hasWaitingMessages()) {
    ofxOscMessage m;
    // Set the next message.
    #pragma warning(disable: WARNING_CODE)
    receive.getNextMessage(&m);
    
    processAudioSignals(m);
    processOscillatorSignals(m);
  }
}

void Osc::processAudioSignals(ofxOscMessage &m){
    if (m.getAddress() == "/Audio/playpause") {
      // Do something.
    }
  
    if (m.getAddress() == "/Audio/delay") {
      // Do something.
    }
  
    if (m.getAddress() == "/Audio/pitch") {
      // Do something.
    }
  
    if (m.getAddress() == "/Audio/decimation") {
      // Do something.
    }
  
    // Rotary knob float values. 
    if (m.getAddress() == "/Audio/gain") {
      // Do something.
    }
}

void Osc::processOscillatorSignals(ofxOscMessage &m) {
  if (m.getAddress() == "/Oscillator/sine") {
      // Trigger sine
    }
  
  if (m.getAddress() == "/Oscillator/square") {
    // Trigger square.
  }

  if (m.getAddress() == "/Oscillator/triangle") {
    // Trigger triangle.
  }

  if (m.getAddress() == "/Oscillator/effect/pitch") {
    // Change pitch.
  }

  if (m.getAddress() == "/Oscillator/effect/delay") {
    // Change delay.
  }
  
  // Rotary knob. Float values
  if (m.getAddress() == "/Oscillator/gain") {
    // Change delay.
  }
}

