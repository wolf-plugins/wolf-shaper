#ifndef WOLF_VOLUME_KNOB_HPP_INCLUDED
#define WOLF_VOLUME_KNOB_HPP_INCLUDED

#include "NanoKnob.hpp"
#include "Animation.hpp"

START_NAMESPACE_DISTRHO

class VolumeKnob : public NanoKnob,
                   public IdleCallback
{
public:
  /* explicit VolumeKnob(Window &parent, Size<uint> size) noexcept; */
  explicit VolumeKnob(Widget  *widget, Size<uint> size) noexcept;

protected:
  void idleCallback() override;

  void onMouseHover() override;
  void onMouseLeave() override;
  void onMouseUp() override;
  void onMouseDown() override;

  void draw() override;

private:
  ScopedPointer<FloatTransition> fGrowAnimation;
  ScopedPointer<ColorTransition> fHoverAnimation;
  
  float fKnobDiameter;

  Color fKnobICol;
  Color fKnobOCol;

  DISTRHO_LEAK_DETECTOR(VolumeKnob)
};

END_NAMESPACE_DISTRHO

#endif