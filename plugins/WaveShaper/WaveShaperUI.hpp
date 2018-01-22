/*#ifndef WAVESHAPER_UI_GLBARS_HPP_INCLUDED
#define WAVESHAPER_UI_GLBARS_HPP_INCLUDED

#include "DistrhoUI.hpp"

START_NAMESPACE_DISTRHO

class WaveShaperUI : public UI
{
  public:
    WaveShaperUI();
    ~WaveShaperUI() override;

  protected:
    void parameterChanged(uint32_t, float value) override;
    void programLoaded(uint32_t index) override;
    void onDisplay() override;
    bool onMouse(const MouseEvent &ev) override;
};

END_NAMESPACE_DISTRHO

#endif*/