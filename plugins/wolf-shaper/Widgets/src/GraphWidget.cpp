#include "Widget.hpp"
#include "Window.hpp"

#include "Application.hpp"
#include "Config.hpp"
#include "Graph.hpp"
#include "GraphWidget.hpp"
#include "Mathf.hpp"

#include "Fonts/chivo_italic.hpp"

#include <chrono>
#include <cmath>
#include <cstdlib>

START_NAMESPACE_DISTRHO

static const char* const graphDefaultState = "0x0p+0,0x0p+0,0x0p+0,0;0x1p+0,0x1p+0,0x0p+0,0;";

GraphWidget::GraphWidget(UI* ui, Size<uint> size)
    : WolfWidget(ui),
      fMargin(16, 16, 16, 16),
      ui(ui),
      mouseLeftDown(false),
      mouseRightDown(false),
      graphGradientMode(GraphGradientMode::None),
      fInput(0.0f),
      fLastCurveTypeSelected(wolf::SingleCurve)
{
    setSize(size);

    rebuildFromString(graphDefaultState);

    getApp().addIdleCallback(this);

    using namespace WOLF_FONTS;
    createFontFromMemory("chivo_italic", (const uchar*)chivo_italic, chivo_italic_size, false);
}

GraphWidget::~GraphWidget() = default;

void GraphWidget::onResize(const ResizeEvent& ev)
{
}

void GraphWidget::reset()
{
    ui->setState("graph", graphDefaultState);
    rebuildFromString(graphDefaultState);
}

void GraphWidget::rebuildFromString(const char* serializedGraph)
{
    lineEditor.rebuildFromString(serializedGraph);
}

void GraphWidget::flipYAxis()
{
    transform(1.0f, 0.0f, 0.0f, -1.0f, 0.0f, getHeight());
}

void GraphWidget::drawGrid()
{
    const float width = getWidth();
    const float height = getHeight();

    const float lineWidth = 1.0f;

    const int squaresPerRow = 8.0f;
    const int gridMiddleLineIndex = squaresPerRow / 2.0f;

    const float verticalStep = width / squaresPerRow;
    const float horizontalStep = height / squaresPerRow;

    const Color gridForegroundColor = CONFIG_NAMESPACE::grid_foreground;
    const Color gridBackgroundColor = CONFIG_NAMESPACE::grid_background;
    const Color subGridColor = CONFIG_NAMESPACE::sub_grid;
    const Color gridMiddleLineHorizontalColor = CONFIG_NAMESPACE::grid_middle_line_horizontal;
    const Color gridMiddleLineVerticalColor = CONFIG_NAMESPACE::grid_middle_line_vertical;

    //vertical
    for (int i = 0; i < squaresPerRow + 1; ++i)
    {
        const float posX = std::round(i * verticalStep);

        //subgrid
        beginPath();
        strokeWidth(lineWidth);
        strokeColor(subGridColor);

        moveTo(std::round(posX + verticalStep / 2.0f), 0.0f);
        lineTo(std::round(posX + verticalStep / 2.0f), height);

        stroke();
        closePath();

        //background
        beginPath();
        strokeWidth(lineWidth);
        strokeColor(gridBackgroundColor);

        moveTo(posX + lineWidth, 0.0f);
        lineTo(posX + lineWidth, height);

        stroke();
        closePath();

        //foreground
        beginPath();
        strokeWidth(lineWidth);
        strokeColor(i == gridMiddleLineIndex ? gridMiddleLineVerticalColor : gridForegroundColor);

        moveTo(posX, 0.0f);
        lineTo(posX, height);

        stroke();
        closePath();
    }

    //horizontal
    for (int i = 0; i < squaresPerRow + 1; ++i)
    {
        const float posY = std::round(i * horizontalStep);

        //subgrid
        beginPath();
        strokeWidth(lineWidth);
        strokeColor(subGridColor);

        moveTo(0.0f, std::round(posY + horizontalStep / 2.0f));
        lineTo(width, std::round(posY + horizontalStep / 2.0f));

        stroke();
        closePath();

        //background
        beginPath();
        strokeWidth(lineWidth);

        moveTo(0.0f, posY + lineWidth);
        lineTo(width, posY + lineWidth);

        strokeColor(gridBackgroundColor);

        stroke();
        closePath();

        //foreground
        beginPath();
        strokeWidth(lineWidth);

        moveTo(0.0f, posY);
        lineTo(width, posY);

        strokeColor(i == gridMiddleLineIndex ? gridMiddleLineHorizontalColor : gridForegroundColor);

        stroke();
        closePath();
    }
}

