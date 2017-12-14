// Author: Amay Kataria
// Date: 11/03/2017

#include "AudioPlayer.h"

void AudioPlayer::patch() {
    //--------PATCHING-------
  
    // ADSR Trigger - Without this the sample wouldn't play.
    sampleTrig >> sampler >> sampleGainAmp >> amp;
    envGate >> env.set(0.0f, 70.0f, 0.3f, 100.0f) >> amp.in_mod();
  
    // Default gain is 1.0f. Let all the signal pass through this.
    1.0f >> sampleGainAmp.in_mod();
  
    // Oscillator amp.
    // Turn on the amp right away. We will control the oscillators
    // by triggering individual of them on/off.
    oscAmpTrigger >> oscAmpEnv.set(3.0f, 50.0f, 1.0f, 50.0f) >> oscAmp.in_mod();
    oscAmpTrigger.trigger(1.0f);
  
    // Default frequency of the sample.
    decimator.set(20000);
  
    // Route the sampler to the output.
    sampleTrig >> sampler * 2.0f >> amp >> delay >> decimator >> engine.audio_out(0);
                             amp >> delay >> decimator >> engine.audio_out(1);
  
    // Defaut state of the system.
    sampleState = stopped;

    // Select the first sample by default.
    sampleIdx = 0;
    sampleIdx >> sampler.in_select();

    // Initialize all the oscillators.
    initOscillators();

    //------------SETUPS AND START AUDIO-------------
    engine.listDevices();
    engine.setDeviceID(1); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    engine.setup(44100, 512, 2);
}

void AudioPlayer::checkToLoopSample() {
  // Check if sound needs to be looped.
  float meter_position = getMeterPosition();
  
  // Check and loop the sample.
  if (meter_position > 1.0f && sampleState == playing) {
    // Audio is done playing.
    sampleState = stopped;
  
    // Repeat the current sample
    sampleIdx >> sampler.in_select();
    
    // Play the sample.
    play();
  }
}

// Update SampleAudio and Oscillator sound with any capacitive values. 
void AudioPlayer::update(float capRange) {
  // Update samples with audio effects.
  updateSampleAudio(capRange);
  
  // Update oscillators with audio effects.
  updateOscillator(capRange);
}

void AudioPlayer::initOscillators() {
    oscillators.resize(oscillatorCount);
    oscillatorTriggers.resize(oscillatorCount);
  
    int idx = 0;
    for (TouchOscillator &osc : oscillators) {
      // Default pitch and trigger.
      defaultOscillatorPitches[idx] >> osc.in_pitch();
      oscillatorTriggers[idx] >> osc.in_trig();
      oscillatorTriggers[idx] >> amp.in_mod();
      switch (idx) {
        // Sine
        case 0: {
          osc.out_sin() * 0.5f >> oscAmp >> oscDelay >> engine.audio_out(0);
                                  oscAmp >> oscDelay >> engine.audio_out(1);
          break;
        }
        
        // Square
        case 1: {
          osc.out_pulse() * 0.5f >> oscAmp >> oscDelay >> engine.audio_out(0);
                                    oscAmp >> oscDelay >> engine.audio_out(1);
          break;
        }
        
        // Triangle
        case 2: {
          osc.out_triangle() * 0.02f >> oscAmp >> oscDelay >> engine.audio_out(0);
                                        oscAmp >> oscDelay >> engine.audio_out(1);
          break;
        }
        
        default: {
          break;
        }
      }
      idx++;
    }
}

void AudioPlayer::addAudioEffect(SampleEffect effect) {
  currentSampleEffects.push_back(effect);
}

void AudioPlayer::removeAudioEffect(SampleEffect effect) {
  if (currentSampleEffects.size() == 0) {
    return;
  }
  
  std::vector<SampleEffect>::iterator it = find(currentSampleEffects.begin(), currentSampleEffects.end(), effect);
  
  // Reset that Audio effect that's removed.
  switch (effect) {
    case sDelay: {
      //Reset delay.
      0.0f >> delay.in_time();
      0.0f >> delay.in_feedback();
      break;
    }
    
    case sDecimation: {
      // Reset decimation.
      20000 >> decimator.in_freq();
      break;
    }
    
    case sPitch: {
      // Reset pitch.
      0.0f >> sampler.in_pitch();
      break;
    }
    
    default: {
      break;
    }
  }
  
  currentSampleEffects.erase(it);
}

