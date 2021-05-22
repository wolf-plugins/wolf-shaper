#ifndef WOLF_GRAPH_WIDGET_HPP_INCLUDED
#define WOLF_GRAPH_WIDGET_HPP_INCLUDED

#include "ObjectPool.hpp"
#include "DistrhoUI.hpp"
#include "Graph.hpp"
#include "Margin.hpp"
#include "Widget.hpp"
#include "RightClickMenu.hpp"
#include "WolfWidget.hpp"

START_NAMESPACE_DISTRHO

class GraphVertex;
class GraphTensionHandle;
class GraphNode;
class GraphWidget;

enum class GraphGradientMode
{
  None = 0,
  Bottom
};

class GraphWidget: public WolfWidget,
                         public IdleCallback,
                         public RightClickMenu::Callback
{
  friend class GraphNode;
  friend class GraphVertex;
  friend class GraphTensionHandle;

public:
  GraphWidget(UI *ui, Size<uint> size);
  ~GraphWidget();

  /**
   * Recreate the graph according to a saved state.
   */
  void rebuildFromString(const char *serializedGraph);

  /**
   * Reset the graph back into its default state.
   */
  void reset();


  void updateInput(const float input);

  void setGraphGradientMode(GraphGradientMode graphGradientMode);

  void setHorizontalWarpAmount(const float warpAmount);
  void setHorizontalWarpType(const wolf::WarpType warpType);

  void setVerticalWarpAmount(const float warpAmount);
  void setVerticalWarpType(const wolf::WarpType warpType);

  void setMustHideVertices(const bool hide);

protected:
  enum GraphRightClickMenuItems
  {
    deleteNodeItem = 0,
    singlePowerCurveItem,
    doublePowerCurveItem,
    stairsCurveItem,
    waveCurveItem
  };

  /**
   * DPF stuff
   */
  void onNanoDisplay() override;
  void onResize(const ResizeEvent &ev) override;
  bool onScroll(const ScrollEvent &ev) override;
  bool onMouse(const MouseEvent &ev) override;
  bool onMotion(const MotionEvent &ev) override;
  void onFocusOut(); //override;

  void idleCallback() override;

  void rightClickMenuItemSelected(RightClickMenuItem *rightClickMenuItem);

  void onMouseLeave();

  /**
   * Unused.
   */
  void updateAnimations();

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
   * Draw a gradient envelope at the bottom of the graph's function.
   */
  void drawGradient();

  void positionGraphNodes();

  /**
   * Draw a vertical line to indicate the input volume on the graph.
   */
  void drawInputIndicator();

  void drawVertices();

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
  Margin fMargin;

  UI *ui;

  Point<int> projectCursorPos(Point<double> pt);

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
  wolf::Graph lineEditor;

  /**
   * Contains the vertex widgets used in the graph.
   */
  GraphVertex *graphVertices[wolf::maxVertices];

  /**
   * Object pool that holds the vertex widgets.
   */
  wolf::ObjectPool<GraphVertex> graphVerticesPool;

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
   * The type of gradient that should be drawn along with the graph's function.
   */
  GraphGradientMode graphGradientMode;

  /**
   * The radius size of the vertices in the graph.
   */
  const float absoluteVertexSize = 7.0f;

  /**
   * Determines whether or not the vertices in the graph must be hidden.
   */
  bool mustHideVertices;

  bool hovered;

  float maxInput;
  float maxInputAcceleration = 0.0f;

  float fInput;

  // ScopedPointer<RightClickMenu> fRightClickMenu;
  GraphNode *fNodeSelectedByRightClick;
  wolf::CurveType fLastCurveTypeSelected;

  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphWidget)
};

END_NAMESPACE_DISTRHO

#endif
