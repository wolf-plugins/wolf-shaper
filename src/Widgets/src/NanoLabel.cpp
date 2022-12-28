#include "NanoLabel.hpp"
#include "Fonts/chivo_bold.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

NanoLabel::NanoLabel(Widget *widget, Size<uint> size) noexcept
    : WolfWidget(widget),
      fText(""),
      fMargin(Margin(0, 0, 0, 0)),
      fAlign(ALIGN_LEFT | ALIGN_TOP),
      fColor(Color(255, 255, 255, 255)),
      fFontSize(12.0f * getScaleFactor())
{
    setSize(size);

    using namespace WOLF_FONTS;
    NanoVG::FontId chivoBoldId = createFontFromMemory("chivo_bold", (const uchar *)chivo_bold, chivo_bold_size, 0);

    fFontId = chivoBoldId;
}

void NanoLabel::onNanoDisplay()
{
    beginPath();

    fontFaceId(fFontId);
    fontSize(fFontSize);
    fillColor(fColor);
    textAlign(fAlign);

    text(fMargin.left, fMargin.top, fText, NULL);

    closePath();
}

void NanoLabel::setColor(Color color)
{
    fColor = color;
}

void NanoLabel::setFontSize(float fontSize)
{
    fFontSize = fontSize;
}

void NanoLabel::setText(const char *text)
{
    fText = text;
}

void NanoLabel::setAlign(int align)
{
    fAlign = align;
}

void NanoLabel::setMargin(Margin margin)
{
    fMargin = margin;
}

void NanoLabel::setFontId(NanoVG::FontId fontId)
{
    fFontId = fontId;
}

END_NAMESPACE_DISTRHO
