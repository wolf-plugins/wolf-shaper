#ifndef WOLF_GLOWING_LABELS_BOX_HPP_INCLUDED
#define WOLF_GLOWING_LABELS_BOX_HPP_INCLUDED

#include "LabelContainer.hpp"
#include "NanoVG.hpp"
#include "Widget.hpp"
#include <vector>

START_NAMESPACE_DISTRHO

class GlowingLabelsBox : public LabelContainer
{
public:
    explicit GlowingLabelsBox(Widget *widget, Size<uint> size) noexcept;

protected:
    void onNanoDisplay() override;
};

END_NAMESPACE_DISTRHO

#endif