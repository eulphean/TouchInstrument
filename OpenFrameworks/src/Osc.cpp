#include "Osc.h"
#include "Midi.h"

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
    
    // Notes can range from 0 - 127. Make sure no two note
    // numbers are same.
    
    // Kick off Scene 1
    if (m.getAddress() == "/Scenes/4/1") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
          Midi::instance().sendMidiNoteOn(0);
//        currentScene = 1;
      }
    }
    
    // Kick off Scene 2
    if (m.getAddress() == "/Scenes/4/2") {
      int val = m.getArgAsInt(0);
      if(val == 1) {
          Midi::instance().sendMidiNoteOn(1);
//        currentScene = 2;
      }
    }
    
    // Kick off Scene 3
    if (m.getAddress() == "/Scenes/4/3") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
          Midi::instance().sendMidiNoteOn(2);
//        currentScene = 3;
      }
    }
    
    // Kick off Scene 4
    if (m.getAddress() == "/Scenes/4/4") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
          Midi::instance().sendMidiNoteOn(3);
//        currentScene = 4;
      }
    }
    
    // Kick off Scene 5
    if (m.getAddress() == "/Scenes/4/5") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
          Midi::instance().sendMidiNoteOn(4);
//        currentScene = 5;
      }
    }
    
    // Kick off Scene 6
    if (m.getAddress() == "/Scenes/4/6") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
          Midi::instance().sendMidiNoteOn(5);
//        currentScene = 6;
      }
    }
    
    // ------------------------ Rotary Knobs -----------------------
    
    // Left rotary.
    if (m.getAddress() == "/RotaryLeft") {
      float val = m.getArgAsFloat(0);
      Midi::instance().sendMidiControlChangeRotary(0, val);
    }
    
    // Right rotary.
    if (m.getAddress() == "/RotaryRight") {
      float val = m.getArgAsFloat(0);
      Midi::instance().sendMidiControlChangeRotary(1, val);
    }
    
    // ------------------------ Fader Knobs -----------------------
    
    // Fader 1
    if (m.getAddress() == "/Scenes/fader1") {
      int val = m.getArgAsInt(0);
    }
    
    // Fader 2
    if (m.getAddress() == "/Scenes/fader2") {
      float val = m.getArgAsFloat(0);
    }
    
    // Fader 3
    if (m.getAddress() == "/Scenes/fader3") {
      float val = m.getArgAsFloat(0);
    }
    
    // Fader 4
    if (m.getAddress() == "/Scenes/fader4") {
      float val = m.getArgAsFloat(0);
    }
    
    // Fader 5
    if (m.getAddress() == "/Scenes/fader5") {
      float val = m.getArgAsFloat(0);
    }
  }
}
