#include "Widget.hpp"
#include "Window.hpp"

#include "Graph.hpp"
#include "ObjectPool.hpp"
#include "GraphWidget.hpp"
#include "WaveShaperUI.hpp"
#include "GraphNode.hpp"
#include "Mathf.hpp"
#include "GraphNodesLayer.hpp"
#include "Config.hpp"

#include "Fonts/roboto_light.hpp"

#include <chrono>
#include <cstdlib>
#include <cmath>

START_NAMESPACE_DISTRHO

const char *graphDefaultState = "0x0p+0,0x0p+0,0x0p+0,0;0x1p+0,0x1p+0,0x0p+0,0;";

GraphWidget::GraphWidget(WaveShaperUI *ui)
    : NanoWidget((NanoWidget*)ui),
      ui(ui),
      graphNodesLayer(this),
      graphVerticesPool(spoonie::maxVertices, this, &graphNodesLayer, GraphVertexType::Middle),
      focusedElement(nullptr),
      mouseLeftDown(false),
      mouseRightDown(false),
      margin(48, 48, 36, 84),
      maxInput(0.0f)
{
    
    const int width = ui->getWidth() - margin.left - margin.right;
    const int height = ui->getHeight() - margin.top - margin.bottom;

    setSize(width, height);
    initialSize = getSize();

    graphNodesLayer.setSize(ui->getWidth(), ui->getHeight());

    setAbsolutePos(margin.left, margin.top);

    initializeDefaultVertices();

    getParentWindow().addIdleCallback(this);

    using namespace SPOONIE_FONTS;
    createFontFromMemory("roboto_light", (const uchar *)roboto_light, roboto_light_size, 0);
}

GraphWidget::~GraphWidget()
{
    for (int i = 0; i < lineEditor.getVertexCount(); ++i)
    {
        delete graphVertices[i];
    }
}

void GraphWidget::onResize(const ResizeEvent &ev)
{
    if(ev.oldSize.isNull())
        return;

    setAbsolutePos(margin.left, margin.top);
    graphNodesLayer.setSize(ui->getWidth(), ui->getHeight());

    for (int i = 0; i < lineEditor.getVertexCount(); ++i)
    {
        GraphVertex *vertexWidget = graphVertices[i];
        spoonie::Vertex *logicalVertex = lineEditor.getVertexAtIndex(i);

        vertexWidget->setPos(logicalVertex->x * ev.size.getWidth(), logicalVertex->y * ev.size.getHeight());
    }
}

void GraphWidget::initializeDefaultVertices()
{
    //Left vertex
    GraphVertex *vertex = graphVerticesPool.getObject();

    vertex->setPos(0, 0);
    vertex->index = 0;
    vertex->type = GraphVertexType::Left;

    graphVertices[0] = vertex;

    //Right vertex
    vertex = graphVerticesPool.getObject();

    vertex->setPos(getWidth(), getHeight());
    vertex->index = 1;
    vertex->type = GraphVertexType::Right;

    graphVertices[1] = vertex;
}

void GraphWidget::reset()
{
    resetVerticesPool();

    initializeDefaultVertices();

    ui->setState("graph", graphDefaultState);
    lineEditor.rebuildFromString(graphDefaultState);
}

void GraphWidget::resetVerticesPool()
{
    for (int i = 0; i < lineEditor.getVertexCount(); ++i)
    {
        graphVerticesPool.freeObject(graphVertices[i]);
    }
}

void GraphWidget::rebuildFromString(const char *serializedGraph)
{
    resetVerticesPool();

    lineEditor.rebuildFromString(serializedGraph);

    //position ui vertices to match the new graph
    for (int i = 0; i < lineEditor.getVertexCount(); ++i)
    {
        GraphVertex *vertex = graphVerticesPool.getObject();
        const spoonie::Vertex *lineEditorVertex = lineEditor.getVertexAtIndex(i);

        const int x = lineEditorVertex->x * getWidth();
        const int y = lineEditorVertex->y * getHeight();

        vertex->setPos(x, y);
        vertex->index = i;

        if (i == 0)
            vertex->type = GraphVertexType::Left;
        else if (i == lineEditor.getVertexCount() - 1)
            vertex->type = GraphVertexType::Right;
        else
            vertex->type = GraphVertexType::Middle;

        graphVertices[i] = vertex;
    }
}

void GraphWidget::updateAnimations()
{
}

void GraphWidget::flipYAxis() //(0,0) at the bottom-left corner makes more sense for this plugin
{
    transform(1.0f, 0.0f, 0.0f, -1.0f, 0.0f, getHeight());
}

void GraphWidget::drawSubGrid()
{
}

void GraphWidget::drawGrid()
{
    const float width = getWidth();
    const float height = getHeight();

    const float lineWidth = 1.0f;

    const int squaresPerRow = 8.0f;

    const float verticalStep = width / squaresPerRow;
    const float horizontalStep = height / squaresPerRow;

    const Color gridForegroundColor = WaveShaperConfig::grid_foreground;
    const Color gridBackgroundColor = WaveShaperConfig::grid_background;
    const Color subGridColor = WaveShaperConfig::sub_grid;

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
        strokeColor(gridForegroundColor);

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

        strokeColor(gridForegroundColor);

        stroke();
        closePath();
    }
}

