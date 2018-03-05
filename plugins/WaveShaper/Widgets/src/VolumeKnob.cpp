#include "VolumeKnob.hpp"

START_NAMESPACE_DISTRHO

VolumeKnob::VolumeKnob(Window &parent, Size<uint> size) noexcept : NanoKnob(parent, size)
{
}

VolumeKnob::VolumeKnob(NanoWidget *widget, Size<uint> size) noexcept : NanoKnob(widget, size)
{
}

void VolumeKnob::drawNormal()
{
    const float width = getWidth();
    const float height = getHeight();
    const Color color = getColor();

    const float value = getValue();
    const float min = getMin();
    const float max = getMax();

    const float percentFilled = (value - min) / (max - min);

    const float radius = height / 2.0f;

    const float indicatorLineHeight = radius - 5;
    const float indicatorLineWidth = 2.0f;
    const float indicatorLineMarginTop = 2.0f;

    const float gaugeWidth = 3.5f;
    Color gaugeColor = Color(0,0,40,255);
    gaugeColor.interpolate(color, 0.4f);

    const float margin = 2.0f;
    
    //Knob
    beginPath();

    fillColor(Color(100, 100, 100, 255));
    circle(radius, radius, radius - gaugeWidth - margin);
    fill();

    //Gauge (empty)
    beginPath();
    
    strokeWidth(gaugeWidth);
    strokeColor(gaugeColor);
    arc(radius, radius, radius - margin, 0.75f * M_PI, 0.25f * M_PI, NanoVG::Winding::CW);
    stroke();

    //Gauge (value)
    beginPath();

    strokeWidth(gaugeWidth - 1.0f);
    strokeColor(color);
    arc(radius, radius, radius - margin, 0.75f * M_PI, (0.75f + 1.5f * percentFilled) * M_PI, NanoVG::Winding::CW);
    stroke();  

    //Indicator line
    beginPath();
    save();

    translate(radius, radius);
    rotate((2.0f + ((percentFilled - 0.5f) * 1.5f)) * M_PI);
    translate(-radius, -radius);

    fillColor(color);
    strokeWidth(1.0f);
    roundedRect(radius - indicatorLineWidth / 2.0f, margin + indicatorLineMarginTop, indicatorLineWidth, indicatorLineHeight, 2.0f);
    fill();

    restore();

    //Label
    /*fontFace("roboto_light");
    fontSize(32.f);
    fillColor(255, 255, 255, 125);

    textAlign(ALIGN_BOTTOM | ALIGN_RIGHT);
    text(getWidth() - 5, getHeight(), "In", NULL);

    textAlign(ALIGN_TOP | ALIGN_LEFT);
    text(5, 0, "Out", NULL);*/
    
    closePath();
}

void VolumeKnob::drawHover()
{
}

void VolumeKnob::drawTurning()
{
}

END_NAMESPACE_DISTRHO