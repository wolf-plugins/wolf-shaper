#include "BipolarModeSwitch.hpp"

START_NAMESPACE_DISTRHO

BipolarModeSwitch::BipolarModeSwitch(Widget *widget, Size<uint> size) noexcept
    : NanoSwitch(widget, size)
{
}

void BipolarModeSwitch::drawSocket()
{
    const float width = getWidth();
    const float height = getHeight();
    const float scaleFactor = getScaleFactor();

    const float centerX = width / 2.0f;
    const float centerY = height / 2.0f;

    const float marginLeftRight = 1.0f * scaleFactor;
    const float marginTopBottom = 2.0f * scaleFactor;
    const float halfMarginTopBottom = marginTopBottom / 2.0f;

    beginPath();
    fillColor(0, 0, 0, 255);

    ellipse(centerX, centerY, centerX - marginLeftRight, centerY - halfMarginTopBottom);
    fill();

    closePath();
}

void BipolarModeSwitch::drawHandle()
{
    const float width = getWidth();
    const float height = getHeight();
    const float scaleFactor = getScaleFactor();

    const float halfWidth = width / 2.0f;

    const float radius = 6.0f * scaleFactor;

    beginPath();

    fillColor(Color(140, 140, 140, 255));

    if (isDown())
        circle(halfWidth, height - radius, radius);
    else
        circle(halfWidth, radius, radius);

    fill();

    closePath();
}

void BipolarModeSwitch::draw()
{
    drawSocket();
    drawHandle();
}

END_NAMESPACE_DISTRHO
