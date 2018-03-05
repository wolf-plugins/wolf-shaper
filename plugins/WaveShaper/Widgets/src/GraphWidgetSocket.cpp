#include "Widget.hpp"
#include "Window.hpp"

#include "Graph.hpp"
#include "GraphWidget.hpp"
#include "GraphWidgetSocket.hpp"
#include "WaveShaperUI.hpp"
#include "Config.hpp"

START_NAMESPACE_DISTRHO

GraphWidgetSocket::GraphWidgetSocket(WaveShaperUI *ui)
    : NanoWidget((NanoWidget*)ui),
      ui(ui),
      graphWidget(ui)
{
  setSize(ui->getWidth(), ui->getHeight());
}

GraphWidgetSocket::~GraphWidgetSocket()
{
}

void GraphWidgetSocket::onResize(const ResizeEvent &)
{
  const Margin margin = graphWidget.getMargin();

  const int width = ui->getWidth() - margin.left - margin.right;
  const int height = ui->getHeight() - margin.top - margin.bottom;

  graphWidget.setSize(width, height);
}

void GraphWidgetSocket::onNanoDisplay()
{
  const float width = getWidth();
  const float height = getHeight();

  const float centerX = width / 2.0f;
  //const float centerY = height / 2.0f;

  const int graphLeft = graphWidget.getAbsoluteX();
  //const int graphRight = graphLeft + graphWidget.getWidth();

  const int graphTop = graphWidget.getAbsoluteY();
  const int graphBottom = graphWidget.getAbsoluteY() + graphWidget.getHeight();

  //background
  beginPath();

  rect(0.f, 0.f, width, height);
  fillColor(WaveShaperConfig::plugin_background);

  fill();

  closePath();

  //graph margin
  beginPath();

  const int marginSize = 16;

  const int marginLeft = graphLeft - marginSize;
  const int marginTop = graphTop - marginSize;
  const int marginWidth = graphWidget.getWidth() + marginSize * 2;
  const int marginHeight = graphWidget.getHeight() + marginSize * 2;

  rect(marginLeft, marginTop, marginWidth, marginHeight);

  fillColor(WaveShaperConfig::graph_margin);

  fill();

  closePath();

  //graph shadow at the bottom
  beginPath();

  const int graphBottomShadowHeight = 8;

  rect(0.0f, graphBottom + marginSize, getWidth(), graphBottomShadowHeight);

  Paint gradient = linearGradient(centerX, graphBottom + marginSize, centerX, graphBottom + marginSize + graphBottomShadowHeight, Color(21, 22, 30, 0), Color(21, 22, 30, 255));
  fillPaint(gradient);

  fill();

  closePath();

  //lines on the background
  /*beginPath();

    const int lineSpacing = 4;

    for(uint i = 0; i < getHeight() / lineSpacing; ++i) 
    {
          moveTo(0.0f, i * lineSpacing);
          lineTo(width, i * lineSpacing);
    }
       
    gradient = linearGradient(centerX, 0.0f, centerX, height, Color(27,32,36,255), Color(24,29,23,0));
    strokePaint(gradient);
    stroke();

    closePath();*/

  //graph margin outline
  beginPath();

  rect(marginLeft, marginTop, marginWidth, marginHeight);
  strokeColor(WaveShaperConfig::side_borders);

  scissor(marginLeft, marginTop + 1, marginWidth, marginHeight - 1);
  stroke();
  resetScissor();

  closePath();

  beginPath();

  moveTo(marginLeft, marginTop);
  lineTo(marginLeft + marginWidth, marginTop);

  strokeColor(WaveShaperConfig::top_border);

  stroke();

  closePath();
}

END_NAMESPACE_DISTRHO