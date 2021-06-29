#ifndef WOLF_SHAPER_PARAMETERS_HPP_INCLUDED
#define WOLF_SHAPER_PARAMETERS_HPP_INCLUDED

#include "src/DistrhoDefines.h"

START_NAMESPACE_DISTRHO

enum Parameters
{
    paramPreGain = 0,
    paramWet,
    paramPostGain,
    paramRemoveDC,
    paramOversample,
    paramBipolarMode,
    paramHorizontalWarpType,
    paramHorizontalWarpAmount,
    paramVerticalWarpType,
    paramVerticalWarpAmount,
    paramOut,
    paramCount
};

END_NAMESPACE_DISTRHO

#endif