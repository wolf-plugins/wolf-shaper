#ifndef SPOONIE_NANO_KNOB_HPP_INCLUDED
#define SPOONIE_NANO_KNOB_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

class NanoKnob : public NanoWidget
{
public:
  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void nanoKnobValueChanged(NanoKnob *nanoKnob, const float value) = 0;
  };

  explicit NanoKnob(Window &parent, Size<uint> size) noexcept;
  explicit NanoKnob(NanoWidget *widget, Size<uint> size) noexcept;

  float getValue() const noexcept;
  void setValue(float value, bool sendCallback = false) noexcept;

  void setDefault(float def) noexcept;
  void setRange(float min, float max) noexcept;
  void setStep(float step) noexcept;
  void setUsingLogScale(bool yesNo) noexcept;
  void setCallback(Callback *callback) noexcept;
  void setColor(Color color) noexcept;

protected:
  void onNanoDisplay() override;
  bool onMouse(const MouseEvent &) override;
  bool onMotion(const MotionEvent &) override;
  bool onScroll(const ScrollEvent &) override;

  Color getColor() noexcept;
  float getMin() noexcept;
  float getMax() noexcept;

  virtual void drawNormal() = 0;
  virtual void drawHover() = 0;
  virtual void drawTurning() = 0;

private:
  float fMin;
  float fMax;
  float fStep;
  float fValue;
  bool fUsingLog;

  bool fLeftMouseDown;
  Point<int> fLeftMouseDownLocation;
  
  int fRotationAngle;
  bool fDragging;
  int fLastX;
  int fLastY;

  Color fColor;

  Callback *fCallback;

  void setRotationAngle(const int angle);

  DISTRHO_LEAK_DETECTOR(NanoKnob)
};

END_NAMESPACE_DISTRHO

#endif