#ifndef SPOONIE_VOLUME_KNOB_HPP_INCLUDED
#define SPOONIE_VOLUME_KNOB_HPP_INCLUDED

#include "NanoKnob.hpp"

START_NAMESPACE_DISTRHO

class VolumeKnob : public NanoKnob
{
public:
    explicit VolumeKnob(Window &parent, Size<uint> size) noexcept;
    explicit VolumeKnob(NanoWidget *widget, Size<uint> size) noexcept;

protected:
    void drawNormal() override;
    void drawHover() override;
    void drawTurning() override;

private:
    DISTRHO_LEAK_DETECTOR(VolumeKnob)
};

END_NAMESPACE_DISTRHO

#endif