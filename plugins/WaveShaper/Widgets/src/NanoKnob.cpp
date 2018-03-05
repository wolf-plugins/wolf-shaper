#include "NanoKnob.hpp"
#include "Mathf.hpp"

#include "Fonts/roboto_light.hpp"

START_NAMESPACE_DISTRHO

NanoKnob::NanoKnob(Window &parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      fMin(0.0f),
      fMax(1.0f),
      fStep(0.0f),
      fValue(0.5f),
      fUsingLog(false),
      fLeftMouseDown(false),
      fColor(Color(255, 0, 0, 255)),
      fCallback(nullptr)
{
    setSize(size);

    using namespace SPOONIE_FONTS;
    createFontFromMemory("roboto_light", (const uchar*)roboto_light, roboto_light_size, 0);
}

NanoKnob::NanoKnob(NanoWidget *widget, Size<uint> size) noexcept
    : NanoWidget(widget),
      fMin(0.0f),
      fMax(1.0f),
      fStep(0.0f),
      fValue(0.5f),
      fUsingLog(false),
      fLeftMouseDown(false),
      fColor(Color(255, 0, 0, 255)),
      fCallback(nullptr)
{
    setSize(size);

    using namespace SPOONIE_FONTS;
    createFontFromMemory("roboto_light", (const uchar*)roboto_light, roboto_light_size, 0);
}

float NanoKnob::getValue() const noexcept
{
    return fValue;
}

void NanoKnob::setRange(float min, float max) noexcept
{
    DISTRHO_SAFE_ASSERT(min < max);

    fMin = min;
    fMax = max;

    fValue = spoonie::clamp(fValue, min, max);
}

void NanoKnob::setStep(float step) noexcept
{
    fStep = step;
}

float NanoKnob::getMin() noexcept
{
    return fMin;
}

float NanoKnob::getMax() noexcept
{
    return fMax;
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

void NanoKnob::setColor(Color color) noexcept
{
    fColor = color;
}

Color NanoKnob::getColor() noexcept
{
    return fColor;
}

void NanoKnob::onNanoDisplay()
{
    drawNormal();
}

bool NanoKnob::onMouse(const MouseEvent &ev)
{
    if (ev.button != 1)
        return fLeftMouseDown;

    Window &window = getParentWindow();

    if (!ev.press)
    {
        if (fLeftMouseDown == true)
        {
            fLeftMouseDown = false;

            window.setCursorPos(this);
            window.showCursor();

            return true;
        }

        return false;
    }

    if (contains(ev.pos))
    {
        fLeftMouseDownLocation = ev.pos;
        fLeftMouseDown = true;

        window.hideCursor();

        return true;
    }

    return false;
}

bool NanoKnob::onMotion(const MotionEvent &ev)
{
    if (fLeftMouseDown)
    {
        const float resistance = 280.0f;
        const float difference = (fLeftMouseDownLocation.getY() - ev.pos.getY()) / resistance * (fMax - fMin);

        fLeftMouseDownLocation.setY(ev.pos.getY());

        setValue(fValue + difference, true);

        return true;
    }

    return false;
}

bool NanoKnob::onScroll(const ScrollEvent &ev)
{
    if (!contains(ev.pos))
        return false;

    const float resistance = 38.0f;

    setValue(getValue() + ev.delta.getY() / resistance * (fMax - fMin), true);

    return true;
}

END_NAMESPACE_DISTRHO