#ifndef SPOONIE_OVERSAMPLE_WHEEL_HPP_INCLUDED
#define SPOONIE_OVERSAMPLE_WHEEL_HPP_INCLUDED

#include "NanoWheel.hpp"

START_NAMESPACE_DISTRHO

class OversampleWheel : public NanoWheel
{
public:
    explicit OversampleWheel(Window &parent, Size<uint> size) noexcept;
    explicit OversampleWheel(NanoWidget *widget, Size<uint> size) noexcept;

protected:
    void draw() override;
    
private:
    DISTRHO_LEAK_DETECTOR(OversampleWheel)
};

END_NAMESPACE_DISTRHO

#endif