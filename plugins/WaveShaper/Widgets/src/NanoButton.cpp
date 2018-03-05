#include "NanoButton.hpp"

START_NAMESPACE_DISTRHO

NanoButton::NanoButton(Window &parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      state(kNanoStateNormal),
      fHasFocus(false)
{
    setSize(size);
}

NanoButton::NanoButton(NanoWidget *parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      state(kNanoStateNormal),
      fHasFocus(false)
{
    setSize(size);
}

void NanoButton::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void NanoButton::onNanoDisplay()
{
    switch (state)
    {
    case kNanoStateDown:
        drawDown();
        break;
    case kNanoStateHover:
        drawHover();
        break;
    default:
        drawUp();
        break;
    }
}

void NanoButton::setButtonState(State state)
{
    this->state = state;

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

bool NanoButton::middleClick(const MouseEvent &ev)
{
    return fHasFocus;
}

bool NanoButton::rightClick(const MouseEvent &ev)
{
    return fHasFocus;
}

bool NanoButton::onMouse(const MouseEvent &ev)
{
    switch (ev.button)
    {
    case 1:
        return leftClick(ev);
    case 2:
        return middleClick(ev);
    case 3:
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
            setButtonState(kNanoStateDown);
        else
            setButtonState(kNanoStateHover);

        return true;
    }

    if (contains(ev.pos))
    {
        if (state == kNanoStateNormal)
        {
            setButtonState(kNanoStateHover);
        }

        return true;
    }
    else if (state == kNanoStateHover)
    {
        setButtonState(kNanoStateNormal);

        return false;
    }

    return false;
}

END_NAMESPACE_DISTRHO