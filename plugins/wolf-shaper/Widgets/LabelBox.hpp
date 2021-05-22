#ifndef WOLF_LABEL_BOX_HPP_INCLUDED
#define WOLF_LABEL_BOX_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"
#include "WolfWidget.hpp"

START_NAMESPACE_DISTRHO

class LabelBox : public WolfWidget
{
  public:
    explicit LabelBox(Widget  *widget, Size<uint> size) noexcept;

    void setFontSize(float fontSize);
    float getFontSize();

    void setText(const char *text);
    const char *getText();

    void setFontId(NanoVG::FontId fontId);
    NanoVG::FontId getFontId();

  protected:
    void onNanoDisplay() override;

  private:
    const char *fText;
    float fFontSize;
    NanoVG::FontId fFontId;

    DISTRHO_LEAK_DETECTOR(LabelBox)
};

END_NAMESPACE_DISTRHO

#endif