#ifndef SPOONIE_NANO_WHEEL_HPP_INCLUDED
#define SPOONIE_NANO_WHEEL_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class NanoWheel : public NanoWidget
{
  public:
    class Callback
    {
      public:
        virtual ~Callback() {}
        virtual void nanoWheelValueChanged(NanoWheel *nanoWheel, int value) = 0;
    };

    explicit NanoWheel(Window &parent, Size<uint> size) noexcept;
    explicit NanoWheel(NanoWidget *widget, Size<uint> size) noexcept;

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
    Point<int> fLeftMouseDownLocation;

    int fValue;
    int fMin;
    int fMax;

    DISTRHO_LEAK_DETECTOR(NanoWheel)
};

END_NAMESPACE_DISTRHO

#endif