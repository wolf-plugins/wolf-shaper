#include "NanoButton.hpp"

START_NAMESPACE_DISTRHO

NanoButton::NanoButton(Window &parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      state(kNanoStateNormal)
{
    setSize(size);
}

NanoButton::NanoButton(Widget *parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      state(kNanoStateNormal)
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

bool NanoButton::onMouse(const MouseEvent &ev)
{
    if (!contains(ev.pos))
    {
        if (state != kNanoStateNormal)
        {
            setButtonState(kNanoStateNormal);
        }

        return false;
    }

    if (ev.press)
    {
        setButtonState(kNanoStateDown);
    }
    else
    {
        setButtonState(kNanoStateHover);

        if (fCallback != nullptr)
        {
            fCallback->nanoButtonClicked(this);
        }
    }

    return true;
}

bool NanoButton::onMotion(const MotionEvent &ev)
{
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