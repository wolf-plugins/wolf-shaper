#ifndef SPOONIE_GRAPH_NODES_LAYER_HPP_INCLUDED
#define SPOONIE_GRAPH_NODES_LAYER_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class GraphWidget;

class GraphNodesLayer : public NanoWidget
{
public:
  GraphNodesLayer(GraphWidget *graph);
  ~GraphNodesLayer();

protected:
  void onNanoDisplay() override;

private:
  GraphWidget *graph;
  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphNodesLayer)
};

END_NAMESPACE_DISTRHO

#endif