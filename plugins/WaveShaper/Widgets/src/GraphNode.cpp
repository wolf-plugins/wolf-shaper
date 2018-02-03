#include "Geometry.hpp"
#include "Graph.hpp"
#include "NanoVG.hpp"
#include "Window.hpp"
#include "WaveShaperUI.hpp"
#include "GraphWidget.hpp"
#include "GraphNode.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

GraphNode::GraphNode(GraphWidget *parent, GraphNodesLayer *layer) : parent(parent),
                                                                    layer(layer),
                                                                    surface(Circle<int>(0, 0, 8.0f)),
                                                                    color(Color(255, 255, 255, 255)),
                                                                    grabbed(false)
{
}

GraphNode::~GraphNode() {}

bool GraphNode::onMotion(const Widget::MotionEvent &ev) {}
bool GraphNode::onMouse(const Widget::MouseEvent &ev) {}
void GraphNode::render() {}

GraphVertex::GraphVertex(GraphWidget *parent, GraphNodesLayer *layer, GraphVertexType type) : GraphNode(parent, layer),
                                                                                              tensionHandle(parent, layer),
                                                                                              type(type),
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

bool GraphVertex::isLockedX() const
{
    return type != GraphVertexType::Middle;
}

void GraphVertex::render()
{
    layer->beginPath();

    layer->strokeWidth(2.0f);

    layer->strokeColor(Color(0, 0, 0, 255));
    layer->fillColor(color);

    layer->circle(parent->marginLeft + surface.getX(), parent->getHeight() - surface.getY() + parent->marginTop, surface.getSize());

    layer->fill();
    layer->stroke();

    layer->closePath();
}

bool GraphNode::contains(Point<int> pos)
{
    return spoonie::pointInCircle(surface, pos);
}

void GraphNode::idleCallback()
{
    parent->repaint();
}

void GraphNode::setPos(int x, int y)
{
    surface.setPos(x, y);
}

void GraphNode::setPos(Point<int> pos)
{
    surface.setPos(pos);
}

int GraphNode::getX() const
{
    return surface.getX();
}

int GraphNode::getY() const
{
    return surface.getY();
}

int GraphNode::getAbsoluteX() const
{
    return getX() + parent->getAbsoluteX();
}

int GraphNode::getAbsoluteY() const
{
    return parent->getHeight() - getY() + parent->getAbsoluteY();
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

    parent->ui->setState("graph", lineEditor->serialize());
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
 * Make the vertex remove itself from the graph.
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
        window->setCursorPos(getAbsoluteX(), getAbsoluteY());
        window->setCursorStyle(Window::CursorStyle::Grab);

        window->showCursor();
    }

    parent->repaint();

    return true;
}

GraphTensionHandle::GraphTensionHandle(GraphWidget *parent, GraphNodesLayer *layer) : GraphNode(parent, layer)
{
}

void GraphTensionHandle::render()
{
}

END_NAMESPACE_DISTRHO