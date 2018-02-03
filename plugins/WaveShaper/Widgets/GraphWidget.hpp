#ifndef SPOONIE_GRAPH_WIDGET_HPP_INCLUDED
#define SPOONIE_GRAPH_WIDGET_HPP_INCLUDED

#include "ObjectPool.hpp"
#include "DistrhoUI.hpp"
#include "Graph.hpp"
#include "GraphNodesLayer.hpp"

START_NAMESPACE_DISTRHO

class GraphVertex;
class GraphTensionHandle;
class GraphNode;
class WaveShaperUI;

class GraphWidget : public NanoWidget
{
  friend class GraphNode;
  friend class GraphVertex;
  friend class GraphTensionHandle;
  friend class GraphNodesLayer;
  
public:
  GraphWidget(WaveShaperUI *ui, Window &parent);
  void rebuildFromString(const char *serializedGraph);
  void reset();

protected:
  void onNanoDisplay() override;
  bool onScroll(const ScrollEvent &ev) override;
  bool onMouse(const MouseEvent &ev) override;
  bool onMotion(const MotionEvent &ev) override;

  void updateAnimations();
  void flipYAxis();
  void drawSubGrid();
  void drawGrid();
  void drawBackground();
  void drawGraphLine(float lineWidth, Color color);
  void drawAlignmentLines();
  void positionTensionHandles();

  /**
   * Insert a new vertex into the graph and return a pointer to it. 
   */
  GraphVertex *insertVertex(const DGL::Point<int> pos);
  void removeVertex(const int index);

  bool leftClick(const MouseEvent &ev);
  bool middleClick(const MouseEvent &ev);
  bool rightClick(const MouseEvent &ev);

private:
  void initializeDefaultVertices();
  void resetVerticesPool();

  bool mouseDown;
  Point<int> mouseDownLocation;

  spoonie::Graph lineEditor;

  spoonie::ObjectPool<GraphVertex> graphVerticesPool;

  GraphVertex *graphVertices[spoonie::maxVertices];

  GraphNode *focusedElement;
  GraphNodesLayer graphNodesLayer;

  const float absoluteVertexSize = 7.0f;
  
  const int marginTop = 48;
  const int marginLeft = 36;
  const int marginRight = 36;
  const int marginBottom = 48;

  WaveShaperUI *ui;

  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphWidget)
};

END_NAMESPACE_DISTRHO

#endif