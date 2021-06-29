#include "Oversampler.hpp"

START_NAMESPACE_DISTRHO

Oversampler::Oversampler() : fRatio(-1),
                             fSampleRate(44100),
                             fNumSamples(512),
                             fLowPass1(),
                             fLowPass2(),
                             fCurrentCapacity(fNumSamples),
                             fRequiredCapacity(fNumSamples)
{
    fBuffer = (float **)malloc(sizeof(float *) * 2);
    fBuffer[0] = (float *)malloc(fNumSamples * sizeof(float));
    fBuffer[1] = (float *)malloc(fNumSamples * sizeof(float));
}

Oversampler::~Oversampler()
{
    free(fBuffer[1]);
    free(fBuffer[0]);
    free(fBuffer);
}

float **Oversampler::upsample(int ratio, uint32_t numSamples, double sampleRate, const float * const *audio)
{
    if (fSampleRate != sampleRate * ratio || fRatio != ratio)
    {
        fSampleRate = sampleRate * ratio;

        fFilterCenter = sampleRate / 2.0f - 4000; //FIXME

        fLowPass1.reset();
        fLowPass1.setup(8, fSampleRate, fFilterCenter);

        fLowPass2.reset();
        fLowPass2.setup(8, fSampleRate, fFilterCenter);
    }

    fRatio = ratio;
    fNumSamples = numSamples;

    fRequiredCapacity = numSamples * ratio;

    if (fCurrentCapacity < fRequiredCapacity)
    {
        fBuffer[0] = (float *)realloc(fBuffer[0], fRequiredCapacity * sizeof(float));
        fBuffer[1] = (float *)realloc(fBuffer[1], fRequiredCapacity * sizeof(float));

        fCurrentCapacity = fRequiredCapacity;
    }

    for (uint32_t i = 0; i < numSamples; ++i)
    {
        const int index = i * fRatio; //TODO: find a better name for this variable

        fBuffer[0][index] = audio[0][i];
        fBuffer[1][index] = audio[1][i];

        for (int j = 1; j < fRatio; ++j)
        {
            fBuffer[0][index + j] = 0.0f;
            fBuffer[1][index + j] = 0.0f;
        }
    }

    if (fRatio > 1)
    {
        lowPass1();
        gainBoost();
    }

    return fBuffer;
}

void Oversampler::downsample(float **targetBuffer)
{
    if (fRatio > 1)
        lowPass2();

    for (uint32_t i = 0; i < fNumSamples; ++i)
    {
        targetBuffer[0][i] = fBuffer[0][i * fRatio];
        targetBuffer[1][i] = fBuffer[1][i * fRatio];
    }
}

void Oversampler::lowPass1()
{
    fLowPass1.process(fRequiredCapacity, fBuffer);
}

void Oversampler::lowPass2()
{
    fLowPass2.process(fRequiredCapacity, fBuffer);
}

void Oversampler::gainBoost()
{
    for (uint32_t i = 0; i < fRequiredCapacity; ++i)
    {
        fBuffer[0][i] *= fRatio;
        fBuffer[1][i] *= fRatio;
    }
}

END_NAMESPACE_DISTRHO