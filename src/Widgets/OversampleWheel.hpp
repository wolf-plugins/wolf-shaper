#ifndef WOLF_OVERSAMPLE_WHEEL_HPP_INCLUDED
#define WOLF_OVERSAMPLE_WHEEL_HPP_INCLUDED

#include "NanoWheel.hpp"

START_NAMESPACE_DISTRHO

class OversampleWheel : public NanoWheel
{
public:
    explicit OversampleWheel(Widget  *widget, Size<uint> size) noexcept;

    void setFontSize(float size);

protected:
    void draw() override;
    void drawTriangles();
    void drawBackground();
    void drawText();
    void drawOutline();
    
private:
    float fFontSize;

    DISTRHO_LEAK_DETECTOR(OversampleWheel)
};

END_NAMESPACE_DISTRHO

#endif