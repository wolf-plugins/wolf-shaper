#include "DistrhoUI.hpp"

#include "WaveShaperUI.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

WaveShaperUI::WaveShaperUI() : UI(600, 600),
                               graphWidgetSocket(this, getParentWindow())
{
    fSwitchRemoveDC = new RemoveDCSwitch(this, Size<uint>(16, 16));
    fSwitchRemoveDC->setCallback(this);
    fSwitchRemoveDC->setAbsolutePos(30, 562);
    fSwitchRemoveDC->setId(paramRemoveDC);

    fButtonResetGraph = new ResetGraphButton(getParentWindow(), Size<uint>(16, 16));
    fButtonResetGraph->setCallback(this);
    fButtonResetGraph->setAbsolutePos(60, 562);

    fWheelOversample = new OversampleWheel(getParentWindow(), Size<uint>(24, 20));
    fWheelOversample->setCallback(this);
    fWheelOversample->setAbsolutePos(90, 562);
    fWheelOversample->setRange(0, 4);
    /*fDesignWindow = new Window(getParentApp(), getParentWindow());
     fDesignWindow->setSize(412, 412);
     fDesignWindow->show();*/
}

WaveShaperUI::~WaveShaperUI()
{
    //fDesignWindow->close();
}

void WaveShaperUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case paramPreGain:
        //fKnobPre->setValue(value);
        break;
    case paramWet:
        //fKnobDryWet->setValue(value);
        break;
    case paramPostGain:
        //fKnobPost->setValue(value);
        break;
    case paramRemoveDC:
        fSwitchRemoveDC->setDown(value > 0.50f);
        break;
    case paramOversample:
        fWheelOversample->setValue(value);
        break;
    }

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

void WaveShaperUI::nanoSwitchClicked(NanoSwitch *nanoSwitch)
{
    uint switchId = nanoSwitch->getId();

    if (nanoSwitch == fSwitchRemoveDC)
    {
        parameters[switchId] = parameters[switchId] > 0.50f ? 0.0f : 1.0f;

        setParameterValue(switchId, parameters[switchId]);
    }
}

void WaveShaperUI::nanoButtonClicked(NanoButton *nanoButton)
{
    if (nanoButton != fButtonResetGraph)
        return;

    graphWidgetSocket.graphWidget.reset();
}

void WaveShaperUI::nanoWheelValueChanged(NanoWheel *nanoWheel, int value) 
{
    if(nanoWheel != fWheelOversample)
        return;

    fprintf(stderr, "%d\n", value);
    setParameterValue(paramOversample, value);
}

/* void WaveShaperUI::imageKnobDragStarted(ImageKnob *knob)
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
} */

float WaveShaperUI::getParameterValue(uint32_t index) const
{
    return parameters[index];
}

UI *createUI()
{
    return new WaveShaperUI();
}

END_NAMESPACE_DISTRHO