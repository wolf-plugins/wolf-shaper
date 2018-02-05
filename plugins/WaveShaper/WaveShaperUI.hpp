#ifndef WAVESHAPER_UI_HPP_INCLUDED
#define WAVESHAPER_UI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "GraphWidgetSocket.hpp"
#include "ImageWidgets.hpp"
#include "DistrhoArtworkNekobi.hpp"

START_NAMESPACE_DISTRHO

enum Parameters
{
  paramPreGain = 0,
  paramWet,
  paramPostGain,
  paramRemoveDC,
  paramOversample,
  paramBipolarMode,
  paramCount
};

class WaveShaperUI : public UI,
                     public ImageButton::Callback,
                     public ImageKnob::Callback

{
public:
  WaveShaperUI();
  ~WaveShaperUI();

protected:
  void parameterChanged(uint32_t, float value) override;
  void stateChanged(const char *key, const char *value) override;
  void onNanoDisplay() override;
  void uiIdle() override;

  void imageButtonClicked(ImageButton *button, int) override;
  void imageKnobDragStarted(ImageKnob *knob) override;
  void imageKnobDragFinished(ImageKnob *knob) override;
  void imageKnobValueChanged(ImageKnob *knob, float value) override;

private:
  bool parameters[paramCount];

  //ScopedPointer<ImageButton> fButtonRemoveDC;
  ScopedPointer<ImageKnob> fKnobPre, fKnobDryWet, fKnobPost;

  GraphWidgetSocket graphWidgetSocket;

  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveShaperUI)
};

END_NAMESPACE_DISTRHO

#endif