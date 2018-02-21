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

    fWheelOversample = new OversampleWheel(getParentWindow(), Size<uint>(36, 20));
    fWheelOversample->setCallback(this);
    fWheelOversample->setAbsolutePos(90, 562);
    fWheelOversample->setRange(0, 4);

    fKnobPreGain = new VolumeKnob(getParentWindow(), Size<uint>(48, 48));
    fKnobPreGain->setCallback(this);
    fKnobPreGain->setAbsolutePos(280, 545);
    fKnobPreGain->setRange(0.0f, 2.0f);
    fKnobPreGain->setId(paramPreGain);
    fKnobPreGain->setColor(Color(255,50,50,255));

    fKnobWet = new VolumeKnob(getParentWindow(), Size<uint>(48, 48));
    fKnobWet->setCallback(this);
    fKnobWet->setAbsolutePos(350, 545);
    fKnobWet->setRange(0.0f, 1.0f);
    fKnobWet->setId(paramWet);
    fKnobWet->setColor(Color(0,200,255,255));

    fKnobPostGain = new VolumeKnob(getParentWindow(), Size<uint>(48, 48));
    fKnobPostGain->setCallback(this);
    fKnobPostGain->setAbsolutePos(420, 545);
    fKnobPostGain->setRange(0.0f, 1.0f);
    fKnobPostGain->setId(paramPostGain);
    fKnobPostGain->setColor(Color(0,255,100,255));
}

WaveShaperUI::~WaveShaperUI()
{
}

void WaveShaperUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case paramPreGain:
        fKnobPreGain->setValue(value);
        break;
    case paramWet:
        fKnobWet->setValue(value);
        break;
    case paramPostGain:
        fKnobPostGain->setValue(value);
        break;
    case paramRemoveDC:
        fSwitchRemoveDC->setDown(value > 0.50f);
        break;
    case paramOversample:
        fWheelOversample->setValue(value);
        break;
    case paramBipolarMode:
        break;
    default:
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

void WaveShaperUI::nanoWheelValueChanged(NanoWheel *nanoWheel, const int value)
{
    if (nanoWheel != fWheelOversample)
        return;

    //parameters[paramOversample] = value;
    setParameterValue(paramOversample, value);
}

void WaveShaperUI::nanoKnobValueChanged(NanoKnob *nanoKnob, const float value)
{
    const int id = nanoKnob->getId();

    parameters[id] = value;
    setParameterValue(id, value);
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