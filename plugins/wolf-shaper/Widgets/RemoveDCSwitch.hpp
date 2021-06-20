#ifndef WOLF_REMOVE_DC_SWITCH_HPP_INCLUDED
#define WOLF_REMOVE_DC_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"
#include "Animation.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

class RemoveDCSwitch : public NanoSwitch,
                       public IdleCallback
{
  public:
    explicit RemoveDCSwitch(Widget  *widget, Size<uint> size) noexcept;

  protected:
    void draw() override;
    void idleCallback() override;
    void onStateChanged() override;

  private:
    Color fSocketColor;
    ColorTransition fSocketColorTransition;

    Color fGlowIcol;
    ColorTransition fGlowIcolTransition;

    Color fMainRectColor;
    ColorTransition fMainRectColorTransition;

    NanoVG::Paint fMainRectGradient;
    GradientTransition fMainRectGradientTransition;

    DISTRHO_LEAK_DETECTOR(RemoveDCSwitch)
};

END_NAMESPACE_DISTRHO

#endif