#ifndef WOLF_SHAPER_CONFIG_H_INCLUDED
#define WOLF_SHAPER_CONFIG_H_INCLUDED

#include "Color.hpp"

START_NAMESPACE_DISTRHO

#define CONFIG_NAMESPACE WolfShaperConfig
namespace CONFIG_NAMESPACE
{

static const Color grid_foreground = Color(103, 98, 102, 255);
static const Color grid_background = Color(25, 24, 26, 255);
static const Color sub_grid = Color(27, 27, 27, 255);
static const Color graph_background = Color(40, 40, 47, 255);
static const Color grid_middle_line_horizontal = Color(103, 98, 102, 255);
static const Color grid_middle_line_vertical = Color(103, 98, 102, 255);
static const Color in_out_labels = Color(255, 255, 255, 125);

static const Color alignment_lines = Color(200, 200, 200, 180);
static const Color input_volume_indicator = Color(255, 255, 255, 180);

static const Color playhead_circle_fill = Color(255, 255, 255, 180);
static const Color playhead_circle_stroke = Color(255, 255, 255, 180);

static const float graph_edges_stroke_width = 2.0f;
static const Color graph_edges_background_normal = Color(169, 29, 239, 100);
static const Color graph_edges_background_focused = Color(255, 221, 76, 100);

static const Color graph_edges_foreground_normal = Color(239, 61, 227, 255);
static const Color graph_edges_foreground_focused = Color(255, 150, 255, 255);

static const Color graph_gradient_icol = Color(60, 60, 60, 20);
static const Color graph_gradient_ocol = Color(60, 60, 60, 60);

static const Color vertex_fill_normal = Color(255, 255, 255, 255);
static const Color vertex_fill_focused = Color(255, 255, 255, 255);

static const Color vertex_halo = Color(0, 0, 0, 255);

static const Color vertex_stroke_normal = Color(0, 0, 0, 255);
static const Color vertex_stroke_focused = Color(0, 0, 0, 255);

static const float vertex_radius = 7.0f;
static const float vertex_stroke_width = 2.0f;

static const Color tension_handle_normal = Color(228, 104, 181, 255);
static const Color tension_handle_focused = Color(255, 150, 255, 255);

static const float tension_handle_radius = 4.5f;
static const float tension_handle_stroke_width = 2.0f;

static const Color plugin_background = Color(42, 44, 47, 255);
static const Color graph_margin = Color(33, 32, 39, 255);
static const Color top_border = Color(0, 0, 0, 255);
static const Color side_borders = Color(100, 100, 100, 255);
static const Color right_click_menu_border_color = Color(10, 10, 10, 255);

} // namespace CONFIG_NAMESPACE

END_NAMESPACE_DISTRHO

#endif
