#include "OversampleWheel.hpp"

START_NAMESPACE_DISTRHO

void OversampleWheel::draw() 
{
    const char* oversamplingFactors[] = {"", "x2", "x4", "x8", "x16"};

    beginPath();

    fillColor(Color(255,0,0,255));
    rect(0, 0, getWidth(), getHeight());
    fill();

    closePath();

    fontSize(4.f);
    fillColor(Color(100,100,100, 255));
    textAlign(ALIGN_CENTER);

    const int value = getValue();

    if(value > 0)
        text(0, 0, oversamplingFactors[getValue()], NULL);
}

END_NAMESPACE_DISTRHO