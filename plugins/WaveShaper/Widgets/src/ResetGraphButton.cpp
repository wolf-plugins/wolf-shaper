#include "ResetGraphButton.hpp"

START_NAMESPACE_DISTRHO

ResetGraphButton::ResetGraphButton(Window &parent, Size<uint> size) noexcept : NanoButton(parent, size)
{
}

ResetGraphButton::ResetGraphButton(NanoWidget *widget, Size<uint> size) noexcept : NanoButton(widget, size)
{
}

void ResetGraphButton::drawUp()
{
    beginPath();

    strokeWidth(2.0f);
    fillColor(Color(255,0,0,255));
    strokeColor(Color(0,0,0,255));

    rect(0, 0, getWidth(), getHeight());

    fill();
    stroke();

    closePath();
}

void ResetGraphButton::drawDown()
{
    beginPath();
    
    strokeWidth(2.0f);
    fillColor(Color(0,0,255,255));
    strokeColor(Color(0,0,0,255));

    rect(0, 0, getWidth(), getHeight());

    fill();
    stroke();

    closePath();
}

void ResetGraphButton::drawHover() 
{
    beginPath();
    
    strokeWidth(2.0f);
    fillColor(Color(0,255,0,255));
    strokeColor(Color(0,0,0,255));

    rect(0, 0, getWidth(), getHeight());

    fill();
    stroke();

    closePath();
}

END_NAMESPACE_DISTRHO