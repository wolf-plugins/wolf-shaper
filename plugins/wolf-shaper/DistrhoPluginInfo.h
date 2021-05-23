/*
 * Wolf Shaper audio effect based on DISTRHO Plugin Framework (DPF)
 *
 * SPDX-License-Identifier: GPL v3+
 *
 * Copyright (C) 2021 Patrick Desaulniers
 */

#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_NAME  "Wolf Shaper"
#define DISTRHO_PLUGIN_BRAND "Wolf Plugins"
#define DISTRHO_PLUGIN_URI   "https://github.com/pdesaulniers/wolf-shaper"

#define DISTRHO_PLUGIN_HAS_UI          1
#define DISTRHO_UI_USE_NANOVG          1
#define DISTRHO_UI_USER_RESIZABLE      1

#define DISTRHO_PLUGIN_NUM_INPUTS      2
#define DISTRHO_PLUGIN_NUM_OUTPUTS     2
#define DISTRHO_PLUGIN_WANT_PROGRAMS   0
#define DISTRHO_PLUGIN_USES_MODGUI     0
#define DISTRHO_PLUGIN_WANT_STATE      1
#define DISTRHO_PLUGIN_WANT_FULL_STATE 0
#define DISTRHO_PLUGIN_LV2_CATEGORY "lv2:WaveshaperPlugin"

/*
 * See https://lv2plug.in/ns/lv2core#hardRTCapable
 * I believe this plugin is not hard rt-capable, because the amount of time for a run() call depends on the plugin state.
 */
#define DISTRHO_PLUGIN_IS_RT_SAFE      0

#endif
