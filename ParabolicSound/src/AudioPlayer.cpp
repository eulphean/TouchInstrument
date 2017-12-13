// Author: Amay Kataria
// Date: 11/03/2017

#include "AudioPlayer.h"

void AudioPlayer::patch() {
    //--------PATCHING-------
  
    // ADSR Trigger - Without this the sample wouldn't play.
    sampleTrig >> sampler >> amp;
    envGate    >> env >> amp.in_mod();
    env >> amp.in_mod();
  
    // Default frequency of the sample.
    decimator.set(20000);
  
    // Route the sampler to the output.
    sampleTrig >> sampler >> amp >> delay >> decimator >> engine.audio_out(0);
                             amp >> delay >> decimator >> engine.audio_out(1);
  
    // Defaut state of the system.
    sampleState = stopped;
  
    // Default state of the effects.
    currentEffect = 0;

    // Select the first sample by default.
    sampleIdx = 0;
    sampleIdx >> sampler.in_select();

    // Setup sample oscillator
    defaultOscillatorPitch >> touchOsc.in_pitch();
    touchOscTrigger >> touchOsc.in_trig();
    touchOsc.out_sin() * 0.5f >> engine.audio_out(0);
    touchOsc.out_sin() * 0.5f >> engine.audio_out(1);

    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(1); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup(44100, 512, 2);
}

void AudioPlayer::update(float capRange) {
  // Update sound for the effect that's currently on.
  updateSample(capRange);
  
  // Update oscillators
  updateOscillator(capRange);
  
  // Check if sound needs to be looped.
  float meter_position = getMeterPosition();
  
  // Check and loop the sample.
  if (meter_position > 1.0f) {
    // Audio is done playing.
    sampleState = stopped;
  
    // Repeat the current sample
    sampleIdx >> sampler.in_select();
    
    // Play the sample.
    play();
  }
}

void AudioPlayer::updateSample(float capRange) {
    switch (currentEffect) {
      // Feedback
      case 0: {
        // Reset decimation.
        10000 >> decimator.in_freq();
        
        // Reset pitch.
        0.0f >> sampler.in_pitch();
        
        float newDelayTime = ofMap(capRange, 0.0f, 1.0f, 0, 3000.0f, true);
        float newFeedbackTime = ofMap(capRange, 0.0f, 1.0f, 0, 3.0f, true);
        
        newDelayTime >> delay.in_time();
        newFeedbackTime >> delay.in_feedback();
        break;
      }
      
      // Decimation
      case 1: {
        // Reset pitch.
        0.0f >> sampler.in_pitch();
        
        // Reset delay.
        0.0f >> delay.in_time();
        0.0f >> delay.in_feedback();
        
        // Calculate the new decimator frequency based on the brightness.
        float newDecimatorFrequency = ofMap(capRange, 0.0f, 1.0f, 10000, 500, true);
        newDecimatorFrequency >> decimator.in_freq();
        break;
      }
      
      // Pitch
      case 2: {
        // Reset decimation.
        10000 >> decimator.in_freq();
        
        // Reset delay.
        0.0f >> delay.in_time();
        0.0f >> delay.in_feedback();
        
        // Change pitch, opposite of the pattern of decimation frequency.
        float newPitch = ofMap(capRange, 0.0f, 1.0f, -6.0f, 0.0f, true);
        newPitch >> sampler.in_pitch();
        break;
      }
      
      default:
        break;
    }
}

void AudioPlayer::setNextEffect() {  
  currentEffect++;
  currentEffect = currentEffect % totalEffects;
}

void AudioPlayer::setNextSample() {
  // Stop the current sample
  stop();
  
  // Load next sample.
  sampleIdx++;
  sampleIdx = sampleIdx % totalSamples;
  sampleIdx >> sampler.in_select();
  
  // Play that sample.
  play();
}


float AudioPlayer::getMeterPosition() {
   return sampler.meter_position();
}

void AudioPlayer::addSample(string path) {
  samples.push_back( new pdsp::SampleBuffer() );
  samples.back()->load(path);
  sampler.addSample(samples.back());
}

// Load an audio sample. 
void AudioPlayer::play() {
  if (sampleState == paused){
    envGate.trigger(1.0f);
  } else if (sampleState == stopped) {
    envGate.trigger(1.0f);
    sampleTrig.trigger(1.0f);
  }
  sampleState = playing;
}

void AudioPlayer::pause(){
  if (sampleState == playing) {
    sampleState = paused;
    envGate.off();
  }
}

void AudioPlayer::stop(){
  if (sampleState == playing) {
    sampleState = stopped;
    envGate.off();
  }
}

State AudioPlayer::getPlaybackState() {
  return sampleState;
}


// Oscillators.
void AudioPlayer::startOscillator() {
  touchOscTrigger.trigger(1.0f);
}

void AudioPlayer::stopOscillator() {
  touchOscTrigger.off();
}

void AudioPlayer::updateOscillator(float capRange) {
  float newPitch = ofMap(capRange, 0.0f, 1.0f, 45.0f, 85.0f);
  newPitch >> touchOsc.in_pitch();
}
