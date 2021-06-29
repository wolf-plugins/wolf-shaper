#include "ParamSmooth.hpp"

START_NAMESPACE_DISTRHO

ParamSmooth::ParamSmooth() : fHistory(0.0f),
                             fValue(0.0f)
{
}

ParamSmooth::ParamSmooth(float value) : fHistory(0.0f),
                                        fValue(value),
                                        fCoeff(0.0f)
{
}

void ParamSmooth::calculateCoeff(float frequency, double sampleRate)
{
    fCoeff = std::exp(-2.0 * M_PI * frequency / sampleRate);
}

void ParamSmooth::setValue(float value)
{
    fValue = value;
}

float ParamSmooth::getRawValue() const
{
    return fValue;
}

float ParamSmooth::getSmoothedValue()
{
    float result = fValue + fCoeff * (fHistory - fValue);
    fHistory = result;

    return result;
}

END_NAMESPACE_DISTRHO