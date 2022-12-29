#include "OversampleWheel.hpp"
#include "Fonts/chivo_bold.hpp"

START_NAMESPACE_DISTRHO

//TODO: Put these in the config file
// NOTE: underscore names so we know these vars need scaleFactor multiplication
const float _trianglesVerticalMargin = 5.0f;
const float _trianglesHorizontalMargin = 4.0f;
const float _trianglesWidth = 8.0f;
const float _trianglesHeight = 6.0f;
const float _textMarginRight = 3.0f;
const float _outlineWidth = 2.0f;

OversampleWheel::OversampleWheel(Widget *widget, Size<uint> size) noexcept
    : NanoWheel(widget, size),
      fFontSize(18.0f * getScaleFactor())
{
    using namespace WOLF_FONTS;
    createFontFromMemory("chivo_bold", (const uchar *)chivo_bold, chivo_bold_size, 0);
}

void OversampleWheel::drawTriangles()
{
    const float width = getWidth();
    const float height = getHeight();
    const float scaleFactor = getScaleFactor();

    const float trianglesVerticalMargin = _trianglesVerticalMargin * scaleFactor;
    const float trianglesHorizontalMargin = _trianglesHorizontalMargin * scaleFactor;
    const float trianglesWidth = _trianglesWidth * scaleFactor;
    const float trianglesHeight = _trianglesHeight * scaleFactor;

    const float rightPointXLocation = width - trianglesHorizontalMargin;
    const float leftPointXLocation = rightPointXLocation - trianglesWidth;
    const float middlePointXLocation = rightPointXLocation - trianglesWidth / 2.0f;

    //top triangle
    const float topTriangleBottomY = trianglesVerticalMargin + trianglesHeight;

    beginPath();

    moveTo(middlePointXLocation, trianglesVerticalMargin);

    lineTo(leftPointXLocation, topTriangleBottomY);
    lineTo(rightPointXLocation, topTriangleBottomY);
    lineTo(middlePointXLocation, trianglesVerticalMargin);

    fillColor(Color(255, 255, 255, 255));
    strokeColor(Color(0, 0, 0, 255));
    strokeWidth(1.0f * scaleFactor);

    fill();
    stroke();

    closePath();

    //bottom triangle
    beginPath();

    const float bottomTriangleBottomY = height - trianglesVerticalMargin;
    const float bottomTriangleTopY = bottomTriangleBottomY - trianglesHeight;

    moveTo(middlePointXLocation, bottomTriangleBottomY);

    lineTo(leftPointXLocation, bottomTriangleTopY);
    lineTo(rightPointXLocation, bottomTriangleTopY);
    lineTo(middlePointXLocation, bottomTriangleBottomY);

    fillColor(Color(255, 255, 255, 255));
    strokeColor(Color(0, 0, 0, 255));

    fill();
    stroke();

    closePath();
}

void OversampleWheel::drawBackground()
{
    const float width = getWidth();
    const float height = getHeight();
    const float scaleFactor = getScaleFactor();

    const float outlineWidth = _outlineWidth * scaleFactor;

    beginPath();

    Paint backgroundGradient = linearGradient(outlineWidth, outlineWidth, outlineWidth, height, Color(54, 52, 88, 255), Color(38, 37, 51, 255));

    fillPaint(backgroundGradient);
    rect(outlineWidth, outlineWidth, width - outlineWidth * 2.0f, height - outlineWidth * 2.0f);
    fill();

    closePath();
}

void OversampleWheel::drawText()
{
    const float width = getWidth();
    const float height = getHeight();
    const float scaleFactor = getScaleFactor();

    const float trianglesHorizontalMargin = _trianglesHorizontalMargin * scaleFactor;
    const float trianglesWidth = _trianglesWidth * scaleFactor;
    const float textMarginRight = _textMarginRight * scaleFactor;

    const char *oversamplingFactors[] = {"", "2x", "4x", "8x", "16x"};

    const int value = getValue();

    if (value > 0)
    {
        beginPath();

        fontFace("chivo_bold");
        fontSize(fFontSize);
        fillColor(Color(193, 219, 240, 255));
        textAlign(ALIGN_RIGHT | ALIGN_CENTER);

        text(std::round(width - trianglesWidth - trianglesHorizontalMargin - textMarginRight), std::round(height / 1.45f), oversamplingFactors[getValue()], NULL);

        fontBlur(5.0f);
        fillColor(Color(255, 255, 255, 0.4f));

        text(std::round(width - trianglesWidth - trianglesHorizontalMargin - textMarginRight), std::round(height / 1.45f), oversamplingFactors[getValue()], NULL);

        closePath();
    }
}

void OversampleWheel::drawOutline()
{
    const float width = getWidth();
    const float height = getHeight();

    beginPath();

    fillColor(Color(27, 27, 27, 255));
    roundedRect(0, 0, width, height, 5.0f);
    fill();

    closePath();
}

void OversampleWheel::draw()
{
    const float width = getWidth();
    const float scaleFactor = getScaleFactor();

    const float outlineWidth = _outlineWidth * scaleFactor;

    //outline
    drawOutline();

    //background
    drawBackground();

    //line at top of display
    beginPath();

    const float widthLineTopDisplay = 1.4f * scaleFactor;

    strokeColor(Color(72, 137, 208, 150));
    strokeWidth(widthLineTopDisplay);

    moveTo(outlineWidth, outlineWidth);
    lineTo(width - outlineWidth, outlineWidth);

    stroke();

    closePath();

    //reflection at bottom of wheel
    /*beginPath();

    Paint wheelShadow = linearGradient(width / 2.0f, height - outlineWidth, width / 2.0f, height, Color(0,0,0,255), Color(0,0,0,0));
    rect(0, height - outlineWidth, width, height);

    fillPaint(wheelShadow);

    fill(); 

    closePath();*/

    //triangles
    drawTriangles();

    //text
    drawText();
}

END_NAMESPACE_DISTRHO
