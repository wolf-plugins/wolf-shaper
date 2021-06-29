#ifndef WOLF_OVERSAMPLER_INCLUDED
#define WOLF_OVERSAMPLER_INCLUDED

#include "DspFilters/Dsp.h"
#include "src/DistrhoDefines.h"
#include "extra/LeakDetector.hpp"

START_NAMESPACE_DISTRHO

class Oversampler
{
  public:
    Oversampler();
    ~Oversampler();

    float **upsample(int ratio, uint32_t numSamples, double sampleRate, const float * const *audio);
    void downsample(float **targetBuffer);

  protected:
    void lowPass1();
    void lowPass2();

    void gainBoost();

  private:
    int fRatio;
    double fSampleRate;
    uint32_t fNumSamples;
    Dsp::SimpleFilter<Dsp::Butterworth::LowPass<8>, 2> fLowPass1;
    Dsp::SimpleFilter<Dsp::Butterworth::LowPass<8>, 2> fLowPass2;

    uint32_t fCurrentCapacity;
    uint32_t fRequiredCapacity;
    float **fBuffer;

    float fFilterCenter;

    DISTRHO_LEAK_DETECTOR(Oversampler)
};

END_NAMESPACE_DISTRHO

#endif