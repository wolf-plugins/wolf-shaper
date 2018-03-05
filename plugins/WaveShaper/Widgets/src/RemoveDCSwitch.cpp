#include "RemoveDCSwitch.hpp"

START_NAMESPACE_DISTRHO

RemoveDCSwitch::RemoveDCSwitch(Window &parent, Size<uint> size) noexcept : NanoSwitch(parent, size)
{
}

RemoveDCSwitch::RemoveDCSwitch(NanoWidget *widget, Size<uint> size) noexcept : NanoSwitch(widget, size)
{
}

void RemoveDCSwitch::drawUp()
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

void RemoveDCSwitch::drawDown()
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

END_NAMESPACE_DISTRHO