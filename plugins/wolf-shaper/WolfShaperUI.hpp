#ifndef WOLF_SHAPER_UI_HPP_INCLUDED
#define WOLF_SHAPER_UI_HPP_INCLUDED

#include "WolfShaperParameters.hpp"
#include "DistrhoUI.hpp"
#include "GraphWidget.hpp"
#include "RemoveDCSwitch.hpp"
#include "ResetGraphButton.hpp"
#include "OversampleWheel.hpp"
#include "VolumeKnob.hpp"
#include "ResizeHandle.hpp"
#include "LabelBox.hpp"
#include "BipolarModeSwitch.hpp"
#include "GlowingLabelsBox.hpp"
#include "NanoLabel.hpp"
#include "WidgetBar.hpp"
#include "ArrowButton.hpp"
#include "LabelBoxList.hpp"

START_NAMESPACE_DISTRHO

class WolfShaperUI : public UI,
                     public NanoSwitch::Callback,
                     public NanoButton::Callback,
                     public NanoWheel::Callback,
                     public NanoKnob::Callback,
                     public ResizeHandle::Callback
{
public:
  WolfShaperUI();
  ~WolfShaperUI();

  float getParameterValue(uint32_t index) const;

protected:
  void parameterChanged(uint32_t, float value) override;
  void tryRememberSize();
  void positionWidgets(uint width, uint height);

  void nanoSwitchClicked(NanoSwitch *nanoSwitch);
  void nanoButtonClicked(NanoButton *nanoButton);
  void nanoWheelValueChanged(NanoWheel *nanoWheel, const int value);
  void nanoKnobValueChanged(NanoKnob *nanoKnob, const float value);

  void resizeHandleMoved(int width, int height);

  void stateChanged(const char *key, const char *value) override;
  void onNanoDisplay() override;
  void uiIdle() override;
  void uiReshape(uint width, uint height) override;
  bool onKeyboard(const KeyboardEvent &ev) override;
  bool onMouse(const MouseEvent &ev) override;

private:
  void toggleBottomBarVisibility();

  ScopedPointer<RemoveDCSwitch> fSwitchRemoveDC;
  ScopedPointer<NanoLabel> fLabelRemoveDC;

  ScopedPointer<BipolarModeSwitch> fSwitchBipolarMode;
  ScopedPointer<GlowingLabelsBox> fLabelsBoxBipolarMode;

  ScopedPointer<OversampleWheel> fWheelOversample;
  ScopedPointer<NanoLabel> fLabelWheelOversample;

  ScopedPointer<VolumeKnob> fKnobPreGain, fKnobWet, fKnobPostGain;
  ScopedPointer<LabelBox> fLabelPreGain, fLabelWet, fLabelPostGain;

  ScopedPointer<VolumeKnob> fKnobHorizontalWarp;
  ScopedPointer<LabelBoxList> fLabelListHorizontalWarpType;

  ScopedPointer<VolumeKnob> fKnobVerticalWarp;
  ScopedPointer<LabelBoxList> fLabelListVerticalWarpType;

  ScopedPointer<ArrowButton> fButtonLeftArrowHorizontalWarp, fButtonRightArrowHorizontalWarp;
  ScopedPointer<ArrowButton> fButtonLeftArrowVerticalWarp, fButtonRightArrowVerticalWarp;

  ScopedPointer<ResizeHandle> fHandleResize;

  ScopedPointer<GraphWidget> fGraphWidget;
  ScopedPointer<WidgetBar> fGraphBar;
  ScopedPointer<ResetGraphButton> fButtonResetGraph;
  ScopedPointer<NanoLabel> fLabelButtonResetGraph;

  bool fBottomBarVisible;

  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WolfShaperUI)
};

END_NAMESPACE_DISTRHO

#endif