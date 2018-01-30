#include "Geometry.hpp"
#include "Graph.hpp"
#include "NanoVG.hpp"
#include "Widget.hpp"
#include "GraphNodes.hpp"
#include "GraphWidget.hpp"
#include "Mathf.hpp"


START_NAMESPACE_DISTRHO

GraphVertex::GraphVertex()
{
}

GraphVertex::GraphVertex(NanoWidget *parent, GraphVertexType type) : NanoVG(parent),
                                                                     parent(static_cast<GraphWidget*>(parent)),
                                                                     lockX(type != GraphVertexType::Middle),
                                                                     type(type),
                                                                     surface(Circle<int>(0, 0, 1.0f))
{
}

void GraphVertex::fadeIn()
{
    parent->getParentWindow().addIdleCallback(this);
}

void GraphVertex::render()
{
    beginPath();

    strokeWidth(2.0f);
    strokeColor(Color(0, 0, 0, 255));

    circle(12.0f, 12.0f, 10.f);

    fill();
    stroke();

    closePath();
}

bool GraphVertex::contains(Point<int> point)
{
    return spoonie::pointInCircle(surface, point);
}

void GraphVertex::idleCallback()
{
    fprintf(stderr, "Render\n");
}

void GraphVertex::stopFadeIn()
{
    parent->getParentWindow().removeIdleCallback(this);
}

bool GraphVertex::onMotion(const Widget::MotionEvent &ev)
{
    fprintf(stderr, "%f\n", parent->lineEditor.getVertexAtIndex(0)->x);
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