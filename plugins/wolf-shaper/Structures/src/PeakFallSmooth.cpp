#include "PeakFallSmooth.hpp"

START_NAMESPACE_DISTRHO

PeakFallSmooth::PeakFallSmooth() : fHistory(0.0f),
                                   fValue(0.0f)
{
}

PeakFallSmooth::PeakFallSmooth(const float value) : fHistory(0.0f),
                                                    fValue(value),
                                                    fCoeff(0.0f)
{
}

void PeakFallSmooth::calculateCoeff(const float frequency, const double sampleRate)
{
    fCoeff = std::exp(-2.0 * M_PI * frequency / sampleRate);
}

void PeakFallSmooth::setValue(const float value)
{
    if (fHistory < value)
        fHistory = value;

    fValue = value;
}

float PeakFallSmooth::getRawValue() const
{
    return fValue;
}

float PeakFallSmooth::getSmoothedValue()
{
    float result = fValue + fCoeff * (fHistory - fValue);
    fHistory = result;

    return result;
}

END_NAMESPACE_DISTRHO