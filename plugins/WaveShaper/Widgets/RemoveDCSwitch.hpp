#ifndef SPOONIE_REMOVE_DC_SWITCH_HPP_INCLUDED
#define SPOONIE_REMOVE_DC_SWITCH_HPP_INCLUDED

#include "NanoSwitch.hpp"

START_NAMESPACE_DISTRHO

class RemoveDCSwitch : public NanoSwitch
{
    
using NanoSwitch::NanoSwitch;

protected:
    void drawUp() override;
    void drawDown() override;
    
private:
    DISTRHO_LEAK_DETECTOR(RemoveDCSwitch)
};

END_NAMESPACE_DISTRHO

#endif