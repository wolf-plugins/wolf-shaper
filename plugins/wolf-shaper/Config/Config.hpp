#ifndef WOLF_SHAPER_CONFIG_H_INCLUDED
#define WOLF_SHAPER_CONFIG_H_INCLUDED

START_NAMESPACE_DISTRHO

#define CONFIG_NAMESPACE WolfShaperConfig
namespace CONFIG_NAMESPACE
{
extern bool isLoaded;

extern Color grid_foreground;
extern Color grid_background;
extern Color sub_grid;
extern Color graph_background;
extern Color grid_middle_line_horizontal;
extern Color grid_middle_line_vertical;
extern Color in_out_labels;
extern Color alignment_lines;
extern Color input_volume_indicator;
extern Color playhead_circle_fill;
extern Color playhead_circle_stroke;
extern Color graph_edges_background_normal;
extern Color graph_edges_background_focused;
extern float graph_edges_stroke_width;
extern Color graph_edges_foreground_normal;
extern Color graph_edges_foreground_focused;
extern Color graph_gradient_icol;
extern Color graph_gradient_ocol;
extern Color vertex_fill_normal;
extern Color vertex_fill_focused;
extern Color vertex_halo;
extern Color vertex_stroke_normal;
extern Color vertex_stroke_focused;
extern float vertex_radius;
extern float vertex_stroke_width;
extern Color tension_handle_normal;
extern Color tension_handle_focused;
extern float tension_handle_radius;
extern float tension_handle_stroke_width;
extern Color plugin_background;
extern Color graph_margin;
extern Color top_border;
extern Color side_borders;


void load();
}

END_NAMESPACE_DISTRHO

#endif