#ifndef WOLF_PEAK_FALL_SMOOTH_INCLUDED
#define WOLF_PEAK_FALL_SMOOTH_INCLUDED

#include "src/DistrhoDefines.h"
#include "extra/LeakDetector.hpp"

START_NAMESPACE_DISTRHO

class PeakFallSmooth
{
public:
  PeakFallSmooth();
  PeakFallSmooth(const float value);

  void calculateCoeff(const float frequency, const double sampleRate);

  float getSmoothedValue();
  float getRawValue() const;

  void setValue(const float value);

private:
  float fHistory;
  float fValue;
  float fCoeff;

  DISTRHO_LEAK_DETECTOR(PeakFallSmooth)
};

END_NAMESPACE_DISTRHO

#endif