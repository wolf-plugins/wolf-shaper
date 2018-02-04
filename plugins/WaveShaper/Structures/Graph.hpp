#ifndef SPOONIE_GRAPH_DEFINED_H
#define SPOONIE_GRAPH_DEFINED_H

namespace spoonie
{
const int maxVertices = 99;

enum CurveType
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

  int getVertexCount();

  static float getOutValueUnipolar(float input, float tension, float p1x, float p1y, float p2x, float p2y);
  float getValueAt(float x);

  void clear();

  const char* serialize();
  void rebuildFromString(const char* serializedGraph);
  
private:
  Vertex vertices[maxVertices];
  int vertexCount;
  
  //format: x,y,tension,type;
  char serializationBuffer[(sizeof(char) * 256 + 4) * maxVertices + 1];
};

}

#endif