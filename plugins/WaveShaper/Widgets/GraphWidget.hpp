#ifndef SPOONIE_GRAPH_WIDGET_HPP_INCLUDED
#define SPOONIE_GRAPH_WIDGET_HPP_INCLUDED

#include "ObjectPool.hpp"
#include "DistrhoUI.hpp"
#include "Graph.hpp"
#include "GraphNodesLayer.hpp"
#include "Layout.hpp"

START_NAMESPACE_DISTRHO

class GraphVertex;
class GraphTensionHandle;
class GraphNode;
class WaveShaperUI;

class GraphWidget : public NanoWidget,
                    public IdleCallback
{
  friend class GraphNode;
  friend class GraphVertex;
  friend class GraphTensionHandle;
  friend class GraphNodesLayer;

public:
  GraphWidget(WaveShaperUI *ui);
  ~GraphWidget();

  /**
   * Recreate the graph according to a saved state.
   */
  void rebuildFromString(const char *serializedGraph);

  /**
   * Reset the graph back into its default state.
   */
  void reset();

  const Margin getMargin();
  void setMargin(const Margin margin);

protected:
  /**
   * DPF stuff
   */
  void onNanoDisplay() override;
  void onResize(const ResizeEvent &ev) override;
  bool onScroll(const ScrollEvent &ev) override;
  bool onMouse(const MouseEvent &ev) override;
  bool onMotion(const MotionEvent &ev) override;
  void idleCallback() override;

  /**
   * Unused.
   */
  void updateAnimations();

  /**
   * Flip the Y axis so that the origin of the graph is located at the bottom-left corner.
   */
  void flipYAxis();

  /**
   * Draw the grid of the graph.
   */
  void drawGrid();

  /**
   * Divide the cells on the grid into more cells.
   */
  void drawSubGrid();

  /**
   * Determine whether the edge at the right of the specified vertex needs to be emphasized.
   */
  bool edgeMustBeEmphasized(int vertexIndex);

  /**
   * Draw the background of the graph.
   */
  void drawBackground();

  /**
   * Draw the edge associated with the specified vertex in the graph.
   */
  void drawGraphEdge(int vertexIndex, float lineWidth, Color color);

  void drawInOutLabels();

  /**
   * Draw the line on the graph according to the current state of the plugin.
   */
  void drawGraphLine(float lineWidth, Color normalColor, Color emphasizedColor);

  /**
   * Draw lines to help the user align the currently grabbed node.
   */
  void drawAlignmentLines();

  /**
   * Draw a vertical line to indicate the input volume on the graph.
   */
  void drawInputIndicator();

  /**
   * Insert a new vertex into the graph at a specified location and return a pointer to it. 
   * The position is in absolute coordinates.
   */
  GraphVertex *insertVertex(const Point<int> pos);

  /**
   * Remove a vertex at the specified index.
   */
  void removeVertex(const int index);

  /**
   * Mouse handling stuff
   */
  bool leftClick(const MouseEvent &ev);
  bool middleClick(const MouseEvent &ev);
  bool rightClick(const MouseEvent &ev);

private:
  WaveShaperUI *ui;

  /**
   * Initialize the left and right vertices in the graph.
   */
  void initializeDefaultVertices();

  /**
   * Reset the object pool that contains the vertices of the graph.
   */
  void resetVerticesPool();

  GraphNode *getHoveredNode(Point<int> cursorPos);

  /**
   * The data structure that contains the graph. Kept synchronized with the dsp side of the plugin.
   */
  spoonie::Graph lineEditor;

  /**
   * The widget on which the nodes are rendered.
   */
  GraphNodesLayer graphNodesLayer;

  /**
   * Contains the vertex widgets used in the graph.
   */
  GraphVertex *graphVertices[spoonie::maxVertices];

  /**
   * Object pool that holds the vertex widgets.
   */
  spoonie::ObjectPool<GraphVertex> graphVerticesPool;

  /**
   * The graph element which currently has mouse focus.
   */
  GraphNode *focusedElement;

  /**
   * Determine if a mouse button is down.
   */
  bool mouseLeftDown;
  bool mouseRightDown;

  /**
   * The radius size of the vertices in the graph.
   */
  const float absoluteVertexSize = 7.0f;

  /**
   * Define the space around the grid of the graph.
   */
  /*const int marginTop = 36;
  const int marginLeft = 48;
  const int marginRight = 48;
  const int marginBottom = 84;*/

  Margin margin;

  float maxInput;
  float maxInputAcceleration = 0.0f;

  Size<uint> initialSize;
  
  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphWidget)
};

END_NAMESPACE_DISTRHO

#endif