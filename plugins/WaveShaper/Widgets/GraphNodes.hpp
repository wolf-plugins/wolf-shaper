#ifndef SPOONIE_GRAPH_NODES_HPP_INCLUDED
#define SPOONIE_GRAPH_NODES_HPP_INCLUDED

#include "Geometry.hpp"
#include "Graph.hpp"
#include "NanoVG.hpp"
#include "Widget.hpp"
#include "GraphWidget.hpp"
#include "Mathf.hpp"


START_NAMESPACE_DISTRHO

class GraphWidget;

enum GraphVertexType
{
    Left,
    Middle,
    Right
};

class GraphVertex : public IdleCallback, public NanoVG
{
    friend class GraphWidget;

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
    GraphTensionHandle();
    GraphTensionHandle(GraphWidget *parent);

    void render();

  private:
    GraphWidget *parent;
};

END_NAMESPACE_DISTRHO

#endif