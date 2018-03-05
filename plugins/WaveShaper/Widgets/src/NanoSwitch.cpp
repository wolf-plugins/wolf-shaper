#include "NanoSwitch.hpp"

START_NAMESPACE_DISTRHO

NanoSwitch::NanoSwitch(Window& parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      fIsDown(false),
      fCallback(nullptr)
{
    setSize(size);
}

NanoSwitch::NanoSwitch(NanoWidget* widget, Size<uint> size) noexcept
    : NanoWidget(widget),
      fIsDown(false),
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
    repaint();
}

void NanoSwitch::onNanoDisplay()
{
    if (fIsDown)
        drawDown();
    else
        drawUp();
}
    
void NanoSwitch::setCallback(Callback* callback) noexcept
{
    fCallback = callback;
}

bool NanoSwitch::onMouse(const MouseEvent& ev)
{
    if (ev.press && contains(ev.pos))
    {
        fIsDown = !fIsDown;

        repaint();

        if (fCallback != nullptr)
            fCallback->nanoSwitchClicked(this);

        return true;
    }

    return false;
}

END_NAMESPACE_DISTRHO