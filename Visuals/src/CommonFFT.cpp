#include "CommonFFT.h"

CommonFFT &CommonFFT::instance() {
  return m;
}

CommonFFT CommonFFT::m;
