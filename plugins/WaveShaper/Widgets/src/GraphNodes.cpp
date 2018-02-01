#include "Geometry.hpp"
#include "Graph.hpp"
#include "NanoVG.hpp"
#include "Widget.hpp"
#include "GraphNodes.hpp"
#include "GraphWidget.hpp"
#include "Mathf.hpp"
#include "WaveShaperUI.hpp"
#include "Utils.hpp"

START_NAMESPACE_DISTRHO

GraphVertex::GraphVertex()
{
}

GraphVertex::GraphVertex(GraphWidget *parent, GraphVertexType type) : NanoVG(parent),
                                                                      parent(parent),
                                                                      type(type),
                                                                      color(Color(255, 255, 255, 255)),
                                                                      grabbed(false),
                                                                      lastClickButton(0)
{
    switch (type)
    {
    case GraphVertexType::Left:
    case GraphVertexType::Middle:
        surface = Circle<int>(0, 0, 8.0f);
        break;
    case GraphVertexType::Right:
        surface = Circle<int>(parent->getWidth(), parent->getHeight(), 8.0f);
        break;
    }
}

void GraphVertex::reset()
{
    surface = Circle<int>(0, 0, 8.0f);
    color = Color(255, 255, 255, 255);
    type = GraphVertexType::Middle;
    grabbed = false;
}

void GraphVertex::fadeIn()
{
}

void GraphVertex::stopFadeIn()
{
}

bool GraphVertex::isLockedX() const
{
    return type != GraphVertexType::Middle;
}

void GraphVertex::render()
{
    beginPath();

    strokeWidth(2.0f);

    strokeColor(Color(0, 0, 0, 255));
    fillColor(color);

    circle(surface.getX(), surface.getY(), surface.getSize());

    fill();
    stroke();

    closePath();
}

bool GraphVertex::contains(Point<int> pos)
{
    return spoonie::pointInCircle(surface, pos);
}

void GraphVertex::idleCallback()
{
    parent->repaint();
}

void GraphVertex::setPos(int x, int y)
{
    surface.setPos(x, y);
}

void GraphVertex::setPos(Point<int> pos)
{
    surface.setPos(pos);
}

int GraphVertex::getX() const
{
    return surface.getX();
}

int GraphVertex::getY() const
{
    return surface.getY();
}

GraphVertex *GraphVertex::getVertexAtLeft() const
{
    if (index == 0)
        return nullptr;

    return parent->graphVertices[index - 1];
}

GraphVertex *GraphVertex::getVertexAtRight() const
{
    if (index == parent->lineEditor.getVertexCount() - 1)
        return nullptr;

    return parent->graphVertices[index + 1];
}

Point<int> GraphVertex::clampVertexPosition(const Point<int> point) const
{
    const GraphVertex *leftVertex = getVertexAtLeft();
    const GraphVertex *rightVertex = getVertexAtRight();

    int x = this->getX();
    int y = point.getY();

    if (!isLockedX())
    {
        //clamp to neighbouring vertices
        x = spoonie::clamp<int>(point.getX(), leftVertex->getX(), rightVertex->getX());
    }

    //clamp to graph
    y = spoonie::clamp<int>(y, 0, parent->getHeight());

    return Point<int>(x, y);
}

spoonie::Graph *GraphVertex::getLineEditor()
{
    return &parent->lineEditor;
}

UI *GraphVertex::getUI()
{
    return parent->ui;
}

Window *GraphVertex::getParentWindow()
{
    return &parent->getParentWindow();
}

void GraphVertex::updateGraph()
{
    const float width = parent->getWidth();
    const float height = parent->getHeight();

    const float normalizedX = spoonie::normalize(surface.getX(), width);
    const float normalizedY = spoonie::normalize(surface.getY(), height);

    spoonie::Graph *lineEditor = getLineEditor();

    lineEditor->getVertexAtIndex(index)->setPosition(normalizedX, normalizedY);

    getUI()->setState("graph", lineEditor->serialize());
}

bool GraphVertex::onMotion(const Widget::MotionEvent &ev)
{
    if (!grabbed)
    {
        getParentWindow()->setCursorStyle(Window::CursorStyle::Grab);
        return true;
    }

    Point<int> pos = spoonie::flipY(ev.pos, parent->getHeight());

    Point<int> clampedPosition = clampVertexPosition(pos);
    surface.setPos(clampedPosition);

    updateGraph();

    parent->repaint();

    //Cancel out double clicks
    lastClickButton = 0;

    return true;
}

/**
 * Make the node remove itself from the graph.
 */
void GraphVertex::removeFromGraph()
{
    parent->removeVertex(index);
}

bool GraphVertex::leftDoubleClick(const Widget::MouseEvent &ev)
{
    removeFromGraph();
    getParentWindow()->setCursorStyle(Window::CursorStyle::Default);

    return true;
}

bool GraphVertex::onMouse(const Widget::MouseEvent &ev)
{
    using namespace std::chrono;

    steady_clock::time_point now = steady_clock::now();

    bool doubleClick = ev.press && lastClickButton == ev.button && duration_cast<duration<double>>(now - lastClickTimePoint).count() < 0.250;

    if (ev.press)
    {
        lastClickTimePoint = now;
        lastClickButton = ev.button;
    }

    if (doubleClick)
        return leftDoubleClick(ev);

    grabbed = ev.press;
    Window *window = getParentWindow();

    if (grabbed)
    {
        window->hideCursor();
    }
    else
    {
        window->setCursorPos(getX(), parent->getHeight() - getY());
        window->setCursorStyle(Window::CursorStyle::Grab);

        window->showCursor();
    }

    parent->repaint();

    return true;
}

GraphTensionHandle::GraphTensionHandle()
{
}

GraphTensionHandle::GraphTensionHandle(GraphWidget *parent) : parent(parent)
{
}

void GraphTensionHandle::render()
{
    parent->beginPath();

    parent->strokeWidth(2.0f);
    parent->strokeColor(Color(0, 0, 0, 255));

    parent->circle(12.0f, 12.0f, 10.f);

    parent->fill();
    parent->stroke();

    parent->closePath();
}

END_NAMESPACE_DISTRHO