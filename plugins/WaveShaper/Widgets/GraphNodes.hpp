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
  GraphVertex(GraphWidget *parent, GraphVertexType type);

  /**
   * Return true if the vertex cannot be moved on the X axis.
   */
  bool isLockedX() const;

  void fadeIn();
  void stopFadeIn();

  /**
   * Draw the vertex.
   */
  void render();

  /**
   * Check if this vertex contains the point @a pos.
   */
  bool contains(Point<int> pos);

  void setPos(int x, int y);
  void setPos(Point<int> pos);

  int getX() const;
  int getY() const;

protected:
  void idleCallback() override;
  bool onMotion(const Widget::MotionEvent &ev);
  bool onMouse(const Widget::MouseEvent &ev);

  GraphVertex *getVertexAtLeft() const;
  GraphVertex *getVertexAtRight() const;
  spoonie::Graph *getLineEditor();
  UI *getUI();
  Window* getParentWindow();

  void updateGraph();

  Point<int> clampVertexPosition(const Point<int> point) const;

private:
  GraphWidget *parent;

  int index;

  GraphVertexType type;

  Circle<int> surface;
  Color color;

  bool grabbed;

  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphVertex)
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