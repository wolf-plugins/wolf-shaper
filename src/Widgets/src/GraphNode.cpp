#include "GraphNode.hpp"
#include "Config.hpp"
#include "Geometry.hpp"
#include "Graph.hpp"
#include "GraphWidget.hpp"
#include "Mathf.hpp"
#include "NanoVG.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

GraphNode::GraphNode(GraphWidget *parent)
    : parent(parent),
      scaleFactor(parent->getScaleFactor()),
      grabbed(false)
{
}

GraphNode::~GraphNode()
{
}

bool GraphNode::onMotion(const Widget::MotionEvent &)
{
    return false;
}
bool GraphNode::onMouse(const Widget::MouseEvent &)
{
    return false;
}
void GraphNode::render()
{
}

GraphVertex::GraphVertex(GraphWidget *parent, GraphVertexType type)
    : GraphNode(parent),
      tensionHandle(parent, this),
      index(0),
      scaleFactor(parent->getScaleFactor()),
      surface(Circle<int>(0, 0, 8.0f * scaleFactor)),
      type(type),
      lastClickButton(0)
{
    switch (type)
    {
    case GraphVertexType::Left:
    case GraphVertexType::Middle:
        surface = Circle<int>(0, 0, CONFIG_NAMESPACE::vertex_radius * scaleFactor);
        break;
    case GraphVertexType::Right:
        surface = Circle<int>(parent->getWidth(), parent->getHeight(), CONFIG_NAMESPACE::vertex_radius * scaleFactor);
        break;
    }
}

void GraphVertex::reset()
{
    surface = Circle<int>(0, 0, CONFIG_NAMESPACE::vertex_radius * scaleFactor);
    type = GraphVertexType::Middle;
    grabbed = false;
}

bool GraphVertex::isLockedX() const
{
    return type != GraphVertexType::Middle;
}

void GraphVertex::render()
{
    const bool focused = parent->focusedElement == this;

    parent->beginPath();

    parent->strokeWidth(CONFIG_NAMESPACE::vertex_stroke_width * scaleFactor);

    if (focused)
    {
        parent->strokeColor(CONFIG_NAMESPACE::vertex_stroke_focused);
        parent->fillColor(CONFIG_NAMESPACE::vertex_fill_focused);
    }
    else
    {
        parent->strokeColor(CONFIG_NAMESPACE::vertex_stroke_normal);
        parent->fillColor(CONFIG_NAMESPACE::vertex_fill_normal);
    }

    parent->circle(getX(), getY(), getSize());

    parent->fill();
    parent->stroke();

    parent->closePath();
}

GraphVertexType GraphVertex::getType()
{
    return type;
}

bool GraphVertex::contains(Point<int> pos)
{
    Circle<int> surface(getX(), getY(), 8.0f * scaleFactor);

    return wolf::pointInCircle(surface, pos);
}

bool GraphTensionHandle::contains(Point<int> pos)
{
    if (vertex->getType() == GraphVertexType::Right) //last vertex doesn't have a tension handle
        return false;

    Circle<int> surface(getX(), getY(), 8.0f * scaleFactor);

    return wolf::pointInCircle(surface, pos);
}

void GraphNode::idleCallback()
{
}

void GraphVertex::setPos(int x, int y)
{
    surface.setPos(x, y);
}

void GraphVertex::setPos(Point<int> pos)
{
    surface.setPos(pos);
}

float GraphVertex::getX() const
{
    return surface.getX();
}

float GraphVertex::getY() const
{
    return surface.getY();
}

float GraphVertex::getSize() const
{
    return surface.getSize();
}

int GraphNode::getAbsoluteX() const
{
    return getX() + parent->getAbsoluteX();
}

int GraphNode::getAbsoluteY() const
{
    return parent->getHeight() - getY() + parent->getAbsoluteY();
}

float GraphTensionHandle::getX() const
{
    GraphVertex *leftVertex = vertex;
    GraphVertex *rightVertex = leftVertex->getVertexAtRight();

    return (leftVertex->getX() + rightVertex->getX()) / 2.0f;
}

wolf::Graph *GraphNode::getLineEditor() const
{
    return &parent->lineEditor;
}

float GraphVertex::getTension()
{
    return getLineEditor()->getVertexAtIndex(index)->getTension();
}

float GraphTensionHandle::getY() const
{
    GraphVertex *leftVertex = vertex;
    GraphVertex *rightVertex = leftVertex->getVertexAtRight();

    float tension = vertex->getTension();

    //calculate value for generic curve
    float p1x = 0.0f;
    float p1y = leftVertex->getY() / parent->getHeight();
    float p2x = 1.0f;
    float p2y = rightVertex->getY() / parent->getHeight();

    const int index = leftVertex->getIndex();

    wolf::CurveType curveType = getLineEditor()->getVertexAtIndex(index)->getType();

    return wolf::Graph::getOutValue(0.5f, tension, p1x, p1y, p2x, p2y, curveType) * parent->getHeight();
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

GraphTensionHandle *GraphVertex::getTensionHandle()
{
    return &tensionHandle;
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
        x = wolf::clamp<int>(point.getX(), leftVertex->getX() + 1, rightVertex->getX() - 1);
    }

    //clamp to graph
    y = wolf::clamp<int>(y, 0, parent->getHeight());

    return Point<int>(x, y);
}

