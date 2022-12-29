#include "ResizeHandle.hpp"
#include "Window.hpp"

#include <cmath>

START_NAMESPACE_DISTRHO

ResizeHandle::ResizeHandle(Widget *parent, Size<uint> size) noexcept
    : WolfWidget(parent),
      fHasFocus(false),
      fIsHovered(false),
      fMinSizeX(1),
      fMinSizeY(1)
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
    const float width = getWidth();
    const float height = getHeight();
    const float scaleFactor = getScaleFactor();
    const float lineMarginSides = 5.0f * getScaleFactor();

    beginPath();

    strokeWidth(1.0f * scaleFactor);
    strokeColor(Color(176, 176, 176, 255));

    const float linesBottom = height;

    moveTo(0, linesBottom);
    lineTo(width, 0);

    for (int i = 1; i < 5; ++i)
    {
        moveTo(lineMarginSides * i, linesBottom);
        lineTo(width + lineMarginSides * i, 0);
    }

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

    if (ev.button == 1 && ev.press && contains(ev.pos))
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

    if (contains(ev.pos))
    {
        if (!fIsHovered)
        {
            fIsHovered = true;
            //            getParentWindow().setCursorStyle(Window::CursorStyle::SouthEastResize);
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
