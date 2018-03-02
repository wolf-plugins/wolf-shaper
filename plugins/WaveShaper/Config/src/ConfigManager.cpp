#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "ConfigManager.hpp"
#include "Color.hpp"
#include "INIReader.h"
#include "StringManipulation.hpp"

START_NAMESPACE_DISTRHO

/**
 * Convert a string containing color values to a Color struct.
 * Supported formats: rgb(rgb), rgba(r,g,b,a), hsl(h,s,l), hsla(h,s,l,a)
 */
static Color colorFromString(std::string color)
{
    const char *str = color.c_str();
    char *rest;

    std::string token = spoonie::takeUntil(str, '(');

    if(token == "rgb" || token == "rgba") 
    {

    }
    else if(token == "hsl" || token == "hsla")
    {

    }

    return Color(0,0,0,0);
}

void ConfigManager::loadConfig() 
{
    const char *homeDirectory = getenv("HOME");

    INIReader reader("/home/spoonie/.config/spoonie-waveshaper.conf");

    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'spoonie-waveshaper.conf', setting default config\n";
        return;
    }

    std::cout << "Config loaded from 'spoonie-waveshaper.conf'\n";

    Color testColor = colorFromString(reader.Get("colors", "grid_foreground", "rgba(255,255,255,255)"));
    reader.Get("user", "name", "UNKNOWN");
    reader.Get("user", "email", "UNKNOWN");
    reader.GetReal("user", "pi", -1);
    reader.GetBoolean("user", "active", true);
}

END_NAMESPACE_DISTRHO