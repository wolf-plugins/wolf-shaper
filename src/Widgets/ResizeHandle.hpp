#ifndef WOLF_RESIZE_HANDLE_HPP_INCLUDED
#define WOLF_RESIZE_HANDLE_HPP_INCLUDED

#include "WolfWidget.hpp"

START_NAMESPACE_DISTRHO

class ResizeHandle : public WolfWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void resizeHandleMoved(const int width, const int height) = 0;
    };

    explicit ResizeHandle(Widget *widget, Size<uint> size) noexcept;

    void setCallback(Callback *callback) noexcept;
    void setMinSize(int minX, int minY);

protected:
    void onNanoDisplay() override;

    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &) override;

private:
    bool fHasFocus;
    Callback *fCallback;

    bool fIsHovered;

    int fMinSizeX;
    int fMinSizeY;

    DISTRHO_LEAK_DETECTOR(ResizeHandle)
};

END_NAMESPACE_DISTRHO

#endif