void GraphWidget::drawBackground()
{
    const float width = getWidth();
    const float height = getHeight();

    beginPath();

    rect(0.f, 0.f, width, height);
    fillColor(CONFIG_NAMESPACE::graph_background);
    fill();

    closePath();
}

bool GraphWidget::edgeMustBeEmphasized(int vertexIndex)
{
    // TODO: check if focusedElement is tension handle
    return false;
}

void GraphWidget::drawGraphEdge(int vertexIndex, float lineWidth, Color color)
{
    DISTRHO_SAFE_ASSERT(vertexIndex < lineEditor.getVertexCount() - 1);

    const float width = getWidth();
    const float height = getHeight();

    wolf::Vertex* leftVertex = lineEditor.getVertexAtIndex(vertexIndex);
    wolf::Vertex* rightVertex = lineEditor.getVertexAtIndex(vertexIndex + 1);

    beginPath();

    strokeColor(color);
    strokeWidth(lineWidth);
    lineJoin(ROUND);

    moveTo(leftVertex->getX() * width, leftVertex->getY() * height);

    const float edgeLength = (rightVertex->getX() - leftVertex->getX()) * width;

    for (int i = 0; i <= edgeLength; ++i)
    {
        const float normalizedX = leftVertex->getX() + i / width;

        lineTo(normalizedX * width, lineEditor.getValueAt(normalizedX) * height);
    }

    lineTo(rightVertex->getX() * width, rightVertex->getY() * height);

    stroke();

    closePath();
}

void GraphWidget::drawGraphLine(float lineWidth, Color normalColor, Color emphasizedColor)
{
    for (int i = 0; i < lineEditor.getVertexCount() - 1; ++i)
    {
        const Color color = edgeMustBeEmphasized(i) ? emphasizedColor : normalColor;

        drawGraphEdge(i, lineWidth, color);
    }
}

void GraphWidget::drawAlignmentLines()
{
    /*
    const int x = focusedElement->getX();
    const int y = focusedElement->getY();
    const int width = getWidth();
    const int height = getHeight();

    translate(0.5f, 0.5f);

    beginPath();

    strokeWidth(1.0f);
    strokeColor(CONFIG_NAMESPACE::alignment_lines);

    moveTo(x, 0);
    lineTo(x, height);

    moveTo(0, y);
    lineTo(width, y);

    stroke();

    closePath();

    translate(-0.5f, -0.5f); */
}

void GraphWidget::drawGradient()
{
    if (graphGradientMode == GraphGradientMode::None)
        return;

    const float width = getWidth();
    const float height = getHeight();

    float peak = 0.0f;

    beginPath();

    moveTo(0, lineEditor.getVertexAtIndex(0)->getY() * height);

    for (int vertexIndex = 0; vertexIndex < lineEditor.getVertexCount() - 1; ++vertexIndex)
    {
        wolf::Vertex* leftVertex = lineEditor.getVertexAtIndex(vertexIndex);
        wolf::Vertex* rightVertex = lineEditor.getVertexAtIndex(vertexIndex + 1);

        const float edgeLength = (rightVertex->getX() - leftVertex->getX()) * width;

        for (int i = 0; i <= edgeLength; ++i)
        {
            const float normalizedX = leftVertex->getX() + i / width;
            const float x = normalizedX * width;
            const float y = lineEditor.getValueAt(normalizedX) * height;

            lineTo(x, y);

            peak = std::max(peak, y);
        }
    }

    lineTo(width, 0);
    lineTo(0, 0);

    fillPaint(linearGradient(width / 2.0f, 0, width / 2.0f, peak, CONFIG_NAMESPACE::graph_gradient_icol, CONFIG_NAMESPACE::graph_gradient_ocol));
    fill();

    closePath();
}

void GraphWidget::updateInput(const float input)
{
    fInput = input;
}

void GraphWidget::setGraphGradientMode(GraphGradientMode graphGradientMode)
{
    this->graphGradientMode = graphGradientMode;
    repaint();
}

void GraphWidget::setHorizontalWarpAmount(const float warpAmount)
{
    lineEditor.setHorizontalWarpAmount(warpAmount);
}

void GraphWidget::setHorizontalWarpType(const wolf::WarpType warpType)
{
    lineEditor.setHorizontalWarpType(warpType);
}

void GraphWidget::setVerticalWarpAmount(const float warpAmount)
{
    lineEditor.setVerticalWarpAmount(warpAmount);
}

void GraphWidget::setVerticalWarpType(const wolf::WarpType warpType)
{
    lineEditor.setVerticalWarpType(warpType);
}

