#include "Osc.h"

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
    if (m.getAddress() == "/Audio/rgong/arm") {
      Midi::instance().sendMidiNoteOn(0);
    }
  
    if (m.getAddress() == "/Audio/rgong/rack") {
      Midi::instance().sendMidiNoteOn(1);
    }
  
    if (m.getAddress() == "/Audio/lgong/arm") {
      Midi::instance().sendMidiNoteOn(2);
    }
  
    if (m.getAddress() == "/Audio/play") {
      Midi::instance().sendMidiNoteOn(3);
    }
  
    if (m.getAddress() == "/Audio/stop") {
      Midi::instance().sendMidiNoteOn(4);
    }
  
    if (m.getAddress() == "/Audio/chimes") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        Midi::instance().sendMidiNoteOn(6);
      } else {
        Midi::instance().sendMidiNoteOn(7);
      }
    }
  
    if (m.getAddress() == "/Audio/arp") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        Midi::instance().sendMidiNoteOn(8);
      } else {
        Midi::instance().sendMidiNoteOn(9);
      }
    }
  
    if (m.getAddress() == "/Audio/kick1") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        Midi::instance().sendMidiNoteOn(10);
      } else {
        Midi::instance().sendMidiNoteOn(11);
      }
    }
  
    if (m.getAddress() == "/Audio/kick2") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        Midi::instance().sendMidiNoteOn(13);
      } else {
        Midi::instance().sendMidiNoteOn(14);
      }
    }
  
    if (m.getAddress() == "/Audio/autosine") {
      int val = m.getArgAsInt(0);
      if (val == 1) {
        Midi::instance().sendMidiNoteOn(16);
      } else {
        Midi::instance().sendMidiNoteOn(17);
      }
    }
  
    if (m.getAddress() == "/Audio/rgong/drive") {
      float val = m.getArgAsFloat(0);
      Midi::instance().sendMidiControlChangeRotary(0, val);
    }

    if (m.getAddress() == "/Audio/rgong/frequency") {
      float val = m.getArgAsFloat(0);
      Midi::instance().sendMidiControlChangeRotary(1, val);
    }
  
    if (m.getAddress() == "/Audio/arprate") {
      float val = m.getArgAsFloat(0);
      Midi::instance().sendMidiControlChangeRotary(2, val);
    }
  
    if (m.getAddress() == "/Audio/kicklength") {
      float val = m.getArgAsFloat(0);
      Midi::instance().sendMidiControlChangeRotary(3, val);
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
  }
  else if(m.getAddress() == "/Video/global/clock"){
      sender.sendMessage(m);
  }
   // KSMR FX.
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
