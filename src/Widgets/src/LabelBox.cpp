#include "LabelBox.hpp"
#include "Fonts/chivo_bold.hpp"
#include "Mathf.hpp"

//#include "Fonts/chivo_bold.hpp"

START_NAMESPACE_DISTRHO

LabelBox::LabelBox(Widget *widget, Size<uint> size) noexcept
    : WolfWidget(widget)
{
    setSize(size);

    using namespace WOLF_FONTS;
    createFontFromMemory("chivo_bold", (const uchar *)chivo_bold, chivo_bold_size, 0);
}

void LabelBox::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();
    const float scaleFactor = getScaleFactor();
    const float verticalMargin = 6.0f * scaleFactor;
    const float boxOutlineWidth = 2.0f * scaleFactor;

    //Box background
    beginPath();

    fillColor(Color(34, 34, 34, 255));
    strokeColor(Color(64, 64, 64, 255));
    strokeWidth(boxOutlineWidth);

    rect(0, 0, width, height);
    fill();
    stroke();

    closePath();

    //Shadow at top of box
    beginPath();

    strokeColor(0, 0, 0, 255);
    strokeWidth(boxOutlineWidth);

    moveTo(boxOutlineWidth, boxOutlineWidth);
    lineTo(width - boxOutlineWidth, boxOutlineWidth);
    stroke();

    closePath();

    //Text
    fontFace("chivo_bold");
    fontSize(16.0f * scaleFactor);
    fillColor(Color(255, 255, 255, 255));
    textAlign(ALIGN_CENTER | ALIGN_MIDDLE);

    text(std::round(width / 2.0f), std::round(height / 2.0f + verticalMargin / 2.0f - 2 * scaleFactor), fText, NULL);

    closePath();
}

void LabelBox::setFontSize(float fontSize)
{
    fFontSize = fontSize;
}

float LabelBox::getFontSize()
{
    return fFontSize;
}

void LabelBox::setText(const char *text)
{
    fText = text;
}

const char *LabelBox::getText()
{
    return fText;
}

void LabelBox::setFontId(NanoVG::FontId fontId)
{
    fFontId = fontId;
}

NanoVG::FontId LabelBox::getFontId()
{
    return fFontId;
}

END_NAMESPACE_DISTRHO
