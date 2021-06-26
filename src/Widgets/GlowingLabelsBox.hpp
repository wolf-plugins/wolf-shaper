#ifndef WOLF_GLOWING_LABELS_BOX_HPP_INCLUDED
#define WOLF_GLOWING_LABELS_BOX_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"
#include "LabelContainer.hpp"
#include <vector>

START_NAMESPACE_DISTRHO

class GlowingLabelsBox : public LabelContainer
{
  public:
    explicit GlowingLabelsBox(Widget  *widget, Size<uint> size) noexcept;

  protected:
    void onNanoDisplay() override;
};

END_NAMESPACE_DISTRHO

#endif