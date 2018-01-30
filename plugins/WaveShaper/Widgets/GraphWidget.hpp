#ifndef SPOONIE_GRAPH_WIDGET_HPP_INCLUDED
#define SPOONIE_GRAPH_WIDGET_HPP_INCLUDED

#include "Widget.hpp"
#include "Window.hpp"

#include "Graph.hpp"

#include "Mathf.hpp"
#include "GraphNodes.hpp"
#include "DistrhoUI.hpp"

START_NAMESPACE_DISTRHO

class GraphVertex;

class GraphWidget : public NanoWidget
{
  friend class GraphVertex;

public:
  GraphWidget(UI *ui, Window &parent);
  void rebuildFromString(const char *serializedGraph);

protected:
  void onNanoDisplay() override;
  bool onScroll(const ScrollEvent &ev) override;
  bool onMouse(const MouseEvent &ev) override;
  bool onMotion(const MotionEvent &ev) override;

  void updateAnimations();
  void flipY();
  void drawSubGrid();
  void drawGrid();
  void drawBackground();
  void drawGraphLine(float lineWidth, Color color);
  void drawVertex(int index);
  void drawTensionHandle(int index);
  void drawGraphVertices();
  void drawAlignmentLines();
  void insertVertex(DGL::Point<int> pos);
  bool leftClick(const MouseEvent &ev);
  bool middleClick(const MouseEvent &ev);
  bool rightClick(const MouseEvent &ev);

  void hoverCircle(DGL::Circle<int> *circle, int index)
  {
    hoveredWidget = circle;
    focusedWidgetIndex = index;
  }

  void expandCircle(DGL::Circle<int> *circle)
  {
    circle->setSize(absoluteVertexSize + 1.5f);

    repaint();
  }

  void shrinkCircle(DGL::Circle<int> *circle)
  {
    circle->setSize(absoluteVertexSize);

    repaint();
  }
  
  template <typename T>
  DGL::Point<T> flipY(const DGL::Point<T> point)
  {
    const T x = point.getX();
    const T y = getHeight() - point.getY();

    return DGL::Point<T>(x, y);
  }

private:
  bool mouseDown;
  Point<int> mouseDownLocation;

  spoonie::Graph lineEditor;

  //VertexWidget *uiVertices[spoonie::maxVertices];
  //VertexWidget uiVertex;

  GraphVertex *graphVertices[spoonie::maxVertices];

  DGL::Circle<int> vertexWidgets[spoonie::maxVertices];
  DGL::Circle<int> tensionWidgets[spoonie::maxVertices - 1];

  DGL::Circle<int> *hoveredWidget = nullptr;
  DGL::Circle<int> *grabbedWidget = nullptr;
  int focusedWidgetIndex = -1;

  const float absoluteVertexSize = 7.0f;
  UI *ui;

  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphWidget)
};

END_NAMESPACE_DISTRHO

#endif