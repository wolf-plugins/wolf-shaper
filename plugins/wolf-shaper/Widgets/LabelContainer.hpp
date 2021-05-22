#ifndef WOLF_LABEL_CONTAINER_HPP_INCLUDED
#define WOLF_LABEL_CONTAINER_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"
#include "WolfWidget.hpp"
#include <vector>

START_NAMESPACE_DISTRHO

class LabelContainer : public WolfWidget
{
public:
    void setSelectedIndex(int index);
    int getSelectedIndex();

    int getLabelCount();
    
    void setLabels(std::vector<const char*>labels);
    std::vector<const char*> getLabels();

    void setFontSize(float fontSize);
    float getFontSize();

    void setFontId(NanoVG::FontId fontId);
    NanoVG::FontId getFontId();

protected:
    explicit LabelContainer(Widget  *widget, Size<uint> size) noexcept;

private:
    int fSelectedIndex;

    std::vector<const char*> fLabels;

    float fFontSize;
    NanoVG::FontId fFontId;

    DISTRHO_LEAK_DETECTOR(LabelContainer)
};

END_NAMESPACE_DISTRHO

#endif