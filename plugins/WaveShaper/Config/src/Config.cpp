#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "Color.hpp"
#include "INIReader.h"
#include "StringManipulation.hpp"

START_NAMESPACE_DISTRHO

namespace WaveShaperConfig
{

enum ColorType
{
    kColorTypeRGB = 3,
    kColorTypeRGBA = 4,
    kColorTypeHSL = 3,
    kColorTypeHSLA = 4
};

bool isLoaded = false;

Color grid_foreground;
Color grid_background;
Color sub_grid;
Color graph_background;
Color in_out_labels;

Color alignment_lines;
Color input_volume_indicator;

Color graph_edges_background_normal;
Color graph_edges_background_focused;

Color graph_edges_foreground_normal;
Color graph_edges_foreground_focused;

Color vertex_fill_normal;
Color vertex_fill_focused;
Color vertex_halo;

Color vertex_stroke_normal;
Color vertex_stroke_focused;

Color tension_handle_normal;
Color tension_handle_focused;

Color plugin_background;
Color graph_margin;
Color top_border;
Color side_borders;

/**
 * Convert a string containing color values to a Color struct.
 * Supported formats: rgb(rgb), rgba(r,g,b,a), hsl(h,s,l), hsla(h,s,l,a)
 */
static Color colorFromString(std::string color)
{
    const char *str = color.c_str();
    char const *rest;

    std::string colorTypeString = spoonie::takeUntil(str, '(');
    ColorType colorType;

    unsigned char x, y, z;
    unsigned char a = 255;
    int scanStatus;

    if (colorTypeString == "rgb")
    {
        scanStatus = sscanf(str, " rgb ( %hhu , %hhu , %hhu ) ", &x, &y, &z);
        colorType = kColorTypeRGB;
    }
    else if (colorTypeString == "hsl")
    {
        scanStatus = sscanf(str, " hsl ( %hhu , %hhu , %hhu ) ", &x, &y, &z);
        colorType = kColorTypeHSL;
    }
    else if (colorTypeString == "rgba")
    {
        scanStatus = sscanf(str, " rgba ( %hhu , %hhu , %hhu , %hhu ) ", &x, &y, &z, &a);
        colorType = kColorTypeRGBA;
    }
    else if (colorTypeString == "hsla")
    {
        scanStatus = sscanf(str, " hsla ( %hhu , %hhu , %hhu , %hhu ) ", &x, &y, &z, &a);        
        colorType = kColorTypeHSLA;
    }
    else
    {
        fprintf(stderr, "spoonie-waveshaper: Warning! Invalid color in config file.\n");

        return Color(255, 255, 255, 255);
    }

    if(scanStatus == colorType)
    {
        if(colorType == kColorTypeRGB || colorType == kColorTypeRGBA)
            return Color(x, y, z, a);
        else
            return Color::fromHSL(x, y, z, a / 255.f);
    }
    
    fprintf(stderr, "spoonie-waveshaper: Warning! Invalid color in config file.\n");

    return Color(255,255,255,255);
}

void load()
{
    std::string homeDirectory = std::string(getenv("HOME"), 255);
    std::string pathToConfig = homeDirectory + "/.config/spoonie-waveshaper.conf";

    INIReader reader(pathToConfig);

    if (reader.ParseError() < 0)
    {
        std::cout << "Can't load 'spoonie-waveshaper.conf', setting default config\n";
        return;
    }

    std::cout << "Config loaded from 'spoonie-waveshaper.conf'\n";

    grid_foreground = colorFromString(reader.Get("colors", "grid_foreground", "rgba(255,255,255,255)"));

    isLoaded = true;
}
}

END_NAMESPACE_DISTRHO