#include "GlowingLabelsBox.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

GlowingLabelsBox::GlowingLabelsBox(Widget *widget, Size<uint> size) noexcept
    : LabelContainer(widget, size)
{
}

void GlowingLabelsBox::onNanoDisplay()
{
    if (getLabelCount() == 0)
        return;

    const float width = getWidth();
    const float height = getHeight();
    const float scaleFactor = getScaleFactor();

    //Box background
    beginPath();

    fillColor(Color(17, 17, 17, 255));

    rect(0, 0, width, height);
    fill();

    closePath();

    const float labelSize = 14.0f * scaleFactor;
    const float labelMarginLeft = 4.0f * scaleFactor;
    const float labelMarginTop = 4.0f * scaleFactor;
    const float labelVerticalSpacing = labelSize + 7.0f * scaleFactor;

    //Text
    for (int i = 0; i < getLabelCount(); ++i)
    {
        beginPath();

        fontFace(NANOVG_DEJAVU_SANS_TTF);
        fontSize(labelSize);

        const Color fontColor = (i == getSelectedIndex()) ? Color(235, 196, 74, 255) : Color(158, 158, 158, 255);

        fillColor(fontColor);
        textAlign(ALIGN_LEFT | ALIGN_TOP);

        text(labelMarginLeft, labelVerticalSpacing * i + labelMarginTop, getLabels()[i], NULL);

        closePath();
    }
}

END_NAMESPACE_DISTRHO
