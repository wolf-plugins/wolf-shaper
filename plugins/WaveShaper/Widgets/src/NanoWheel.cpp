#include "NanoWheel.hpp"
#include "Mathf.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

NanoWheel::NanoWheel(Window &parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      fLeftMouseDown(false),
      fValue(0),
      fMin(0),
      fMax(0)
{
    setSize(size);
}

NanoWheel::NanoWheel(NanoWidget *parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      fLeftMouseDown(false),
      fValue(0),
      fMin(0),
      fMax(0)
{
    setSize(size);
}

void NanoWheel::setRange(int min, int max) noexcept
{
    fMin = min;
    fMax = max;

    fValue = spoonie::clamp(fValue, min, max);
}

void NanoWheel::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoWheel::setValue(int value, bool sendCallback) noexcept
{
    value = spoonie::clamp(value, fMin, fMax);

    if (fValue == value)
        return;

    fValue = value;

    if (sendCallback && fCallback != nullptr)
        fCallback->nanoWheelValueChanged(this, fValue);

    repaint();
}

int NanoWheel::getValue() noexcept
{
    return fValue;
}

void NanoWheel::onNanoDisplay()
{
    draw();
}

bool NanoWheel::onScroll(const ScrollEvent &ev)
{
    if (!contains(ev.pos))
        return false;

    setValue(getValue() + ev.delta.getY(), true);

    return true;
}

bool NanoWheel::onMouse(const MouseEvent &ev)
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

bool NanoWheel::onMotion(const MotionEvent &ev)
{
    if (fLeftMouseDown)
    {
        const int value = (fLeftMouseDownLocation.getY() - ev.pos.getY()) / 20;

        if (value != 0)
        {
            fLeftMouseDownLocation.setY(ev.pos.getY());

            setValue(fValue + spoonie::clamp(value, -1, 1), true);
        }

        return true;
    }

    return false;
}

END_NAMESPACE_DISTRHO