Window &GraphNode::getParentWindow()
{
    return parent->getWindow();
}

void GraphVertex::updateGraph()
{
    const float width = parent->getWidth();
    const float height = parent->getHeight();

    const float normalizedX = wolf::normalize(surface.getX(), width);
    const float normalizedY = wolf::normalize(surface.getY(), height);

    wolf::Graph *lineEditor = &parent->lineEditor;

    lineEditor->getVertexAtIndex(index)->setPosition(normalizedX, normalizedY);

    parent->ui->setState("graph", lineEditor->serialize());
}

bool GraphVertex::onMotion(const Widget::MotionEvent &ev)
{
    if (!grabbed)
    {
        //getParentWindow().setCursorStyle(Window::CursorStyle::Grab);
        return true;
    }

    const Point<int> pos = parent->projectCursorPos(ev.pos);

    Point<int> clampedPosition = clampVertexPosition(pos);
    surface.setPos(clampedPosition);

    updateGraph();

    parent->repaint();

    //Cancel out double clicks
    lastClickButton = 0;

    return true;
}

int GraphVertex::getIndex()
{
    return index;
}

void GraphTensionHandle::reset()
{
    wolf::Graph *lineEditor = getLineEditor();
    lineEditor->getVertexAtIndex(vertex->getIndex())->setTension(0);

    parent->ui->setState("graph", lineEditor->serialize());
}

bool GraphTensionHandle::onMotion(const Widget::MotionEvent &ev)
{
    if (!grabbed)
    {
        //parent->getParentWindow().setCursorStyle(Window::CursorStyle::Grab);
        return true;
    }

    const float resistance = 4.0f * scaleFactor;

    const Point<int> pos = parent->projectCursorPos(ev.pos);

    const GraphVertex *leftVertex = vertex;
    const GraphVertex *rightVertex = vertex->getVertexAtRight();

    float tension = vertex->getTension();
    float difference = mouseDownPosition.getY() - pos.getY();

    if (leftVertex->getY() > rightVertex->getY())
        difference = -difference;

    mouseDownPosition = pos;

    tension = wolf::clamp(tension + difference / resistance, -100.0f, 100.0f);

    wolf::Graph *lineEditor = getLineEditor();
    lineEditor->getVertexAtIndex(vertex->getIndex())->setTension(tension);

    parent->ui->setState("graph", lineEditor->serialize());

    parent->repaint();

    return true;
}

void GraphVertex::removeFromGraph()
{
    parent->removeVertex(index);
}

bool GraphVertex::leftDoubleClick(const Widget::MouseEvent &)
{
    removeFromGraph();
    //getParentWindow().setCursorStyle(Window::CursorStyle::Default);

    return true;
}

void GraphVertex::clipCursorToNeighbouringVertices()
{
}

bool GraphVertex::onMouse(const Widget::MouseEvent &ev)
{
    using namespace std::chrono;

    steady_clock::time_point now = steady_clock::now();

    bool doubleClick = ev.press && lastClickButton == (int)ev.button && duration_cast<duration<double>>(now - lastClickTimePoint).count() < 0.250;

    if (ev.press)
    {
        lastClickTimePoint = now;
        lastClickButton = (int)ev.button;
    }

    if (doubleClick)
    {
        lastClickButton = -1;

        if (this->type == GraphVertexType::Middle) //vertices on the sides don't receive double click, cause they can't get removed
            return leftDoubleClick(ev);
    }

    grabbed = ev.press;

    parent->repaint();

    return true;
}

GraphTensionHandle::GraphTensionHandle(GraphWidget *parent, GraphVertex *vertex)
    : GraphNode(parent),
      vertex(vertex)
{
}

bool GraphTensionHandle::onMouse(const Widget::MouseEvent &ev)
{
    grabbed = ev.press;

    if (grabbed)
    {
        const Point<int> pos = parent->projectCursorPos(ev.pos);

        mouseDownPosition = pos;
    }

    parent->repaint();

    return true;
}

void GraphTensionHandle::render()
{
    if (vertex->getType() == GraphVertexType::Right) //last vertex doesn't have a tension handle
        return;

    parent->beginPath();

    parent->strokeWidth(CONFIG_NAMESPACE::tension_handle_stroke_width * scaleFactor);

    if (parent->edgeMustBeEmphasized(vertex->getIndex())) //TODO: make that a method on the vertex
        parent->strokeColor(CONFIG_NAMESPACE::tension_handle_focused);
    else
        parent->strokeColor(CONFIG_NAMESPACE::tension_handle_normal);

    parent->circle(getX(), getY(), CONFIG_NAMESPACE::tension_handle_radius * scaleFactor);

    parent->stroke();

    parent->closePath();
}

END_NAMESPACE_DISTRHO
