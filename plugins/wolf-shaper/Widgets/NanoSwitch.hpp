#ifndef WOLF_NANO_SWITCH_HPP_INCLUDED
#define WOLF_NANO_SWITCH_HPP_INCLUDED

#include "WolfWidget.hpp"

START_NAMESPACE_DISTRHO

class NanoSwitch : public WolfWidget
{
  public:
    class Callback
    {
      public:
        virtual ~Callback() {}
        virtual void nanoSwitchClicked(NanoSwitch *nanoSwitch) = 0;
    };

    explicit NanoSwitch(Widget  *widget, Size<uint> size) noexcept;

    bool isDown() const noexcept;
    void setDown(bool down) noexcept;

    void setCallback(Callback *callback) noexcept;

  protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &ev) override;

    virtual void draw() = 0;

    virtual void onStateChanged();

  private:
    bool fIsDown;
    bool fIsHovered;
    
    Callback *fCallback;

    DISTRHO_LEAK_DETECTOR(NanoSwitch)
};

END_NAMESPACE_DISTRHO

#endif