#ifndef WAVESHAPER_LAYOUT_DEFINED_H
#define WAVESHAPER_LAYOUT_DEFINED_H

#include "src/DistrhoDefines.h"

START_NAMESPACE_DISTRHO

struct Margin
{
    Margin(float left, float right, float top, float bottom);

    float left;
    float right;
    float top;
    float bottom;
};

END_NAMESPACE_DISTRHO

#endif