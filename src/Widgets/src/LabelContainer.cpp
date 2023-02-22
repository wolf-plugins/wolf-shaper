#include "LabelContainer.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

LabelContainer::LabelContainer(Widget *widget, Size<uint> size) noexcept
    : WolfWidget(widget),
      fSelectedIndex(0)
{
    setSize(size);
    loadSharedResources();
}

void LabelContainer::setFontSize(float fontSize)
{
    fFontSize = fontSize;
}

float LabelContainer::getFontSize()
{
    return fFontSize;
}

void LabelContainer::setSelectedIndex(int index)
{
    fSelectedIndex = wolf::clamp(index, 0, getLabelCount() - 1);
    repaint();
}

int LabelContainer::getSelectedIndex()
{
    return fSelectedIndex;
}

int LabelContainer::getLabelCount()
{
    return fLabels.size();
}

void LabelContainer::setLabels(std::vector<const char *> labels)
{
    fLabels = labels;
}

std::vector<const char *> LabelContainer::getLabels()
{
    return fLabels;
}

void LabelContainer::setFontId(NanoVG::FontId fontId)
{
    fFontId = fontId;
}

NanoVG::FontId LabelContainer::getFontId()
{
    return fFontId;
}

END_NAMESPACE_DISTRHO