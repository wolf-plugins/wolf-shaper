#include "LabelBoxList.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

LabelBoxList::LabelBoxList(Widget  *widget, Size<uint> size) noexcept : LabelContainer(widget, size),
                                                                           fLabelBox(widget, size)
{
    setSize(size);
}

void LabelBoxList::onNanoDisplay()
{
    fLabelBox.setText(getLabels()[getSelectedIndex()]);
    fLabelBox.setAbsolutePos(getAbsolutePos());
}

void LabelBoxList::goNext()
{
    const int index = wolf::modulo(getSelectedIndex() + 1, getLabelCount());

    setSelectedIndex(index);
}

void LabelBoxList::goPrevious()
{
    const int index = wolf::modulo(getSelectedIndex() - 1, getLabelCount());

    setSelectedIndex(index);
}

/* void LabelBoxList::onShow()
{
    fLabelBox.setVisible(true);
}

void LabelBoxList::onHide()
{
    fLabelBox.setVisible(false);
} */

END_NAMESPACE_DISTRHO