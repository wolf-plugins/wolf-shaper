#include "DistrhoUI.hpp"

#include "WaveShaperUI.hpp"
#include "Window.hpp"
#include "Config.hpp"
#include "Layout.hpp"
#include "Fonts/chivo_bold.hpp"

#include <string>

#if defined(DISTRHO_OS_WINDOWS)
#include "windows.h"
#endif

START_NAMESPACE_DISTRHO

WaveShaperUI::WaveShaperUI() : UI(580, 615)
{
    const uint minWidth = 580;
    const uint minHeight = 615;

    const uint knobsLabelBoxWidth = 66;
    const uint knobsLabelBoxHeight = 21;

    using namespace SPOONIE_FONTS;
    NanoVG::FontId chivoBoldId = createFontFromMemory("chivo_bold", (const uchar *)chivo_bold, chivo_bold_size, 0);

    WaveShaperConfig::load();

    tryRememberSize();
    getParentWindow().saveSizeAtExit(true);

    const float width = getWidth();
    const float height = getHeight();

    fGraphWidget = new GraphWidget(this, Size<uint>(width - 4 * 2, height - 4 * 2 - 122));

    fSwitchRemoveDC = new RemoveDCSwitch(this, Size<uint>(30, 29));
    fSwitchRemoveDC->setCallback(this);
    fSwitchRemoveDC->setId(paramRemoveDC);

    fLabelRemoveDC = new NanoLabel(this, Size<uint>(50, 29));
    fLabelRemoveDC->setText("CENTER");

    fLabelRemoveDC->setFontId(chivoBoldId);
    fLabelRemoveDC->setFontSize(14.0f);
    fLabelRemoveDC->setAlign(ALIGN_LEFT | ALIGN_MIDDLE);
    fLabelRemoveDC->setMargin(Margin(3, 0, fSwitchRemoveDC->getWidth() / 2.0f, 0));

    fSwitchBipolarMode = new BipolarModeSwitch(this, Size<uint>(16, 34));
    fSwitchBipolarMode->setCallback(this);
    fSwitchBipolarMode->setId(paramBipolarMode);

    fLabelsBoxBipolarMode = new GlowingLabelsBox(this, Size<uint>(34, 42));
    fLabelsBoxBipolarMode->setLabels({"UNI", "BI"});

    fButtonResetGraph = new ResetGraphButton(this, Size<uint>(16, 16));
    fButtonResetGraph->setCallback(this);

    fLabelWheelOversample = new LabelBox(this, Size<uint>(118, knobsLabelBoxHeight));
    fLabelWheelOversample->setText("OVERSAMPLE");

    fWheelOversample = new OversampleWheel(this, Size<uint>(58, 34));
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
    fHandleResize->setMinSize(minWidth, minHeight);

    positionWidgets(width, height);
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

void WaveShaperUI::positionWidgets(uint width, uint height)
{
    //TODO: Clean that up
    const float graphMargin = 8;
    const float bottomBarSize = 102;

    fGraphWidget->setSize(width - graphMargin * 2, height - graphMargin * 2 - bottomBarSize);
    fGraphWidget->setAbsolutePos(graphMargin, graphMargin);

    const float knobLabelMarginBottom = 12;

    fSwitchRemoveDC->setAbsolutePos(24, height - 38);
    fLabelRemoveDC->setAbsolutePos(24 + fSwitchRemoveDC->getWidth(), height - 38);

    fSwitchBipolarMode->setAbsolutePos(31, height - 86);
    fLabelsBoxBipolarMode->setAbsolutePos(53, height - 90);

    fButtonResetGraph->setAbsolutePos(200, height - 33);

    float centerAlignDifference = (fLabelWheelOversample->getWidth() - fWheelOversample->getWidth()) / 2.0f;

    fWheelOversample->setAbsolutePos(160, height - 82);
    fLabelWheelOversample->setAbsolutePos(160 - centerAlignDifference, height - fLabelWheelOversample->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelPreGain->getWidth() - fKnobPreGain->getWidth()) / 2.0f;

    fKnobPreGain->setAbsolutePos(width - 280, height - 90);
    fLabelPreGain->setAbsolutePos(width - 280 - centerAlignDifference, height - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelWet->getWidth() - fKnobWet->getWidth()) / 2.0f;

    fKnobWet->setAbsolutePos(width - 200, height - 90);
    fLabelWet->setAbsolutePos(width - 200 - centerAlignDifference, height - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelPostGain->getWidth() - fKnobPostGain->getWidth()) / 2.0f;

    fKnobPostGain->setAbsolutePos(width - 120, height - 90);
    fLabelPostGain->setAbsolutePos(width - 120 - centerAlignDifference, height - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    fHandleResize->setAbsolutePos(width - fHandleResize->getWidth(), height - fHandleResize->getHeight());
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
        fSwitchRemoveDC->setDown(value >= 0.50f);
        break;
    case paramOversample:
        fWheelOversample->setValue(value);
        break;
    case paramBipolarMode:
    {
        const bool down = value >= 0.50f;

        fSwitchBipolarMode->setDown(down);
        fLabelsBoxBipolarMode->setSelectedIndex(down ? 1 : 0);
        break;
    }
    case paramOut:
        fGraphWidget->updateInput(value);
        break;
    default:
        break;
    }

    repaint();
}

void WaveShaperUI::stateChanged(const char *key, const char *value)
{
    if (std::strcmp(key, "graph") == 0)
        fGraphWidget->rebuildFromString(value);

    repaint();
}

void WaveShaperUI::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    //background
    beginPath();

    rect(0.f, 0.f, width, height);
    fillColor(WaveShaperConfig::plugin_background);

    fill();

    closePath();

    //graph shadow at the bottom
    beginPath();

    const int shadowHeight = 8;
    const int shadowMargin = 2;

    const float graphBottom = fGraphWidget->getAbsoluteY() + fGraphWidget->getHeight();
    const float shadowTop = graphBottom + shadowMargin;
    const float shadowBottom = shadowTop + shadowHeight;

    rect(0.0f, shadowTop, getWidth(), shadowHeight);

    Paint gradient = linearGradient(0, shadowTop, 0, shadowBottom, Color(21, 22, 30, 0), Color(21, 22, 30, 180));
    fillPaint(gradient);

    fill();

    closePath();
}

void WaveShaperUI::uiIdle()
{
}

void WaveShaperUI::uiReshape(uint width, uint height)
{
    //setSize(width, height);
    positionWidgets(width, height);
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
    const uint switchId = nanoSwitch->getId();
    const int value = nanoSwitch->isDown() ? 1 : 0;

    setParameterValue(switchId, value);

    if (switchId == paramBipolarMode)
    {
        fLabelsBoxBipolarMode->setSelectedIndex(value);
    }
}

void WaveShaperUI::nanoButtonClicked(NanoButton *nanoButton)
{
    if (nanoButton != fButtonResetGraph)
        return;

    fGraphWidget->reset();
}

void WaveShaperUI::nanoWheelValueChanged(NanoWheel *nanoWheel, const int value)
{
    if (nanoWheel != fWheelOversample)
        return;

    setParameterValue(paramOversample, value);
}

void WaveShaperUI::nanoKnobValueChanged(NanoKnob *nanoKnob, const float value)
{
    const uint id = nanoKnob->getId();

    setParameterValue(id, value);
}

void WaveShaperUI::resizeHandleMoved(int width, int height)
{
    setSize(width, height);
}

UI *createUI()
{
    return new WaveShaperUI();
}

END_NAMESPACE_DISTRHO