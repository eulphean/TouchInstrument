// Singleton pattern for handling Midi calls by
// multiple components.

#pragma once
#include "ofMain.h"
#include "ofxMidi.h"

class Midi {
  public:
    void setup();
    void exit();
    void sendMidiControlChangeRotary(int device, float val);
    void sendMidiControlChangeDishes(int device, int midiVal);
    void sendMidiNoteOn(int midiNote);
    
    static Midi &instance();
    
  private:
    // Private constructor.
    Midi();
    static Midi m;
    ofxMidiOut midiOut;
    int channelMidiNote, channelControlChangeDishes, channelControlChangeRotary;
};
