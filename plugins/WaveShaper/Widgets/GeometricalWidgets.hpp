#ifndef SPOONIE_GEOMETRICAL_WIDGETS_DEFINED_H
#define SPOONIE_GEOMETRICAL_WIDGETS_DEFINED_H

namespace spoonie
{
struct Circle
{
    Circle()
    {
    }

    Circle(float x, float y, float absoluteDiameter) : x(x),
                                                       y(y),
                                                       absoluteDiameter(absoluteDiameter),
                                                       color(Color(255, 255, 255, 255))
    {
    }

    float getRadius()
    {
        return absoluteDiameter / 2.0f;
    }

    float x;
    float y;
    float absoluteDiameter;

    Color color;
};
}

#endif