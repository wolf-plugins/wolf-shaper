#ifndef WOLF_GRAPH_DEFINED_H
#define WOLF_GRAPH_DEFINED_H

#include "src/DistrhoDefines.h"

START_NAMESPACE_DISTRHO

namespace wolf
{
/**
   * The max number of vertices that can be in the graph at the same time.
   */
const int maxVertices = 99;

enum CurveType
{
  SingleCurve = 0,
  DoubleCurve,
  StairsCurve,
  WaveCurve
};

enum WarpType
{
  None = 0,
  BendPlus,
  BendMinus,
  BendPlusMinus,
  SkewPlus,
  SkewMinus,
  SkewPlusMinus
};

class Graph;

class Vertex
{
public:
  friend class Graph;

  float getX();
  float getY();
  float getTension() const;
  CurveType getType() const;

  void setX(float x);
  void setY(float y);
  void setPosition(float x, float y);
  void setTension(float tension);
  void setType(CurveType type);
  void setGraphPtr(Graph *graphPtr);

protected:
  Vertex();
  Vertex(float posX, float posY, float tension, CurveType type, Graph *graphPtr);

  float warpCoordinate(const float coordinate, const float warpAmount, const WarpType warpType) const;
  float unwarpCoordinate(float coordinate, const float warpAmount, const WarpType warpType) const;

private:
  float x;
  float y;
  bool xDirty;
  bool yDirty;
  float tension;
  float hWarp;
  float vWarp;
  float graphHWarp;
  float graphVWarp;
  WarpType graphHType;
  WarpType graphVType;
  CurveType type;

  Graph *graphPtr;
};

class Graph
{
public:
  Graph();

  void insertVertex(float x, float y, float tension = 0.0f, CurveType type = CurveType::SingleCurve);
  void removeVertex(int index);
  Vertex *getVertexAtIndex(int index);

  void setTensionAtIndex(int index, float tension);

  /**
   * Return the number of vertices contained in the graph.
   */
  int getVertexCount();

  static float getOutValue(float input, float tension, float p1x, float p1y, float p2x, float p2y, CurveType type);

  /**
   * Get the y value at x in the graph. 
   */
  float getValueAt(float x);

  /**
   * Empty the graph.
   */
  void clear();

  /**
   * Save the graph into a string.
   */
  const char *serialize();

  bool getBipolarMode();
  void setBipolarMode(bool bipolarMode);

  /**
   * warp getter/setters
   */
  //-------------------------------------------
  void setHorizontalWarpAmount(float warp);
  float getHorizontalWarpAmount() const;

  void setVerticalWarpAmount(float warp);
  float getVerticalWarpAmount() const;

  void setHorizontalWarpType(WarpType warpType);
  WarpType getHorizontalWarpType() const;

  void setVerticalWarpType(WarpType warpType);
  WarpType getVerticalWarpType() const;
  //-------------------------------------------

  /**
   * Rebuild the graph from a string.
   */
  void rebuildFromString(const char *serializedGraph);

private:
  Vertex vertices[maxVertices];
  int vertexCount;

  float horizontalWarpAmount;
  float verticalWarpAmount;

  WarpType horizontalWarpType;
  WarpType verticalWarpType;

  bool bipolarMode;

  //format: x,y,tension,type;
  char serializationBuffer[(sizeof(char) * 256 + 4) * maxVertices + 1];
};

} // namespace wolf

END_NAMESPACE_DISTRHO

#endif