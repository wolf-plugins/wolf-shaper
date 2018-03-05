#ifndef SPOONIE_GRAPH_WIDGET_SOCKET_HPP_INCLUDED
#define SPOONIE_GRAPH_WIDGET_SOCKET_HPP_INCLUDED

#include "GraphWidget.hpp"

START_NAMESPACE_DISTRHO

class WaveShaperUI;

class GraphWidgetSocket : public NanoWidget
{
  friend class WaveShaperUI;

public:
  GraphWidgetSocket(WaveShaperUI *ui);
  ~GraphWidgetSocket();

protected:
  void onResize(const ResizeEvent &) override;
  void onNanoDisplay() override;

private:
  WaveShaperUI *ui;
  GraphWidget graphWidget;

  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphWidgetSocket)
};

END_NAMESPACE_DISTRHO

#endif