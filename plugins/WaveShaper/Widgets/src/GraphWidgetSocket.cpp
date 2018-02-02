#include "Widget.hpp"
#include "Window.hpp"

#include "Graph.hpp"
#include "GraphWidget.hpp"
#include "GraphWidgetSocket.hpp"

START_NAMESPACE_DISTRHO

GraphWidgetSocket::GraphWidgetSocket(WaveShaperUI *ui, Window &parent)
    : NanoWidget(parent),
      ui(ui),
      graphWidget(ui, getParentWindow())
{
}

GraphWidgetSocket::~GraphWidgetSocket()
{
}

void GraphWidgetSocket::onNanoDisplay() {}
bool GraphWidgetSocket::onScroll(const ScrollEvent &ev) {}
bool GraphWidgetSocket::onMouse(const MouseEvent &ev) {}
bool GraphWidgetSocket::onMotion(const MotionEvent &ev) {}

END_NAMESPACE_DISTRHO