void GraphWidget::drawBackground()
{
    const float width = getWidth();
    const float height = getHeight();

    //const float centerX = width / 2.0f;
    //const float centerY = height / 2.0f;

    beginPath();

    rect(0.f, 0.f, width, height);
    //Paint gradient = radialGradient(centerX, centerY, 1.0f, centerX, Color(42, 42, 42, 255), Color(33, 32, 39, 255));
    //fillPaint(gradient);
    fillColor(WaveShaperConfig::graph_background);
    fill();

    closePath();
}

bool GraphWidget::edgeMustBeEmphasized(int vertexIndex)
{
    if (focusedElement == nullptr)
        return false;

    GraphVertex *vertex = graphVertices[vertexIndex];
    const GraphVertexType type = vertex->getType();

    if (dynamic_cast<GraphTensionHandle *>(focusedElement))
        return focusedElement == vertex->getTensionHandle();

    if (type == GraphVertexType::Right)
        return false; //there is no edge at the right of the last vertex

    return focusedElement == vertex || focusedElement == vertex->getVertexAtRight();
}

void GraphWidget::drawGraphEdge(int vertexIndex, float lineWidth, Color color)
{
    DISTRHO_SAFE_ASSERT(vertexIndex < lineEditor.getVertexCount() - 1);

    const float width = getWidth();
    const float height = getHeight();

    const spoonie::Vertex *leftVertex = lineEditor.getVertexAtIndex(vertexIndex);
    const spoonie::Vertex *rightVertex = lineEditor.getVertexAtIndex(vertexIndex + 1);

    beginPath();

    strokeColor(color);
    strokeWidth(lineWidth);

    moveTo(leftVertex->x * width, leftVertex->y * height);

    const float edgeLength = (rightVertex->x - leftVertex->x) * width;

    for (int i = 0; i <= edgeLength; ++i)
    {
        const float normalizedX = leftVertex->x + i / width;

        lineTo(normalizedX * width, lineEditor.getValueAt(normalizedX) * height);
    }

    lineTo(rightVertex->x * width, rightVertex->y * height);

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
    const int x = focusedElement->getX();
    const int y = focusedElement->getY();
    const int width = getWidth();
    const int height = getHeight();

    beginPath();

    strokeWidth(1.0f);
    strokeColor(WaveShaperConfig::alignment_lines);

    moveTo(x, 0);
    lineTo(x, height);

    moveTo(0, y);
    lineTo(width, y);

    stroke();

    closePath();
}

void GraphWidget::drawInputIndicator()
{
    const float width = getWidth();
    const float height = getHeight();

    if (maxInput <= 0.0f)
        return;

    const float inputIndicatorX = std::round(maxInput * width);

    beginPath();

    strokeColor(WaveShaperConfig::input_volume_indicator);
    strokeWidth(2.0f);

    moveTo(inputIndicatorX, 0);
    lineTo(inputIndicatorX, height);

    stroke();

    closePath();
}

void GraphWidget::idleCallback()
{
    const float input = ui->getParameterValue(paramOut);
    const float deadZone = 0.001f;

    if (input > deadZone && input > maxInput)
    {
        maxInput = input;
        maxInputAcceleration = 0.0f;

        repaint();
    }
    else if (maxInput > -deadZone)
    {
        maxInput -= maxInputAcceleration;
        maxInputAcceleration += std::pow(maxInputAcceleration + 0.01f, 2); //not sure if visually pleasant

        repaint();
    }
}

void GraphWidget::drawInOutLabels()
{
    fontFace("roboto_light");
    fontSize(32.f);
    fillColor(255, 255, 255, 125);

    textAlign(ALIGN_BOTTOM | ALIGN_RIGHT);
    text(getWidth() - 5, getHeight(), "In", NULL);

    textAlign(ALIGN_TOP | ALIGN_LEFT);
    text(5, 0, "Out", NULL);
}

void GraphWidget::onNanoDisplay()
{
    drawBackground();
    drawGrid();

    drawInOutLabels();

    flipYAxis();

    drawGraphLine(5.0f, WaveShaperConfig::graph_edges_background_normal, WaveShaperConfig::graph_edges_background_focused);    //outer
    drawGraphLine(1.1416f, WaveShaperConfig::graph_edges_foreground_normal, WaveShaperConfig::graph_edges_foreground_focused); //inner

    drawInputIndicator();

    if (focusedElement != nullptr && dynamic_cast<GraphVertex *>(focusedElement))
        drawAlignmentLines();
}

