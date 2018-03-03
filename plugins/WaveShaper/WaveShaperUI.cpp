#include "DistrhoUI.hpp"

#include "WaveShaperUI.hpp"
#include "Window.hpp"
#include "Config.hpp"

START_NAMESPACE_DISTRHO

WaveShaperUI::WaveShaperUI() : UI(450, 450),
                               graphWidgetSocket(this, getParentWindow())
{
    WaveShaperConfig::load();

    fSwitchRemoveDC = new RemoveDCSwitch(this, Size<uint>(16, 16));
    fSwitchRemoveDC->setCallback(this);
    fSwitchRemoveDC->setId(paramRemoveDC);

    fButtonResetGraph = new ResetGraphButton(getParentWindow(), Size<uint>(16, 16));
    fButtonResetGraph->setCallback(this);

    fWheelOversample = new OversampleWheel(getParentWindow(), Size<uint>(36, 20));
    fWheelOversample->setCallback(this);
    fWheelOversample->setRange(0, 4);

    fKnobPreGain = new VolumeKnob(getParentWindow(), Size<uint>(48, 48));
    fKnobPreGain->setCallback(this);
    fKnobPreGain->setRange(0.0f, 2.0f);
    fKnobPreGain->setId(paramPreGain);
    fKnobPreGain->setColor(Color(255,50,50,255));

    fKnobWet = new VolumeKnob(getParentWindow(), Size<uint>(48, 48));
    fKnobWet->setCallback(this);
    fKnobWet->setRange(0.0f, 1.0f);
    fKnobWet->setId(paramWet);
    fKnobWet->setColor(Color(0,200,255,255));

    fKnobPostGain = new VolumeKnob(getParentWindow(), Size<uint>(48, 48));
    fKnobPostGain->setCallback(this);
    fKnobPostGain->setRange(0.0f, 1.0f);
    fKnobPostGain->setId(paramPostGain);
    fKnobPostGain->setColor(Color(0,255,100,255));

    fHandleResize = new ResizeHandle(getParentWindow(), Size<uint>(24,24));
    fHandleResize->setCallback(this);
    fHandleResize->setAbsolutePos(getWidth() - fHandleResize->getWidth(), getHeight() - fHandleResize->getHeight());
    fHandleResize->setMinSize(450, 450);

    positionWidgets();
}

WaveShaperUI::~WaveShaperUI()
{
}

void WaveShaperUI::positionWidgets() 
{
    fSwitchRemoveDC->setAbsolutePos(30, getHeight() - 43);
    fButtonResetGraph->setAbsolutePos(60, getHeight() - 43);
    fWheelOversample->setAbsolutePos(90, getHeight() - 44);
    fKnobPreGain->setAbsolutePos(getWidth() - 250, getHeight() - 55);
    fKnobWet->setAbsolutePos(getWidth() - 180, getHeight() - 55);
    fKnobPostGain->setAbsolutePos(getWidth() - 110, getHeight() - 55);
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

void WaveShaperUI::uiReshape(uint, uint)
{
    positionWidgets();
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

void WaveShaperUI::resizeHandleMoved(int width, int height)
{
    graphWidgetSocket.setSize(width, height);
    setSize(width, height);
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