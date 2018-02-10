#include "Widget.hpp"
#include "Window.hpp"

#include "Graph.hpp"
#include "ObjectPool.hpp"
#include "GraphWidget.hpp"
#include "WaveShaperUI.hpp"
#include "GraphNode.hpp"
#include "Mathf.hpp"
#include "GraphNodesLayer.hpp"

#include <chrono>
#include <cstdlib>
#include <cmath>

START_NAMESPACE_DISTRHO

GraphWidget::GraphWidget(WaveShaperUI *ui, Window &parent)
    : NanoWidget(parent),
      ui(ui),
      graphNodesLayer(this, parent),
      graphVerticesPool(spoonie::maxVertices, this, &graphNodesLayer, GraphVertexType::Middle),
      focusedElement(nullptr),
      mouseLeftDown(false),
      mouseRightDown(false),
      maxInput(0.0f)

{
    const int width = ui->getWidth() - marginLeft - marginRight;
    const int height = ui->getHeight() - marginTop - marginBottom;

    setSize(width, height);

    graphNodesLayer.setSize(ui->getWidth(), ui->getHeight());

    setAbsolutePos(marginLeft, marginTop);

    initializeDefaultVertices();

    parent.addIdleCallback(this);

    getParentWindow().clipCursor(this);
}

GraphWidget::~GraphWidget()
{
    for (int i = 0; i < lineEditor.getVertexCount(); ++i)
    {
        delete graphVertices[i];
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

    const Color gridForegroundColor = Color(103, 98, 102, 255);
    const Color gridBackgroundColor = Color(25, 24, 26, 255);
    const Color subGridColor = Color(27, 27, 27, 255);

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
    fillColor(Color(40, 40, 47, 255));
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
    strokeColor(Color(255, 255, 255, 180));

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

    strokeColor(Color(255, 255, 255, 122));
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

void GraphWidget::onNanoDisplay()
{
    flipYAxis();

    drawBackground();
    drawGrid();

    drawGraphLine(5.0f, Color(169, 29, 239, 100), Color(255, 255, 0, 100));     //outer
    drawGraphLine(1.1416f, Color(245, 112, 188, 255), Color(255, 255, 0, 255)); //inner

    drawInputIndicator();

    if (focusedElement != nullptr && dynamic_cast<GraphVertex *>(focusedElement))
        drawAlignmentLines();
}

bool GraphWidget::onScroll(const ScrollEvent &)
{
    /*float oldTension = lineEditor.getVertexAtIndex(0)->tension;
    lineEditor.setTensionAtIndex(0, spoonie::clamp(oldTension + 0.50f * -ev.delta.getY(), -100.0f, 100.0f));

    //position tension handles
    const float centerX = (lineEditor.getVertexAtIndex(0)->x + lineEditor.getVertexAtIndex(1)->x) / 2.0f;
    //tensionWidgets[0].setY(lineEditor.getValueAt(centerX) * getHeight());

    ui->setState("graph", lineEditor.serialize());

    repaint();
    */
    return true;
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

    /*const spoonie::Vertex *vertexLeft = lineEditor.getVertexAtIndex(i);
    const spoonie::Vertex *vertexRight = lineEditor.getVertexAtIndex(i + 1);

    const float normalizedCenterX = (vertexLeft->x + vertexRight->x) / 2.0f;
    const float centerX = normalizedCenterX * width;

    const float posY = lineEditor.getValueAt(normalizedCenterX) * height;

    tensionWidgets[i - 1] = Circle<int>(centerX, posY, 8.0f);*/

    const float normalizedX = spoonie::normalize(pos.getX(), width);
    const float normalizedY = spoonie::normalize(pos.getY(), height);

    lineEditor.insertVertex(normalizedX, normalizedY);

    ui->setState("graph", lineEditor.serialize());

    return vertex;
}

bool GraphWidget::leftClick(const MouseEvent &ev)
{
    const Point<int> point = spoonie::flipY(ev.pos, getHeight());

    if (mouseRightDown)
        return true;

    mouseLeftDown = ev.press;

    if (!ev.press)
    {
        getParentWindow().unclipCursor();

        if (focusedElement != nullptr)
        {
            focusedElement->onMouse(ev);
            focusedElement = nullptr;
        }

        return true;
    }

    getParentWindow().clipCursor(this);

    //Testing for mouse hover on graph vertices
    for (int i = lineEditor.getVertexCount() - 1; i >= 0; --i)
    {
        if (graphVertices[i]->contains(point))
        {
            focusedElement = graphVertices[i];

            return focusedElement->onMouse(ev);
        }
    }

    //Testing for mouse hover on tension handles
    for (int i = lineEditor.getVertexCount() - 1; i >= 0; --i)
    {
        if (graphVertices[i]->tensionHandle.contains(point))
        {
            focusedElement = &graphVertices[i]->tensionHandle;

            return focusedElement->onMouse(ev);
        }
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
        if (ev.press)
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

    //Testing for mouse hover on graph nodes
    for (int i = 0; i < lineEditor.getVertexCount(); ++i)
    {
        if (graphVertices[i]->contains(point))
        {
            return graphVertices[i]->onMotion(ev);
        }
        else if (graphVertices[i]->tensionHandle.contains(point))
        {
            return graphVertices[i]->tensionHandle.onMotion(ev);
        }
    }

    //The cursor is not over any graph node
    getParentWindow().setCursorStyle(Window::CursorStyle::Default);

    return false;
}

END_NAMESPACE_DISTRHO