bool GraphWidget::onScroll(const ScrollEvent &ev)
{
    const Point<int> point = spoonie::flipY(ev.pos, getHeight());

    //Testing for mouse hover on tension handles
    for (int i = 0; i < lineEditor.getVertexCount() - 1; ++i)
    {
        GraphTensionHandle *tensionHandle = graphVertices[i]->getTensionHandle();

        if (tensionHandle->contains(point))
        {
            const float delta = graphVertices[i]->getY() < graphVertices[i + 1]->getY() ? -ev.delta.getY() : ev.delta.getY();
            const float oldTension = lineEditor.getVertexAtIndex(i)->tension;

            lineEditor.setTensionAtIndex(i, spoonie::clamp(oldTension + 1.5f * delta, -100.0f, 100.0f));

            ui->setState("graph", lineEditor.serialize());
            repaint();

            getParentWindow().setCursorPos(tensionHandle->getAbsoluteX(), tensionHandle->getAbsoluteY());

            return true;
        }
    }

    return false;
}

void GraphWidget::removeVertex(int index)
{
    //Make sure the vertex to remove is in the middle of the graph
    if (index <= 0)
        return;
    else if (index >= lineEditor.getVertexCount() - 1)
        return;

    //Get rid of the ui widget
    graphVerticesPool.freeObject(graphVertices[index]);

    const int vertexCount = lineEditor.getVertexCount() - 1;

    for (int i = index; i < vertexCount; ++i)
    {
        graphVertices[i] = graphVertices[i + 1];
        graphVertices[i]->index--;
    }

    //Get rid of the logical vertex and update dsp
    lineEditor.removeVertex(index);
    ui->setState("graph", lineEditor.serialize());

    focusedElement = nullptr;

    repaint();
}

GraphVertex *GraphWidget::insertVertex(const Point<int> pos)
{
    int i = lineEditor.getVertexCount();

    if (i == spoonie::maxVertices)
        return nullptr;

    while ((i > 0) && (pos.getX() < graphVertices[i - 1]->getX()))
    {
        graphVertices[i] = graphVertices[i - 1];
        graphVertices[i]->index++;

        --i;
    }

    GraphVertex *vertex = graphVerticesPool.getObject();
    vertex->setPos(pos);
    vertex->index = i;

    graphVertices[i] = vertex;

    const float width = getWidth();
    const float height = getHeight();

    const float normalizedX = spoonie::normalize(pos.getX(), width);
    const float normalizedY = spoonie::normalize(pos.getY(), height);

    lineEditor.insertVertex(normalizedX, normalizedY);

    ui->setState("graph", lineEditor.serialize());

    return vertex;
}

GraphNode *GraphWidget::getHoveredNode(Point<int> cursorPos)
{
    //Testing for mouse hover on graph vertices
    for (int i = lineEditor.getVertexCount() - 1; i >= 0; --i)
    {
        if (graphVertices[i]->contains(cursorPos))
        {
            return graphVertices[i];
        }
    }

    //Testing for mouse hover on tension handles
    for (int i = lineEditor.getVertexCount() - 1; i >= 0; --i)
    {
        if (graphVertices[i]->tensionHandle.contains(cursorPos))
        {
            return &graphVertices[i]->tensionHandle;
        }
    }

    return nullptr;
}

bool GraphWidget::leftClick(const MouseEvent &ev)
{
    const Point<int> point = spoonie::flipY(ev.pos, getHeight());

    if (mouseRightDown)
        return true;

    mouseLeftDown = ev.press;

    if (!ev.press)
    {
        if (focusedElement != nullptr)
        {
            focusedElement->onMouse(ev);
            focusedElement = nullptr;
        }

        return true;
    }

    GraphNode *hoveredNode = getHoveredNode(point);

    if (hoveredNode != nullptr)
    {
        focusedElement = hoveredNode;

        return focusedElement->onMouse(ev);
    }

    //The cursor is not over any graph node
    return false;
}

bool GraphWidget::middleClick(const MouseEvent &)
{
    return false;
}

bool GraphWidget::rightClick(const MouseEvent &ev)
{
    const Point<int> point = spoonie::flipY(ev.pos, getHeight());

    if (mouseLeftDown)
        return true;

    mouseRightDown = ev.press;

    if (focusedElement == nullptr)
    {
        if (getHoveredNode(point) != nullptr)
            return true;

        if (ev.press && contains(ev.pos))
        {
            focusedElement = insertVertex(point);

            if (focusedElement != nullptr)
                return focusedElement->onMouse(ev);
        }
    }
    else
    {
        focusedElement->onMouse(ev);
        focusedElement = nullptr;

        return true;
    }

    return false;
}

bool GraphWidget::onMouse(const MouseEvent &ev)
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

bool GraphWidget::onMotion(const MotionEvent &ev)
{
    const Point<int> point = spoonie::flipY(ev.pos, getHeight());

    if (focusedElement)
    {
        return focusedElement->onMotion(ev);
    }

    GraphNode *hoveredNode = getHoveredNode(point);

    if (hoveredNode != nullptr)
    {
        return hoveredNode->onMotion(ev);
    }

    //The cursor is not over any graph node
    getParentWindow().setCursorStyle(Window::CursorStyle::Default);

    return false;
}

const Margin GraphWidget::getMargin()
{
    return margin;
}

void GraphWidget::setMargin(Margin margin)
{
    this->margin = margin;
}

END_NAMESPACE_DISTRHO
