#ifndef WAVESHAPER_UI_HPP_INCLUDED
#define WAVESHAPER_UI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "GraphWidget.hpp"

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

class WaveShaperUI : public UI
{
public:
  WaveShaperUI();
  ~WaveShaperUI();

protected:
  void parameterChanged(uint32_t, float value) override;
  void stateChanged(const char *key, const char *value) override;
  void onNanoDisplay() override;
  void uiIdle() override;

private:
  bool parameters[paramCount];

  GraphWidget graphWidget;

  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveShaperUI)
};

END_NAMESPACE_DISTRHO

#endif