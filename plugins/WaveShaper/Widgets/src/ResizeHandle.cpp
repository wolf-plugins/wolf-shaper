#include "ResizeHandle.hpp"
#include <cmath>

START_NAMESPACE_DISTRHO

ResizeHandle::ResizeHandle(Window &parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      fHasFocus(false)
{
    setSize(size);
}

ResizeHandle::ResizeHandle(NanoWidget *parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      fHasFocus(false)
{
    setSize(size);
}

void ResizeHandle::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void ResizeHandle::setMinSize(int minX, int minY)
{
    fMinSizeX = minX;
    fMinSizeY = minY;
}

void ResizeHandle::onNanoDisplay()
{
    beginPath();

    strokeWidth(2.0f);
    fillColor(Color(255, 0, 0, 255));
    strokeColor(Color(0, 0, 0, 255));

    rect(0, 0, getWidth(), getHeight());

    fill();
    stroke();

    closePath();
}

bool ResizeHandle::onMouse(const MouseEvent &ev)
{
    if (fHasFocus)
    {
        if (ev.button == 1 && !ev.press)
        {
            fHasFocus = false;
        }

        return true;
    }

    if (ev.button == 1 && contains(ev.pos))
    {
        fHasFocus = true;

        return true;
    }

    return false;
}

bool ResizeHandle::onMotion(const MotionEvent &ev)
{
    if (fHasFocus)
    {
        const int centerX = getWidth() / 2;
        const int translationX = ev.pos.getX() - centerX;

        const int centerY = getHeight() / 2;
        const int translationY = ev.pos.getY() - centerY;

        const int left = getAbsoluteX() + translationX;
        const int top = getAbsoluteY() + translationY;

        const int right = std::max<int>(left + getWidth(), fMinSizeX);
        const int bottom = std::max<int>(top + getHeight(), fMinSizeY);

        setAbsolutePos(right - getWidth(), bottom - getHeight());

        fCallback->resizeHandleMoved(right, bottom);

        return true;
    }

    return false;
}

END_NAMESPACE_DISTRHO