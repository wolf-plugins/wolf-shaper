#ifndef WOLF_LABEL_BOX_LIST_HPP_INCLUDED
#define WOLF_LABEL_BOX_LIST_HPP_INCLUDED

#include "LabelBox.hpp"
#include "LabelContainer.hpp"
#include "NanoVG.hpp"
#include "Widget.hpp"

START_NAMESPACE_DISTRHO

class LabelBoxList : public LabelContainer
{
public:
    explicit LabelBoxList(Widget *widget, Size<uint> size) noexcept;
    void goNext();
    void goPrevious();

protected:
    void onNanoDisplay() override;
    //void onShow() override;
    //void onHide() override;

private:
    LabelBox fLabelBox;

    DISTRHO_LEAK_DETECTOR(LabelBoxList)
};

END_NAMESPACE_DISTRHO

#endif