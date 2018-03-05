#ifndef SPOONIE_RESIZE_HANDLE_HPP_INCLUDED
#define SPOONIE_RESIZE_HANDLE_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class ResizeHandle : public NanoWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void resizeHandleMoved(int width, int height) = 0;
    };

    explicit ResizeHandle(Window& parent, Size<uint> size) noexcept;
    explicit ResizeHandle(NanoWidget* widget, Size<uint> size) noexcept;

    void setCallback(Callback* callback) noexcept;
    void setMinSize(int minX, int minY);

protected:
    void onNanoDisplay() override;

    bool onMouse(const MouseEvent&) override;
    bool onMotion(const MotionEvent&) override;

private:
    bool fHasFocus;
    Callback* fCallback;

    int fMinSizeX;
    int fMinSizeY;

    DISTRHO_LEAK_DETECTOR(ResizeHandle)
};

END_NAMESPACE_DISTRHO

#endif