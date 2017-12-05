#include "Osc.h"
#include "Midi.h"

void Osc::setup() {
  // Setup OSC.
  receive.setup(PORT);
  sender.setup(HOST, PORTTOSEND);
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
    
    processAudioMessages(m);
    
    // We receive these and route them directly to Mac Mini.
    processVideoMessages(m);
  }
}

void Osc::processAudioMessages(ofxOscMessage &m) {
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

// Route these messages directly to Mac mini.
void Osc::processVideoMessages(ofxOscMessage &m) {
  if(m.getAddress() == "/Video/stripes/blend"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/stripes/offset"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/stripes/rotation"){
      sender.sendMessage(m);
  } // Global Mixer
  else if(m.getAddress() == "/Video/global/noise"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/global/stripes"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/global/tree"){
      sender.sendMessage(m);
  } // KSMR FX.
  else if(m.getAddress() == "/Video/ksmr/noise"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/edge"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/fringe"){
      sender.sendMessage(m);  }
  else if(m.getAddress() == "/Video/ksmr/invert"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/slant"){
     sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/tex"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/vertN"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/vertS"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/water"){
      sender.sendMessage(m);
  } // Uniform volume parameters for KSMR FX.
  else if(m.getAddress() == "/Video/ksmr/noiseVolume"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/edgeVolume"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/fringeVolume"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/invertVolume"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/slantVolume"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/texVolume"){
      sender.sendMessage(m);  }
  else if(m.getAddress() == "/Video/ksmr/vertnVolume"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/vertsVolume"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/ksmr/waterVolume"){
      sender.sendMessage(m);
  }
  else if(m.getAddress() == "/Video/reset"){
      sender.sendMessage(m);
  }
}
