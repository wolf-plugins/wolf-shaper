#include "DistrhoUI.hpp"

#include "WaveShaperUI.hpp"

START_NAMESPACE_DISTRHO

WaveShaperUI::WaveShaperUI() : UI(600, 600),
                               graphWidgetSocket(this, getParentWindow())
{

}

WaveShaperUI::~WaveShaperUI()
{
}

void WaveShaperUI::parameterChanged(uint32_t index, float value)
{
    /*switch (index)
    {
    case paramPreGain:
        fKnobPre->setValue(value);
        break;
    case paramWet:
        fKnobDryWet->setValue(value);
        break;
    case paramPostGain:
        fKnobPost->setValue(value);
        break;
    }*/

    parameters[index] = value;
}

void WaveShaperUI::stateChanged(const char *key, const char *value)
{
    if (std::strcmp(key, "graph") == 0)
        graphWidgetSocket.graphWidget.rebuildFromString(value);

    repaint();
}

void WaveShaperUI::onNanoDisplay()
{
}

void WaveShaperUI::uiIdle()
{

}

void WaveShaperUI::imageButtonClicked(ImageButton *button, int)
{
    /*if (button != fButtonRemoveDC)
        return;

    setParameterValue(button->getId(), 1.0f);*/
}

void WaveShaperUI::imageKnobDragStarted(ImageKnob *knob)
{
    //editParameter(knob->getId(), true);
}

void WaveShaperUI::imageKnobDragFinished(ImageKnob *knob)
{
    //editParameter(knob->getId(), false);
}

void WaveShaperUI::imageKnobValueChanged(ImageKnob *knob, float value)
{
    //setParameterValue(knob->getId(), value);
}

float WaveShaperUI::getParameterValue(uint32_t index) const
{
    return parameters[index];
}

UI *createUI()
{
    return new WaveShaperUI();
}

END_NAMESPACE_DISTRHO