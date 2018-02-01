#include "DistrhoUI.hpp"

#include "Graph.hpp"
#include "Window.hpp"
#include "Mathf.hpp"
//#include "VertexWidget.hpp"
#include "GraphWidget.hpp"
#include "WaveShaperUI.hpp"


START_NAMESPACE_DISTRHO

WaveShaperUI::WaveShaperUI() : UI(512, 512),
                               graphWidget(this, getParentWindow())
{
}

WaveShaperUI::~WaveShaperUI()
{
}

void WaveShaperUI::parameterChanged(uint32_t index, float value)
{
    parameters[index] = value;

    repaint();
}

void WaveShaperUI::stateChanged(const char *key, const char *value)
{
    if (std::strcmp(key, "graph") == 0)
        graphWidget.rebuildFromString(value);

    repaint();
}

void WaveShaperUI::onNanoDisplay()
{
}

void WaveShaperUI::uiIdle()
{
}

UI *createUI()
{
    return new WaveShaperUI();
}

END_NAMESPACE_DISTRHO