void AudioPlayer::setAudioSampleGain(float oscVal) {
  float newGain = ofMap(oscVal, 0, 1, 0.0f, 10.0f, true);
  newGain >> sampleGainAmp.in_mod();
}

// Update audio if any effects are applied.
void AudioPlayer::updateSampleAudio(float capRange) {
    // Don't do anything if no effect is applied. 
    if (currentSampleEffects.size() == 0) {
      // No effects in the collection, no audio to change. 
      return;
    }
  
    for (SampleEffect &effect : currentSampleEffects) {
      switch (effect) {
        // Feedback
        case sDelay: {
          float newFeedbackTime = ofMap(capRange, 0.0f, 1.0f, 1.0, 0.0f, true);
          newFeedbackTime >> delay.in_feedback();
          break;
        }
        
        // Decimation
        case sDecimation: {
          // Calculate the new decimator frequency based on the brightness.
          float newDecimatorFrequency = ofMap(capRange, 0.0f, 1.0f, 10000, 500, true);
          newDecimatorFrequency >> decimator.in_freq();
          break;
        }
        
        // Pitch
        case sPitch: {
          // Change pitch, opposite of the pattern of decimation frequency.
          float newPitch = ofMap(capRange, 0.0f, 1.0f, -6.0f, 10.0f, true);
          newPitch >> sampler.in_pitch();
          break;
        }
        
        default:
          break;
      }
    }
}

void AudioPlayer::updateOscillator(float capRange) {
  if (currentOscillatorEffects.size() == 0) {
    // No effects in the collection, return.
    return;
  }
  
  // Loop through each sound effect and apply it on the sound.
  for (OscillatorEffect &effect : currentOscillatorEffects) {
    switch (effect) {
      case oPitch: {
        float newPitch = ofMap(capRange, 0.0f, 1.0f, 45.0f, 85.0f, true);
        
        // Only update the pitch for the oscillators that are turned on.
        for (TouchOscillator &osc : oscillators) {
          if (osc.getIsOscOn()) {
            newPitch >> osc.in_pitch();
          }
        }
        break;
      }
      
      case oDelay: {
        float newDelayTime = ofMap(capRange, 0.0f, 1.0f, 0, 3000.0f, true);
        float newFeedbackTime = ofMap(capRange, 0.0f, 1.0f, 0, 3.0f, true);

        newDelayTime >> delay.in_time();
        newFeedbackTime >> delay.in_feedback();
        break;
      }
      
      default:
        break;
    }
  }
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
    cout << "Sample amp " << amp.meter_mod() << endl;
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
void AudioPlayer::startOscillator(Oscillator osc) {
  int idx = static_cast<int>(osc);
  oscillatorTriggers[idx].trigger(1.0f);
  // Turn on the oscillator.
  oscillators[idx].setIsOscOn(true);
  cout << "OSC_amp " << oscAmp.meter_mod() << endl;
}

void AudioPlayer::stopOscillator(Oscillator osc) {
  int idx = static_cast<int>(osc);
  // Turn off the oscillator off.
  oscillators[idx].setIsOscOn(false);
  oscillatorTriggers[idx].off();
}

void AudioPlayer::setOscillatorGain(float oscVal) {
  float newGain = ofMap(oscVal, 0, 1, 0.2f, 2.5f, true);
  newGain >> oscAmp.in_mod();
}

void AudioPlayer::addOscillatorEffect(OscillatorEffect effect) {
  currentOscillatorEffects.push_back(effect);
}

void AudioPlayer::removeOscillatorEffect(OscillatorEffect effect) {
  if (currentOscillatorEffects.size() == 0) {
    return;
  }
  std::vector<OscillatorEffect>::iterator it = find(currentOscillatorEffects.begin(), currentOscillatorEffects.end(), effect);
  
  // Loop through each sound effect and apply it on the sound.
  for (OscillatorEffect &effect : currentOscillatorEffects) {
    switch (effect) {
      case oPitch: {
        // Only update the pitch for the oscillators that are turned on.
        for (TouchOscillator &osc : oscillators) {
          if (osc.getIsOscOn()) {
            0.0f >> osc.in_pitch();
          }
        }
        break;
      }
      
      case oDelay: {
        0.0f >> delay.in_time();
        0.0f >> delay.in_feedback();
        break;
      }
      
      default:
        break;
    }
  }
  
  currentOscillatorEffects.erase(it);
}
