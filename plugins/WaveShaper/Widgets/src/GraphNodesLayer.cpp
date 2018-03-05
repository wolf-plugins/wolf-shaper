#include "GraphNodesLayer.hpp"
#include "GraphWidget.hpp"
#include "GraphNode.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

GraphNodesLayer::GraphNodesLayer(GraphWidget *graph) : NanoWidget(graph->getParentWindow()),
                                                       graph(graph)
{
    Window &window = getParentWindow();
    setSize(window.getWidth(), window.getHeight());
}

GraphNodesLayer::~GraphNodesLayer()
{
}

void GraphNodesLayer::onNanoDisplay()
{
    const int vertexCount = graph->lineEditor.getVertexCount();

    for (int i = 0; i < vertexCount; ++i)
    {
        GraphVertex *vertex = graph->graphVertices[i];

        vertex->getTensionHandle()->render();
        vertex->render();
    }
}

END_NAMESPACE_DISTRHO