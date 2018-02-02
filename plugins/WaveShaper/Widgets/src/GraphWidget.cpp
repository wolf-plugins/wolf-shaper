#include "Widget.hpp"
#include "Window.hpp"

#include "Graph.hpp"
#include "ObjectPool.hpp"
#include "GraphWidget.hpp"
#include "WaveShaperUI.hpp"
#include "GraphNodes.hpp"
#include "Mathf.hpp"

#include <chrono>

START_NAMESPACE_DISTRHO

GraphWidget::GraphWidget(WaveShaperUI *ui, Window &parent)
    : NanoWidget(parent),
      ui(ui),
      graphVerticesPool(spoonie::maxVertices, this, GraphVertexType::Middle),
      focusedElement(nullptr)
{
    const int marginTop = 48;
    const int marginLeft = 24;
    const int marginRight = 24;
    const int marginBottom = 24;

    setSize(ui->getWidth() - marginLeft - marginRight, ui->getHeight() - marginTop - marginBottom);
    setAbsolutePos(marginLeft, marginTop);

    initializeDefaultVertices();
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

    const float lineWidth = 2.0f;

    const float squaresPerRow = 10.0f;

    const float verticalStep = width / squaresPerRow;
    const float horizontalStep = height / squaresPerRow;

    const Color gridForegroundColor = Color(51, 51, 51, 255);
    const Color gridBackgroundColor = Color(25, 24, 26, 255);
    const Color subGridColor = Color(25, 24, 26, 255);

    //vertical
    for (int i = 0; i < squaresPerRow; ++i)
    {
        const float posX = i * verticalStep;

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

        //subgrid
        beginPath();
        strokeWidth(lineWidth);
        strokeColor(subGridColor);

        moveTo(posX + verticalStep / 2.0f, 0.0f);
        lineTo(posX + verticalStep / 2.0f, height);

        stroke();
        closePath();
    }

    //horizontal
    for (int i = 0; i < squaresPerRow; ++i)
    {
        const float posY = i * horizontalStep;

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

        //subgrid
        beginPath();
        strokeWidth(lineWidth);
        strokeColor(subGridColor);

        moveTo(0.0f, posY + horizontalStep / 2.0f);
        lineTo(width, posY + horizontalStep / 2.0f);

        stroke();
        closePath();
    }
}

void GraphWidget::drawBackground()
{
    const float width = getWidth();
    const float height = getHeight();

    const float centerX = width / 2.0f;
    const float centerY = height / 2.0f;

    beginPath();

    rect(0.f, 0.f, width, height);
    Paint gradient = radialGradient(centerX, centerY, 1.0f, centerX, Color(42, 42, 42, 255), Color(33, 32, 39, 255));
    fillPaint(gradient);

    fill();

    closePath();
}

void GraphWidget::drawGraphLine(float lineWidth, Color color)
{
    const float width = getWidth();
    const float height = getHeight();

    beginPath();

    strokeColor(color);
    strokeWidth(lineWidth);

    moveTo(0.0f, 0.0f);

    const float iteration = 1.0f / width * 2.0f;

    for (float i = 0; i < 1.0f; i += iteration)
    {
        lineTo(i * width, lineEditor.getValueAt(i) * height);
    }

    lineTo(width, lineEditor.getValueAt(1.0f) * height);

    stroke();

    closePath();
}

void GraphWidget::drawVertex(int index)
{
}

void GraphWidget::drawTensionHandle(int index)
{
}

void GraphWidget::drawGraphVertices()
{
    for (int i = 0; i < lineEditor.getVertexCount() - 1; ++i)
    {
        graphVertices[i]->render();
    }

    graphVertices[lineEditor.getVertexCount() - 1]->render();
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

void GraphWidget::onNanoDisplay()
{
    flipYAxis();

    drawBackground();
    drawGrid();

    drawGraphLine(3.2f, Color(169, 29, 239, 255));  //outer
    drawGraphLine(1.2f, Color(245, 112, 188, 255)); //inner

    if (focusedElement != nullptr)
        drawAlignmentLines();

    drawGraphVertices();
}

bool GraphWidget::onScroll(const ScrollEvent &ev)
{
    float oldTension = lineEditor.getVertexAtIndex(0)->tension;
    lineEditor.setTensionAtIndex(0, spoonie::clamp(oldTension + 0.50f * -ev.delta.getY(), -100.0f, 100.0f));

    //position tension handles
    const float centerX = (lineEditor.getVertexAtIndex(0)->x + lineEditor.getVertexAtIndex(1)->x) / 2.0f;
    //tensionWidgets[0].setY(lineEditor.getValueAt(centerX) * getHeight());

    ui->setState("graph", lineEditor.serialize());

    repaint();

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

GraphVertex *GraphWidget::insertVertex(const DGL::Point<int> pos)
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

    tensionWidgets[i - 1] = DGL::Circle<int>(centerX, posY, 8.0f);*/

    const float normalizedX = spoonie::normalize(pos.getX(), width);
    const float normalizedY = spoonie::normalize(pos.getY(), height);

    lineEditor.insertVertex(normalizedX, normalizedY);

    ui->setState("graph", lineEditor.serialize());

    return vertex;
}

bool GraphWidget::leftClick(const MouseEvent &ev)
{
    const Point<int> point = spoonie::flipY(ev.pos, getHeight());

    if (!ev.press)
    {
        if (focusedElement != nullptr)
        {
            focusedElement->onMouse(ev);
            focusedElement = nullptr;
        }

        return true;
    }

    for (int i = 0; i < lineEditor.getVertexCount(); ++i)
    {
        if (graphVertices[i]->contains(point))
        {
            focusedElement = graphVertices[i];

            return focusedElement->onMouse(ev);
        }
    }

    return false;
}

bool GraphWidget::middleClick(const MouseEvent &ev)
{
    return false;
}

bool GraphWidget::rightClick(const MouseEvent &ev)
{
    const Point<int> point = spoonie::flipY(ev.pos, getHeight());

    if (ev.press)
    {
        focusedElement = insertVertex(point);

        if (focusedElement != nullptr)
            return focusedElement->onMouse(ev);
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

    for (int i = 0; i < lineEditor.getVertexCount(); ++i)
    {
        if (graphVertices[i]->contains(point))
        {
            return graphVertices[i]->onMotion(ev);
        }
    }

    //The cursor is not over any graph element
    getParentWindow().setCursorStyle(Window::CursorStyle::Default);

    return false;
}

END_NAMESPACE_DISTRHO