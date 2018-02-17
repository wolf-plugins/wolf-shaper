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
    loadSharedResources();

    setSize(size);
}

NanoWheel::NanoWheel(Widget *parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      fLeftMouseDown(false),
      fValue(0),
      fMin(0),
      fMax(0)
{
    loadSharedResources();

    setSize(size);
}

void NanoWheel::setRange(int min, int max) noexcept
{
    fMin = min;
    fMax = max;

    spoonie::clamp(fValue, min, max);
}

void NanoWheel::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoWheel::setValue(int value) noexcept
{
    fValue = spoonie::clamp(value, fMin, fMax);
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

bool NanoWheel::onMouse(const MouseEvent &ev)
{
    if (ev.button != 1)
        return fLeftMouseDown;

    if (!ev.press)
    {
        if (fLeftMouseDown == true)
        {
            fLeftMouseDown = false;
            return true;
        }

        return false;
    }

    if (contains(ev.pos))
    {
        fLeftMouseDownLocation = ev.pos;
        fLeftMouseDown = true;

        return true;
    }

    return false;
}

bool NanoWheel::onMotion(const MotionEvent &ev)
{
    if (fLeftMouseDown)
    {
        const int value = (fLeftMouseDownLocation.getY() - ev.pos.getY()) / 30;

        if (value != 0)
        {
            //getParentWindow().setCursorPos(this);
            fLeftMouseDownLocation.setY(ev.pos.getY());
            setValue(fValue + spoonie::clamp(value, -1, 1));
        }

        return true;
    }

    return false;
}

END_NAMESPACE_DISTRHO