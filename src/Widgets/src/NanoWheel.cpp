#include "NanoWheel.hpp"
#include "Mathf.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

NanoWheel::NanoWheel(Widget *parent, Size<uint> size) noexcept
    : WolfWidget(parent),
      fLeftMouseDown(false),
      fLeftMouseDownLocation(Point<double>(0, 0)),
      fIsHovered(true),
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

    fValue = wolf::clamp(fValue, min, max);
}

void NanoWheel::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoWheel::setValue(int value, bool sendCallback) noexcept
{
    value = wolf::clamp(value, fMin, fMax);

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
    if (ev.button != kMouseButtonLeft)
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
    if (!canBeFocused())
        return false;

    if (fLeftMouseDown)
    {
        const float tension = 20.0f;

        const int value = (fLeftMouseDownLocation.getY() - ev.pos.getY()) / tension;

        if (value != 0)
        {
            fLeftMouseDownLocation.setY(ev.pos.getY());

            Window &window = getWindow();
            const int windowHeight = window.getHeight();

            if (ev.pos.getY() + getAbsoluteY() >= windowHeight - 1)
            {
                fLeftMouseDownLocation.setY(-getAbsoluteY() + 2);
            }
            else if (ev.pos.getY() + getAbsoluteY() == 0)
            {
                fLeftMouseDownLocation.setY(windowHeight - getAbsoluteY() - 2);
            }
            else
            {
                fLeftMouseDownLocation.setY(ev.pos.getY());
            }

            setValue(fValue + wolf::clamp(value, -1, 1), true);
        }

        return true;
    }

    if (contains(ev.pos))
    {
        if (!fIsHovered)
        {
            fIsHovered = true;
            //            getParentWindow().setCursorStyle(Window::CursorStyle::UpDown);
        }

        return true;
    }
    else if (fIsHovered)
    {
        fIsHovered = false;
        //        getParentWindow().setCursorStyle(Window::CursorStyle::Default);
    }

    return false;
}

END_NAMESPACE_DISTRHO
