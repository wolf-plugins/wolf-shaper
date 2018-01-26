#include "DistrhoUI.hpp"

#include "Graph.hpp"
#include "Window.hpp"
#include "Mathf.hpp"
//#include "VertexWidget.hpp"
#include "GraphWidget.hpp"

START_NAMESPACE_DISTRHO

class WaveShaperUI : public UI
{
    enum Parameters
    {
        paramPreGain = 0,
        paramWet,
        paramPostGain,
        paramRemoveDC,
        paramOversample,
        paramBipolarMode,
        paramCount
    };

  public:
    WaveShaperUI() : UI(512, 512),
                     graphWidget(this, getParentWindow())
    {
    }

  protected:
    void parameterChanged(uint32_t index, float value) override
    {
        parameters[index] = value;

        repaint();
    }

    void stateChanged(const char *key, const char *value) override
    {
        if (std::strcmp(key, "graph") == 0)
            graphWidget.rebuildFromString(value);

        repaint();
    }

    void onNanoDisplay() override
    {
    }

    void uiIdle() override 
    {
        graphWidget.updateAnimations();
    }

  private:
    bool parameters[paramCount];

    bool mouseDown;
    Point<int> mouseDownLocation;

    GraphWidget graphWidget;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveShaperUI)
};

UI *createUI()
{
    return new WaveShaperUI();
}

END_NAMESPACE_DISTRHO