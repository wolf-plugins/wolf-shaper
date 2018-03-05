#include "OversampleWheel.hpp"

START_NAMESPACE_DISTRHO

OversampleWheel::OversampleWheel(Window &parent, Size<uint> size) noexcept : NanoWheel(parent, size)
{
    loadSharedResources();
}

OversampleWheel::OversampleWheel(NanoWidget *widget, Size<uint> size) noexcept : NanoWheel(widget, size)
{
    loadSharedResources();
}

void OversampleWheel::draw()
{
    const float width = getWidth();
    const float height = getHeight();

    const char *oversamplingFactors[] = {"", "2x", "4x", "8x", "16x"};

    resetTransform();

    beginPath();

    fillColor(Color(255, 0, 0, 255));
    rect(0, 0, width, height);
    fill();

    closePath();

    const int value = getValue();

    if (value > 0)
    {
        beginPath();

        fontFace(NANOVG_DEJAVU_SANS_TTF);
        fontSize(height);
        fillColor(Color(255, 255, 255, 255));
        textAlign(ALIGN_RIGHT | ALIGN_BOTTOM);

        text(width - 2, height + 1, oversamplingFactors[getValue()], NULL);

        closePath();
    }
}

END_NAMESPACE_DISTRHO