#ifndef SPOONIE_GRAPH_NODES_HPP_INCLUDED
#define SPOONIE_GRAPH_NODES_HPP_INCLUDED

#include <chrono>

START_NAMESPACE_DISTRHO

class GraphWidget;

enum GraphVertexType
{
  Left,
  Middle,
  Right
};

class GraphVertex : public IdleCallback
{
  friend class GraphWidget;

public:
  GraphVertex();
  GraphVertex(GraphWidget *parent, GraphNodesLayer *layer, GraphVertexType type);

  /**
   * Return true if the vertex cannot be moved on the X axis.
   */
  bool isLockedX() const;

  void fadeIn();
  void stopFadeIn();

  void reset();

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
  int getAbsoluteX() const;
  int getAbsoluteY() const;

protected:
  void idleCallback() override;
  bool onMotion(const Widget::MotionEvent &ev);
  bool onMouse(const Widget::MouseEvent &ev);
  bool leftDoubleClick(const Widget::MouseEvent &ev);

  void removeFromGraph();
  GraphVertex *getVertexAtLeft() const;
  GraphVertex *getVertexAtRight() const;
  spoonie::Graph *getLineEditor();
  Window *getParentWindow();

  void updateGraph();

  Point<int> clampVertexPosition(const Point<int> point) const;

private:
  GraphWidget *parent;

  int index;

  GraphVertexType type;
  GraphNodesLayer* layer;
  Circle<int> surface;
  Color color;

  std::chrono::steady_clock::time_point lastClickTimePoint;
  int lastClickButton;

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