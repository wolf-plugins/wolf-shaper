#ifndef SPOONIE_GRAPH_NODES_HPP_INCLUDED
#define SPOONIE_GRAPH_NODES_HPP_INCLUDED

#include "Geometry.hpp"
#include "Graph.hpp"
#include "NanoVG.hpp"
#include "Widget.hpp"


START_NAMESPACE_DISTRHO

class GraphWidget;

enum GraphVertexType
{
    Left,
    Middle,
    Right
};

template <typename T, typename U>
bool pointInCircle(DGL::Circle<T> circle, DGL::Point<U> point)
{
    const float radius = circle.getSize();

    const T x = point.getX();
    const T xo = circle.getX();

    const T dx = std::abs(x - xo);

    if (dx > radius)
        return false;

    const T y = point.getY();
    const T yo = circle.getY();

    const T dy = std::abs(y - yo);

    if (dy > radius)
        return false;

    if (dx + dy <= radius)
        return true;

    return dx * dx + dy * dy <= radius * radius;
}

class GraphVertex : public IdleCallback, public NanoVG
{
  public:
    GraphVertex();
    GraphVertex(NanoWidget *parent, GraphVertexType type);

    void fadeIn();
    void render();

    bool contains(Point<int> point);

  protected:
    void idleCallback() override;
    void stopFadeIn();
    bool onMotion(const Widget::MotionEvent &ev);

  private:
    GraphWidget *parent;

    bool lockX;
    GraphVertexType type;

    Circle<int> surface;
};

class GraphTensionHandle
{
  public:
    GraphTensionHandle()
    {
    }

    GraphTensionHandle(NanoWidget *parent) : parent(parent)
    {
    }

    void render()
    {
        parent->beginPath();

        parent->strokeWidth(2.0f);
        parent->strokeColor(Color(0, 0, 0, 255));

        parent->circle(12.0f, 12.0f, 10.f);

        parent->fill();
        parent->stroke();

        parent->closePath();
    }

  private:
    NanoWidget *parent;
};

END_NAMESPACE_DISTRHO

#endif