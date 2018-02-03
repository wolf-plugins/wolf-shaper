#include "GraphNodesLayer.hpp"
#include "GraphWidget.hpp"
#include "GraphNode.hpp"

START_NAMESPACE_DISTRHO

GraphNodesLayer::GraphNodesLayer(GraphWidget *graph, Window &parent) : NanoWidget(parent),
                                                                       graph(graph)
{
}

GraphNodesLayer::~GraphNodesLayer()
{
}

void GraphNodesLayer::onNanoDisplay()
{
    const int vertexCount = graph->lineEditor.getVertexCount();
    
    for (int i = 0; i < vertexCount; ++i)
    {
        graph->graphVertices[i]->render();
    }
}

END_NAMESPACE_DISTRHO