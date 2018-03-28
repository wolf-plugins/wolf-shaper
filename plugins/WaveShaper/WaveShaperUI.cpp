#include "DistrhoUI.hpp"

#include "WaveShaperUI.hpp"
#include "Window.hpp"
#include "Config.hpp"

#include <string>

#if defined(DISTRHO_OS_WINDOWS)
#include "windows.h"
#endif

START_NAMESPACE_DISTRHO

WaveShaperUI::WaveShaperUI() : UI(616, 651)
{
    const uint minWidth = 616;
    const uint minHeight = 651;

    const uint knobsLabelBoxWidth = 66;
    const uint knobsLabelBoxHeight = 21;
    
    WaveShaperConfig::load();

    tryRememberSize();

    fGraphWidgetSocket = new GraphWidgetSocket(this);

    fSwitchRemoveDC = new RemoveDCSwitch(this, Size<uint>(16, 16));
    fSwitchRemoveDC->setCallback(this);
    fSwitchRemoveDC->setId(paramRemoveDC);

    fSwitchBipolarMode = new BipolarModeSwitch(this, Size<uint>(16, 34));
    fSwitchBipolarMode->setCallback(this);
    fSwitchBipolarMode->setId(paramBipolarMode);

    fLabelsBoxBipolarMode = new GlowingLabelsBox(this, Size<uint>(34, 42));
    fLabelsBoxBipolarMode->setLabels({"UNI", "BI"});
    
    fButtonResetGraph = new ResetGraphButton(this, Size<uint>(16, 16));
    fButtonResetGraph->setCallback(this);

    fLabelWheelOversample = new LabelBox(this, Size<uint>(118, knobsLabelBoxHeight));
    fLabelWheelOversample->setText("OVERSAMPLE");

    fWheelOversample = new OversampleWheel(this, Size<uint>(67, 38));
    fWheelOversample->setCallback(this);
    fWheelOversample->setRange(0, 4);

    fLabelPreGain = new LabelBox(this, Size<uint>(knobsLabelBoxWidth, knobsLabelBoxHeight));
    fLabelPreGain->setText("PRE");

    fKnobPreGain = new VolumeKnob(this, Size<uint>(54, 54));
    fKnobPreGain->setCallback(this);
    fKnobPreGain->setRange(0.0f, 2.0f);
    fKnobPreGain->setId(paramPreGain);
    fKnobPreGain->setColor(Color(0, 200, 255, 255));

    fLabelWet = new LabelBox(this, Size<uint>(knobsLabelBoxWidth, knobsLabelBoxHeight));
    fLabelWet->setText("WET");

    fKnobWet = new VolumeKnob(this, Size<uint>(54, 54));
    fKnobWet->setCallback(this);
    fKnobWet->setRange(0.0f, 1.0f);
    fKnobWet->setId(paramWet);
    fKnobWet->setColor(Color(237, 237, 143, 255));

    fLabelPostGain = new LabelBox(this, Size<uint>(knobsLabelBoxWidth, knobsLabelBoxHeight));
    fLabelPostGain->setText("POST");

    fKnobPostGain = new VolumeKnob(this, Size<uint>(54, 54));
    fKnobPostGain->setCallback(this);
    fKnobPostGain->setRange(0.0f, 1.0f);
    fKnobPostGain->setId(paramPostGain);
    fKnobPostGain->setColor(Color(0, 255, 100, 255));

    fHandleResize = new ResizeHandle(this, Size<uint>(18, 18));
    fHandleResize->setCallback(this);
    fHandleResize->setAbsolutePos(getWidth() - fHandleResize->getWidth(), getHeight() - fHandleResize->getHeight());
    fHandleResize->setMinSize(minWidth, minHeight);

    positionWidgets();
}

WaveShaperUI::~WaveShaperUI()
{
}

void WaveShaperUI::tryRememberSize()
{
    int width, height;
    FILE *file;
    std::string tmpFileName = PLUGIN_NAME ".tmp";

#if defined(DISTRHO_OS_WINDOWS)
    CHAR tempPath[MAX_PATH + 1];

    GetTempPath(MAX_PATH + 1, tempPath);
    std::string path = std::string(tempPath) + tmpFileName;
    file = fopen(path.c_str(), "r");
#else
    file = fopen(("/tmp/" + tmpFileName).c_str(), "r");
#endif

    if (file == NULL)
        return;

    const int numberScanned = fscanf(file, "%d %d", &width, &height);

    if (numberScanned == 2 && width && height)
    {
        setSize(width, height);
    }

    fclose(file);
}

void WaveShaperUI::positionWidgets()
{
    //TODO: Clean that up

    const float knobLabelMarginBottom = 12;

    fSwitchRemoveDC->setAbsolutePos(30, getHeight() - 33);

    fSwitchBipolarMode->setAbsolutePos(60, getHeight() - 86);
    fLabelsBoxBipolarMode->setAbsolutePos(22, getHeight() - 90);

    fButtonResetGraph->setAbsolutePos(60, getHeight() - 33);

    float centerAlignDifference = (fLabelWheelOversample->getWidth() - fWheelOversample->getWidth()) / 2.0f;

    fWheelOversample->setAbsolutePos(160, getHeight() - 82);
    fLabelWheelOversample->setAbsolutePos(160 - centerAlignDifference, getHeight() - fLabelWheelOversample->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelPreGain->getWidth() - fKnobPreGain->getWidth()) / 2.0f;

    fKnobPreGain->setAbsolutePos(getWidth() - 280, getHeight() - 90);
    fLabelPreGain->setAbsolutePos(getWidth() - 280 - centerAlignDifference, getHeight() - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelWet->getWidth() - fKnobWet->getWidth()) / 2.0f;

    fKnobWet->setAbsolutePos(getWidth() - 200, getHeight() - 90);
    fLabelWet->setAbsolutePos(getWidth() - 200 - centerAlignDifference, getHeight() - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelPostGain->getWidth() - fKnobPostGain->getWidth()) / 2.0f;

    fKnobPostGain->setAbsolutePos(getWidth() - 120, getHeight() - 90);
    fLabelPostGain->setAbsolutePos(getWidth() - 120 - centerAlignDifference, getHeight() - fLabelPreGain->getHeight() - knobLabelMarginBottom);
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
        fGraphWidgetSocket->graphWidget.rebuildFromString(value);

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

bool WaveShaperUI::onKeyboard(const KeyboardEvent &ev)
{
    if (ev.press)
    {
        WaveShaperConfig::load();
        repaint();
    }

    return true;
}

void WaveShaperUI::nanoSwitchClicked(NanoSwitch *nanoSwitch)
{
    uint switchId = nanoSwitch->getId();

    parameters[switchId] = parameters[switchId] > 0.50f ? 0.0f : 1.0f;
    setParameterValue(switchId, parameters[switchId]);

    if (switchId == paramBipolarMode)
    {
        fLabelsBoxBipolarMode->setSelectedIndex((int)parameters[switchId]);
    }
}

void WaveShaperUI::nanoButtonClicked(NanoButton *nanoButton)
{
    if (nanoButton != fButtonResetGraph)
        return;

    fGraphWidgetSocket->graphWidget.reset();
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
    fGraphWidgetSocket->setSize(width, height);
    setSize(width, height);

    getParentWindow().saveSizeAtExit(true);
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