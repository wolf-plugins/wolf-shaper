#include "NanoKnob.hpp"

START_NAMESPACE_DISTRHO

NanoKnob::NanoKnob(Window &parent, Size<uint> size) noexcept
    : Widget(parent),
      fMinimum(0.0f),
      fMaximum(1.0f),
      fStep(0.0f),
      fValue(0.5f),
      fUsingLog(false),
      fRotationAngle(0),
      fDragging(false),
      fLastX(0),
      fLastY(0),
      fCallback(nullptr)
{
    setSize(size);
}

NanoKnob::NanoKnob(Widget *widget, Size<uint> size) noexcept
    : Widget(widget->getParentWindow()),
      fMinimum(0.0f),
      fMaximum(1.0f),
      fStep(0.0f),
      fValue(0.5f),
      fUsingLog(false),
      fRotationAngle(0),
      fDragging(false),
      fLastX(0),
      fLastY(0),
      fCallback(nullptr)
{
    setSize(size);
}

float NanoKnob::getValue() const noexcept
{
    return fValue;
}

void NanoKnob::setRange(float min, float max) noexcept
{
    fMinimum = min;
    fMaximum = max;

    fValue = spoonie::clamp(fValue, min, max);
}

void NanoKnob::setStep(float step) noexcept
{
    fStep = step;
}

// NOTE: value is assumed to be scaled if using log
void NanoKnob::setValue(float value, bool sendCallback) noexcept
{
    value = spoonie::clamp(value, fMin, fMax);

    if (d_isEqual(fValue, value))
        return;

    fValue = value;

    if (sendCallback && fCallback != nullptr)
        fCallback->nanoKnobValueChanged(this, fValue);

    repaint();
}

void NanoKnob::setUsingLogScale(bool yesNo) noexcept
{
    fUsingLog = yesNo;
}

void NanoKnob::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoKnob::setRotationAngle(int angle)
{
    if (fRotationAngle == angle)
        return;

    DISTRHO_SAFE_ASSERT(angle < 210 || angle > 330);
    DISTRHO_SAFE_ASSERT(angle >= 0 && angle <= 360);

    fRotationAngle = angle % 360;
}

void NanoKnob::onNanoDisplay()
{
    /*drawNormal();
      drawHover();
      drawTurning();*/
}

bool NanoKnob::onMouse(const MouseEvent &ev)
{
    if (ev.button != 1)
        return false;

    return false;
}

bool NanoKnob::onMotion(const MotionEvent &ev)
{
    if (!fDragging)
        return false;

    return true;
}

bool NanoKnob::onScroll(const ScrollEvent &ev)
{
    if (!contains(ev.pos))
        return false;

    return true;
}

END_NAMESPACE_DISTRHO