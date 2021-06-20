#ifndef WOLF_PARAM_SMOOTH_INCLUDED
#define WOLF_PARAM_SMOOTH_INCLUDED

#include "src/DistrhoDefines.h"
#include "extra/LeakDetector.hpp"

START_NAMESPACE_DISTRHO

class ParamSmooth
{
public:
  ParamSmooth();
  ParamSmooth(float value);

  void calculateCoeff(float frequency, double sampleRate);

  float getSmoothedValue();
  float getRawValue() const;

  void setValue(float value);

private:
  float fHistory;
  float fValue;
  float fCoeff;

  DISTRHO_LEAK_DETECTOR(ParamSmooth)
};

END_NAMESPACE_DISTRHO

#endif