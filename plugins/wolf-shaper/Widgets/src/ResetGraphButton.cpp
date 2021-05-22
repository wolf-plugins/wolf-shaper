#include "ResetGraphButton.hpp"

START_NAMESPACE_DISTRHO

ResetGraphButton::ResetGraphButton(Widget  *widget, Size<uint> size) noexcept : NanoButton(widget, size)
{
}

void ResetGraphButton::drawUp()
{
    const float width = getWidth();
    const float height = getHeight();

    const float halfWidth = width / 2.0f;
    const float halfHeight = height / 2.0f;

    //arrow
    beginPath();

    strokeWidth(3.0f);
    strokeColor(Color(255, 255, 255, 255));

    const float margin = 6.0f;
    const float halfMargin = margin / 2.0f;

    const float radius = halfWidth - margin;

    arc(halfWidth, halfHeight, radius, 3.47321f, 2.98451f, NanoVG::Winding::CW);

    moveTo(margin, margin);
    lineTo(margin, radius + halfMargin);
    lineTo(radius + halfMargin, radius + halfMargin);

    stroke();

    closePath();
}

void ResetGraphButton::drawDown()
{
    drawHover();
}

void ResetGraphButton::drawHover()
{
    drawUp();

    beginPath();

    strokeWidth(1.0f);
    strokeColor(Color(255, 255, 255, 255));

    const float margin = 2.0f;
    const float doubleMargin = margin * 2.0f;

    roundedRect(margin, margin, getWidth() - doubleMargin, getHeight() - doubleMargin, 4.0f);

    stroke();

    closePath();
}

void ResetGraphButton::draw()
{
    switch (getButtonState())
    {
    case kNanoStateDown:
        drawDown();
        break;
    case kNanoStateHover:
        drawHover();
        break;
    default:
        drawUp();
        break;
    }
}

END_NAMESPACE_DISTRHO