void GraphWidget::drawInputIndicator()
{
    const auto width = static_cast<float>(getWidth());
    const auto height = static_cast<float>(getHeight());

    if (fInput <= 0.0f)
    {
        return;
    }

    const float inputIndicatorX = std::round(fInput * width);

    translate(0.5f, 0.5f);

    beginPath();

    strokeColor(CONFIG_NAMESPACE::input_volume_indicator);
    strokeWidth(1.0f);

    moveTo(inputIndicatorX, 0);
    lineTo(inputIndicatorX, height);

    stroke();

    closePath();

    translate(-0.5f, -0.5f);

    //circle at indicator intersection with the graph
    const float circleY = lineEditor.getValueAt(fInput) * height;

    beginPath();

    fillColor(CONFIG_NAMESPACE::playhead_circle_fill);
    strokeColor(CONFIG_NAMESPACE::playhead_circle_stroke);

    circle(inputIndicatorX, circleY, 3.5f);
    fill();
    stroke();
    closePath();
}

void GraphWidget::idleCallback()
{
    repaint();
}

void GraphWidget::drawInOutLabels()
{
    fontFace("chivo_italic");
    fontSize(36.f);
    fillColor(255, 255, 255, 125);

    textAlign(ALIGN_BOTTOM | ALIGN_RIGHT);
    text(getWidth() - 5, getHeight(), "In", nullptr);

    textAlign(ALIGN_TOP | ALIGN_LEFT);
    text(5, 0, "Out", nullptr);
}

void GraphWidget::drawVertices()
{
    const int vertexCount = lineEditor.getVertexCount();

    for (int i = 0; i < vertexCount; ++i)
    {
        const auto pos = getVertexPos(i);

        beginPath();

        strokeWidth(CONFIG_NAMESPACE::vertex_stroke_width);
        strokeColor(CONFIG_NAMESPACE::vertex_stroke_normal);
        fillColor(CONFIG_NAMESPACE::vertex_fill_normal);

        circle(pos.getX(), pos.getY(), CONFIG_NAMESPACE::vertex_radius);

        fill();
        stroke();

        closePath();
    }
}

Point<float> GraphWidget::getVertexPos(const int vertexIndex)
{
    DISTRHO_SAFE_ASSERT_RETURN(vertexIndex >= 0 && vertexIndex < lineEditor.getVertexCount(), Point<float>(0.f, 0.f));

    const auto width = static_cast<float>(getWidth());
    const auto height = static_cast<float>(getHeight());

    auto* vertex = lineEditor.getVertexAtIndex(vertexIndex);

    auto vx = vertex->getX();
    auto vy = vertex->getY();

    return Point<float>(vx * width, vy * height);
}

Point<float> GraphWidget::getTensionHandlePos(const int vertexIndex)
{
    DISTRHO_SAFE_ASSERT_RETURN(vertexIndex >= 0 && vertexIndex < lineEditor.getVertexCount() - 1, Point<float>(0.f, 0.f));

    const auto width = static_cast<float>(getWidth());
    const auto height = static_cast<float>(getHeight());

    auto* leftVertex = lineEditor.getVertexAtIndex(vertexIndex);
    auto* rightVertex = lineEditor.getVertexAtIndex(vertexIndex + 1);

    auto lx = leftVertex->getX();
    auto rx = rightVertex->getX();

    auto tx = (lx + rx) / 2.f;
    auto ty = lineEditor.getValueAt(tx);

    return Point<float>(tx * width, ty * height);
}

void GraphWidget::drawTensionHandles()
{
    const int vertexCount = lineEditor.getVertexCount();

    for (int i = 0; i < vertexCount - 1; ++i)
    {
        auto pos = getTensionHandlePos(i);

        beginPath();

        strokeWidth(CONFIG_NAMESPACE::tension_handle_stroke_width);
        strokeColor(CONFIG_NAMESPACE::tension_handle_normal);

        circle(pos.getX(), pos.getY(), CONFIG_NAMESPACE::tension_handle_radius);

        stroke();

        closePath();
    }
}

