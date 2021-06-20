#include "ArrowButton.hpp"

START_NAMESPACE_DISTRHO

ArrowButton::ArrowButton(Widget  *widget, Size<uint> size) noexcept : NanoButton(widget, size),
                                                                         fArrowDirection(Left)
{
}

void ArrowButton::setArrowDirection(ArrowDirection direction)
{
    fArrowDirection = direction;
}

void ArrowButton::drawArrow()
{
    const float width = getWidth();
    const float height = getHeight();

    const float centerY = height / 2.0f;

    const float marginLeftRight = 5.0f;
    const float marginTopBottom = 5.0f;

    beginPath();

    if(fArrowDirection == Left)
    {
        translate(width, 0);
        scale(-1, 1); // flip
    }

    fillColor(255, 255, 255, 255);

    moveTo(marginLeftRight, marginTopBottom);

    lineTo(marginLeftRight, height - marginTopBottom);
    lineTo(width - marginLeftRight, centerY);
    lineTo(marginLeftRight, marginTopBottom);

    fill();
    
    closePath();
}

void ArrowButton::drawBackground()
{
    const float width = getWidth();
    const float height = getHeight();

    beginPath();

    fillColor(Color(52,52,52, 255));
    strokeColor(Color(64, 64, 64, 255));
    strokeWidth(2.0f);

    rect(0,0,width,height);
    fill();
    stroke();

    closePath();
}

void ArrowButton::draw()
{
    drawBackground();
    drawArrow();
}

END_NAMESPACE_DISTRHO