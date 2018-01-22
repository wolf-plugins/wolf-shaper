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

    void programLoaded(uint32_t index) override
    {
        switch (index)
        {
        case 0:
            break;
        case 1:
            break;
        }

        repaint();
    }

    void stateChanged(const char *key, const char *value) override
    {
        if (std::strcmp(key, "graph") == 0)
            lineEditor.rebuildFromString(value);

        repaint();
    }

    void onNanoDisplay() override
    {
    }

  private:
    bool parameters[paramCount];

    bool mouseDown;
    Point<int> mouseDownLocation;

    spoonie::Graph lineEditor;

    GraphWidget graphWidget;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveShaperUI)
};

UI *createUI()
{
    return new WaveShaperUI();
}

END_NAMESPACE_DISTRHO