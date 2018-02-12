#ifndef SPOONIE_REMOVE_DC_SWITCH_HPP_INCLUDED
#define SPOONIE_REMOVE_DC_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"

class RemoveDCSwitch : public NanoSwitch
{
public:
    explicit RemoveDCSwitch(Window& parent, Size<uint> size) noexcept;
    explicit RemoveDCSwitch(Widget* widget, Size<uint> size) noexcept;

protected:
    void drawUp() override;
    void drawDown() override;
    
private:
    DISTRHO_LEAK_DETECTOR(RemoveDCSwitch)
};

#endif