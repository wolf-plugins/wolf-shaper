#include "NanoSwitch.hpp"

NanoSwitch::NanoSwitch(Window& parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      fIsDown(false),
      fCallback(nullptr)
{
    setSize(size);
}

NanoSwitch::NanoSwitch(Widget* widget, Size<uint> size) noexcept
    : NanoWidget(widget->getParentWindow()),
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
    
void NanoSwitch::setCallback(Callback* callback) 
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