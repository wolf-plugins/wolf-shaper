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
    paramPreFilterOn,
    paramPreFilterType,
    paramPreFilterCutoff,
    paramPreFilterResonance,
    paramPreFilterPreGain,
    paramPreFilterWet,
    paramPreFilterPostGain,
    paramPostFilterOn,
    paramPostFilterType,
    paramPostFilterCutoff,
    paramPostFilterResonance,
    paramPostFilterPreGain,
    paramPostFilterWet,
    paramPostFilterPostGain,
    paramOut,
    paramCount
};

END_NAMESPACE_DISTRHO

#endif