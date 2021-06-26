#include "NanoSwitch.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

NanoSwitch::NanoSwitch(Widget  *widget, Size<uint> size) noexcept
    : WolfWidget(widget),
      fIsDown(false),
      fIsHovered(false),
      fCallback(nullptr)
{
    setSize(size);
}

bool NanoSwitch::isDown() const noexcept
{
    return fIsDown;
}

void NanoSwitch::setDown(bool down) noexcept
{
    if (fIsDown == down)
        return;

    fIsDown = down;

    onStateChanged();

    repaint();
}

void NanoSwitch::onNanoDisplay()
{
    draw();
}

void NanoSwitch::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoSwitch::onStateChanged()
{
}

bool NanoSwitch::onMouse(const MouseEvent &ev)
{
    if (ev.press && contains(ev.pos))
    {
        setDown(!fIsDown);

        if (fCallback != nullptr)
            fCallback->nanoSwitchClicked(this);

        return true;
    }

    return false;
}

bool NanoSwitch::onMotion(const MotionEvent &ev)
{
    if (!canBeFocused())
        return false;

    if (contains(ev.pos))
    {
        if (!fIsHovered)
        {
            fIsHovered = true;
//            getParentWindow().setCursorStyle(Window::CursorStyle::Pointer);
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
