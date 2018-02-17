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
  virtual bool contains(Point<int> pos) = 0;

  virtual float getX() const = 0;
  virtual float getY() const = 0;

  int getAbsoluteX() const;
  int getAbsoluteY() const;
  
  Window &getParentWindow();

protected:
  void idleCallback() override;

  virtual bool onMotion(const Widget::MotionEvent &ev);
  virtual bool onMouse(const Widget::MouseEvent &ev);

  spoonie::Graph *getLineEditor() const;

  GraphWidget *parent;
  GraphNodesLayer *layer;
  Color color;

  bool grabbed;
};

class GraphTensionHandle : public GraphNode
{
public:
  friend class GraphWidget;

  GraphTensionHandle(GraphWidget *parent, GraphNodesLayer *layer, GraphVertex *vertex);

  void render() override;
  bool contains(Point<int> pos) override;

  float getX() const override;
  float getY() const override;

protected:
  bool onMotion(const Widget::MotionEvent &ev) override;
  bool onMouse(const Widget::MouseEvent &ev) override;

private:
  GraphVertex *vertex;
  Point<int> mouseDownPosition;
  
  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphTensionHandle)
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

  bool contains(Point<int> pos) override;
  void setPos(int x, int y);
  void setPos(Point<int> pos);

  float getX() const override;
  float getY() const override;
  float getSize() const;

  float getTension();
  int getIndex();

  GraphVertex *getVertexAtLeft() const;
  GraphVertex *getVertexAtRight() const;
  GraphTensionHandle *getTensionHandle();

  GraphVertexType getType();

protected:
  bool leftDoubleClick(const Widget::MouseEvent &ev);

  GraphTensionHandle tensionHandle;

  bool onMotion(const Widget::MotionEvent &ev) override;
  bool onMouse(const Widget::MouseEvent &ev) override;

  void clipCursorToNeighbouringVertices();

  void updateGraph();

  Point<int> clampVertexPosition(const Point<int> point) const;

private:
  int index;
  Circle<int> surface;

  GraphVertexType type;

  std::chrono::steady_clock::time_point lastClickTimePoint;
  int lastClickButton;

  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphVertex)
};

END_NAMESPACE_DISTRHO

#endif