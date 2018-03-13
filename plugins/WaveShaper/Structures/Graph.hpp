#ifndef SPOONIE_GRAPH_DEFINED_H
#define SPOONIE_GRAPH_DEFINED_H

namespace spoonie
{
  /**
   * The max number of vertices that can be in the graph at the same time.
   */
const int maxVertices = 99;

enum CurveType //TODO: implement more curve types
{
  Exponential
};

struct Vertex
{
  float x;
  float y;
  float tension;
  CurveType type;

  Vertex();
  Vertex(float posX, float posY, float tension = 0.0f, CurveType type = CurveType::Exponential);

  void setPosition(float x, float y);
};

class Graph
{
public:
  Graph();

  void insertVertex(float x, float y, float tension = 0.0f, CurveType type = CurveType::Exponential);
  void removeVertex(int index);
  Vertex* getVertexAtIndex(int index);

  void setTensionAtIndex(int index, float tension);

  /**
   * Return the number of vertices contained in the graph.
   */
  int getVertexCount();

  static float getOutValue(float input, float tension, float p1x, float p1y, float p2x, float p2y);

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
  const char* serialize();

  bool getBipolarMode();
  void setBipolarMode(bool bipolarMode);

  /**
   * Rebuild the graph from a string.
   */
  void rebuildFromString(const char* serializedGraph);
  
private:
  Vertex vertices[maxVertices];
  int vertexCount;

  bool bipolarMode;

  //format: x,y,tension,type;
  char serializationBuffer[(sizeof(char) * 256 + 4) * maxVertices + 1];
};

}

#endif