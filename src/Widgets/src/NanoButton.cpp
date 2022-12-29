#include "NanoButton.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

NanoButton::NanoButton(Widget *parent, Size<uint> size) noexcept
    : WolfWidget(parent),
      fState(kNanoStateNormal),
      fHasFocus(false),
      fIsHovered(false)
{
    setSize(size);
}

void NanoButton::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoButton::onNanoDisplay()
{
    draw();
}

NanoButton::ButtonState NanoButton::getButtonState()
{
    return fState;
}

void NanoButton::setButtonState(ButtonState state)
{
    fState = state;

    repaint();
}

bool NanoButton::leftClick(const MouseEvent &ev)
{
    bool hover = contains(ev.pos);

    if (!ev.press)
    {
        if (fHasFocus == true)
        {
            fHasFocus = false;

            if (hover)
            {
                setButtonState(kNanoStateHover);

                if (fCallback != nullptr)
                {
                    fCallback->nanoButtonClicked(this);
                }
            }
            else
            {
                setButtonState(kNanoStateNormal);
            }

            return true;
        }

        return false;
    }

    if (ev.press && hover)
    {
        setButtonState(kNanoStateDown);
        fHasFocus = true;

        return true;
    }

    return false;
}

bool NanoButton::middleClick(const MouseEvent &)
{
    return fHasFocus;
}

bool NanoButton::rightClick(const MouseEvent &)
{
    return fHasFocus;
}

bool NanoButton::onMouse(const MouseEvent &ev)
{
    switch (ev.button)
    {
    case kMouseButtonLeft:
        return leftClick(ev);
    case kMouseButtonMiddle:
        return middleClick(ev);
    case kMouseButtonRight:
        return rightClick(ev);
    }

    return false;
}

bool NanoButton::onMotion(const MotionEvent &ev)
{
    bool hover = contains(ev.pos);

    if (fHasFocus)
    {
        //should be drawn as down only when the mouse is on the widget
        if (hover)
        {
            setButtonState(kNanoStateHover);
            //            getParentWindow().setCursorStyle(Window::CursorStyle::Pointer);
        }
        else
        {
            setButtonState(kNanoStateHover);
            //            getParentWindow().setCursorStyle(Window::CursorStyle::Default);
        }

        return true;
    }

    if (hover)
    {
        if (fState == kNanoStateNormal)
        {
            setButtonState(kNanoStateHover);
            //            getParentWindow().setCursorStyle(Window::CursorStyle::Pointer);
        }

        return true;
    }
    else if (fState == kNanoStateHover)
    {
        setButtonState(kNanoStateNormal);
        //        getParentWindow().setCursorStyle(Window::CursorStyle::Default);

        return false;
    }

    return false;
}

END_NAMESPACE_DISTRHO
