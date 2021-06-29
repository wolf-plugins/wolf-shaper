#ifndef WOLF_BIPOLAR_MODE_SWITCH_HPP_INCLUDED
#define WOLF_BIPOLAR_MODE_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"

START_NAMESPACE_DISTRHO

class BipolarModeSwitch : public NanoSwitch
{
public:
    explicit BipolarModeSwitch(Widget  *widget, Size<uint> size) noexcept;

protected:
    void drawHandle();
    void drawSocket();

    void draw() override;
    
private:
    DISTRHO_LEAK_DETECTOR(BipolarModeSwitch)
};

END_NAMESPACE_DISTRHO

#endif