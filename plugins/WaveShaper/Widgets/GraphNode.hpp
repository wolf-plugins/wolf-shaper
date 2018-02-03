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

class GraphNode : public IdleCallback
{
public:
  friend class GraphWidget;

  GraphNode(GraphWidget *parent, GraphNodesLayer *layer);
  ~GraphNode();

  /**
   * Draw the node.
   */
  virtual void render();

  /**
   * Check if this node contains the point @a pos.
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

  virtual bool onMotion(const Widget::MotionEvent &ev);
  virtual bool onMouse(const Widget::MouseEvent &ev);

  GraphWidget *parent;
  GraphNodesLayer *layer;
  Circle<int> surface;
  Color color;

  bool grabbed;
};

class GraphTensionHandle : public GraphNode
{
public:
  GraphTensionHandle(GraphWidget *parent, GraphNodesLayer *layer);

  void render() override;
};

class GraphVertex : public GraphNode
{
  friend class GraphWidget;

public:
  GraphVertex(GraphWidget *parent, GraphNodesLayer *layer, GraphVertexType type);

  void reset();
  void render() override;

  /**
   * Return true if the vertex cannot be moved on the X axis.
   */
  bool isLockedX() const;

  void removeFromGraph();

protected:
  bool leftDoubleClick(const Widget::MouseEvent &ev);

  GraphVertex *getVertexAtLeft() const;
  GraphVertex *getVertexAtRight() const;
  spoonie::Graph *getLineEditor();
  Window *getParentWindow();

  GraphTensionHandle tensionHandle;

  bool onMotion(const Widget::MotionEvent &ev) override;
  bool onMouse(const Widget::MouseEvent &ev) override;

  void updateGraph();

  Point<int> clampVertexPosition(const Point<int> point) const;

private:
  int index;

  GraphVertexType type;

  std::chrono::steady_clock::time_point lastClickTimePoint;
  int lastClickButton;

  float tension;

  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphVertex)
};

END_NAMESPACE_DISTRHO

#endif