void GraphWidget::onNanoDisplay()
{
    const auto width = static_cast<float>(getWidth());
    const auto height = static_cast<float>(getHeight());

    beginPath();

    fillColor(CONFIG_NAMESPACE::graph_margin);
    strokeColor(CONFIG_NAMESPACE::side_borders);
    strokeWidth(1.0f);

    rect(0.f, 0.f, width, height);

    fill();
    stroke();

    closePath();

    beginPath();

    const float topBorderWidth = 2.0f;

    strokeColor(CONFIG_NAMESPACE::top_border);
    strokeWidth(topBorderWidth);

    moveTo(0, 1);
    lineTo(width, 1);

    stroke();

    closePath();

    translate(fMargin.left, fMargin.top);
    scale((float)(width - fMargin.left - fMargin.right) / width, (float)(height - fMargin.top - fMargin.bottom) / height);

    drawBackground();
    drawGrid();
    drawInOutLabels();

    flipYAxis();

    /* if (focusedElement != nullptr && dynamic_cast<GraphVertex*>(focusedElement))
        drawAlignmentLines(); */

    drawGradient();
    drawGraphLine(CONFIG_NAMESPACE::graph_edges_stroke_width, CONFIG_NAMESPACE::graph_edges_foreground_normal, CONFIG_NAMESPACE::graph_edges_foreground_focused); //inner

    drawInputIndicator();

    drawVertices();
    drawTensionHandles();
}

bool GraphWidget::onScroll(const ScrollEvent&)
{
    return false;
}

void GraphWidget::removeVertex(int index)
{
    //Make sure the vertex to remove is in the middle of the graph
    if (index <= 0)
        return;

    if (index >= lineEditor.getVertexCount() - 1)
        return;

    lineEditor.removeVertex(index);
    ui->setState("graph", lineEditor.serialize());

    repaint();
}

bool GraphWidget::insertVertex(const Point<int> pos)
{
    const auto width = static_cast<float>(getWidth());
    const auto height = static_cast<float>(getHeight());

    const float normalizedX = wolf::normalize(pos.getX(), width);
    const float normalizedY = wolf::normalize(pos.getY(), height);

    DISTRHO_SAFE_ASSERT_RETURN(normalizedX >= 0.f && normalizedX <= 1.f && normalizedY >= 0.f && normalizedY <= 1.f, false);

    // Avoid a crash when we make a pixel perfect click on the sides of the graph
    if (normalizedX == 0.f || normalizedX == 1.f)
    {
        return false;
    }

    int i = lineEditor.getVertexCount();

    if (i == wolf::maxVertices)
    {
        return false;
    }

    lineEditor.insertVertex(normalizedX, normalizedY, 0, fLastCurveTypeSelected);
    ui->setState("graph", lineEditor.serialize());

    return true;
}

Point<int> GraphWidget::projectCursorPos(Point<double> pt)
{
    // Flip the position upside down
    const Point<double> flippedPos = wolf::flipY(pt, getHeight());

    // Adjust for zoom
    const float innerGraphLeft = fMargin.left;
    const float innerGraphRight = getWidth() - fMargin.right;
    const float innerGraphWidth = innerGraphRight - innerGraphLeft;

    const float innerGraphTop = fMargin.top;
    const float innerGraphBottom = getHeight() - fMargin.bottom;
    const float innerGraphHeight = innerGraphBottom - innerGraphTop;

    // Inverse lerp
    const float x = (flippedPos.getX() - innerGraphLeft) / innerGraphWidth;
    const float y = (flippedPos.getY() - innerGraphTop) / innerGraphHeight;

    return Point<int>(x * getWidth(), y * getHeight());
}

bool GraphWidget::innerGraphContains(Point<double> pt)
{
    const float innerGraphLeft = fMargin.left;
    const float innerGraphRight = getWidth() - fMargin.right;
    const float innerGraphTop = fMargin.top;
    const float innerGraphBottom = getHeight() - fMargin.bottom;

    return pt.getX() >= innerGraphLeft && pt.getX() <= innerGraphRight && pt.getY() >= innerGraphTop && pt.getY() <= innerGraphBottom;
}

bool GraphWidget::leftClick(const MouseEvent& ev)
{
    const Point<int> point = projectCursorPos(ev.pos);

    if (mouseRightDown)
        return true;

    mouseLeftDown = ev.press;

    return false;
}

bool GraphWidget::middleClick(const MouseEvent&)
{
    return false;
}

void GraphWidget::rightClickMenuItemSelected(RightClickMenuItem* rightClickMenuItem)
{
}

bool GraphWidget::rightClick(const MouseEvent& ev)
{
    const Point<int> point = projectCursorPos(ev.pos);

    if (mouseLeftDown)
        return true;

    mouseRightDown = ev.press;

    return false;
}

bool GraphWidget::onMouse(const MouseEvent& ev)
{
    switch (ev.button)
    {
    case 1:
        return leftClick(ev);
    case 2:
        return middleClick(ev);
    case 3:
        return rightClick(ev);
    }

    return false;
}

bool GraphWidget::onMotion(const MotionEvent& ev)
{
    return false;
}

END_NAMESPACE_DISTRHO
