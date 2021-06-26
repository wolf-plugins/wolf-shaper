#ifndef WOLF_NANO_WHEEL_HPP_INCLUDED
#define WOLF_NANO_WHEEL_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"
#include "WolfWidget.hpp"

START_NAMESPACE_DISTRHO

class NanoWheel : public WolfWidget
{
  public:
    class Callback
    {
      public:
        virtual ~Callback() {}
        virtual void nanoWheelValueChanged(NanoWheel *nanoWheel, int value) = 0;
    };

    explicit NanoWheel(Widget  *widget, Size<uint> size) noexcept;

    void setValue(int value, bool sendCallback = false) noexcept;
    int getValue() noexcept;
    void setRange(int min, int max) noexcept;

    void setCallback(Callback *callback) noexcept;

  protected:
    void onNanoDisplay() override;

    bool onScroll(const ScrollEvent &ev) override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &) override;

    virtual void draw() = 0;

  private:
    Callback *fCallback;

    bool fLeftMouseDown;
    Point<double> fLeftMouseDownLocation;

    bool fIsHovered;

    int fValue;
    int fMin;
    int fMax;

    DISTRHO_LEAK_DETECTOR(NanoWheel)
};

END_NAMESPACE_DISTRHO

#endif