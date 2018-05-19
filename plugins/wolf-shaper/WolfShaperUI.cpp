#include "DistrhoUI.hpp"

#include "WolfShaperUI.hpp"
#include "Window.hpp"
#include "Config.hpp"
#include "Layout.hpp"
#include "Fonts/chivo_bold.hpp"

#include <string>

#if defined(DISTRHO_OS_WINDOWS)
#include "windows.h"
#endif

START_NAMESPACE_DISTRHO

WolfShaperUI::WolfShaperUI() : UI(620, 655)
{
    const uint minWidth = 526;
    const uint minHeight = 634;

    const uint knobsLabelBoxWidth = 66;
    const uint knobsLabelBoxHeight = 21;

    loadSharedResources();

    using namespace WOLF_FONTS;
    NanoVG::FontId chivoBoldId = createFontFromMemory("chivo_bold", (const uchar *)chivo_bold, chivo_bold_size, 0);
    NanoVG::FontId dejaVuSansId = findFont(NANOVG_DEJAVU_SANS_TTF);

    WolfShaperConfig::load();

    tryRememberSize();
    getParentWindow().saveSizeAtExit(true);

    const float width = getWidth();
    const float height = getHeight();

    fGraphWidget = new GraphWidget(this, Size<uint>(width - 4 * 2, height - 4 * 2 - 122));

    const float graphBarHeight = 42;

    fGraphBar = new WidgetBar(this, Size<uint>(width, graphBarHeight));
    fGraphBar->setStrokePaint(linearGradient(0, 0, 0, graphBarHeight, Color(43, 43, 43, 255), Color(34, 34, 34, 255)));
    fGraphBar->setStrokeWidth(4.0f);

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
    fKnobPreGain->setColor(Color(255, 197, 246, 255));

    fLabelWet = new LabelBox(this, Size<uint>(knobsLabelBoxWidth, knobsLabelBoxHeight));
    fLabelWet->setText("WET");

    fKnobWet = new VolumeKnob(this, Size<uint>(54, 54));
    fKnobWet->setCallback(this);
    fKnobWet->setRange(0.0f, 1.0f);
    fKnobWet->setId(paramWet);
    fKnobWet->setColor(Color(136,228,255));

    fLabelPostGain = new LabelBox(this, Size<uint>(knobsLabelBoxWidth, knobsLabelBoxHeight));
    fLabelPostGain->setText("POST");

    fKnobPostGain = new VolumeKnob(this, Size<uint>(54, 54));
    fKnobPostGain->setCallback(this);
    fKnobPostGain->setRange(0.0f, 1.0f);
    fKnobPostGain->setId(paramPostGain);
    fKnobPostGain->setColor(Color(143, 255, 147, 255));

    fKnobWarp = new VolumeKnob(this, Size<uint>(54, 54));
    fKnobWarp->setCallback(this);
    fKnobWarp->setRange(0.0f, 1.0f);
    fKnobWarp->setId(paramWarpAmount);
    fKnobWarp->setColor(Color(255, 225, 169, 255));

    fLabelListWarpType = new LabelBoxList(this, Size<uint>(knobsLabelBoxWidth, knobsLabelBoxHeight));
    fLabelListWarpType->setLabels({"NONE", "BEND +", "BEND -", "BEND +/-", "SKEW +", "SKEW -", "SKEW +/-"});

    fButtonLeftArrow = new ArrowButton(this, Size<uint>(knobsLabelBoxHeight, knobsLabelBoxHeight));
    fButtonLeftArrow->setCallback(this);
    fButtonLeftArrow->setId(paramWarpType);
    fButtonLeftArrow->setArrowDirection(ArrowButton::Left);

    fButtonRightArrow = new ArrowButton(this, Size<uint>(knobsLabelBoxHeight, knobsLabelBoxHeight));
    fButtonRightArrow->setCallback(this);
    fButtonRightArrow->setId(paramWarpType);
    fButtonRightArrow->setArrowDirection(ArrowButton::Right);

    fHandleResize = new ResizeHandle(this, Size<uint>(18, 18));
    fHandleResize->setCallback(this);
    fHandleResize->setMinSize(minWidth, minHeight);

    fButtonResetGraph = new ResetGraphButton(this, Size<uint>(32, 32));
    fButtonResetGraph->setCallback(this);

    fLabelButtonResetGraph = new NanoLabel(this, Size<uint>(50, fButtonResetGraph->getHeight()));
    fLabelButtonResetGraph->setText("RESET");
    fLabelButtonResetGraph->setFontId(dejaVuSansId);
    fLabelButtonResetGraph->setFontSize(15.0f);
    fLabelButtonResetGraph->setAlign(ALIGN_LEFT | ALIGN_MIDDLE);
    fLabelButtonResetGraph->setMargin(Margin(6, 0, std::round(fButtonResetGraph->getHeight() / 2.0f) + 1, 0));

    positionWidgets(width, height);
}

WolfShaperUI::~WolfShaperUI()
{
}

void WolfShaperUI::tryRememberSize()
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

void WolfShaperUI::positionWidgets(uint width, uint height)
{
    //TODO: Clean that up

    const float graphMargin = 8;
    const float bottomBarSize = 102;
    const float graphBarHeight = fGraphBar->getHeight();
    const float graphBarMargin = 6;

    fGraphWidget->setSize(width - graphMargin * 2, height - graphMargin * 2 - bottomBarSize - graphBarHeight);
    fGraphWidget->setAbsolutePos(graphMargin, graphMargin);

    const float graphBottom = fGraphWidget->getAbsoluteY() + fGraphWidget->getHeight();

    fGraphBar->setWidth(width);
    fGraphBar->setAbsolutePos(0, graphBottom + graphBarMargin);
    fGraphBar->setFillPaint(radialGradient(width / 2.0f, graphBarHeight / 2.0f, graphBarHeight, width / 2.0f, Color(71, 74, 80, 255), Color(40, 42, 46, 255)));

    const float knobLabelMarginBottom = 12;

    fSwitchRemoveDC->setAbsolutePos(24, height - 38);
    fLabelRemoveDC->setAbsolutePos(24 + fSwitchRemoveDC->getWidth(), height - 38);

    fSwitchBipolarMode->setAbsolutePos(31, height - 86);
    fLabelsBoxBipolarMode->setAbsolutePos(53, height - 90);

    const float graphBarMiddleY = fGraphBar->getAbsoluteY() + fGraphBar->getHeight() / 2.0f;

    fButtonResetGraph->setAbsolutePos(20, graphBarMiddleY - fButtonResetGraph->getHeight() / 2.0f);
    fLabelButtonResetGraph->setAbsolutePos(fButtonResetGraph->getAbsoluteX() + fButtonResetGraph->getWidth(), fButtonResetGraph->getAbsoluteY());

    float centerAlignDifference = (fLabelWheelOversample->getWidth() - fWheelOversample->getWidth()) / 2.0f;

    fWheelOversample->setAbsolutePos(155, height - 82);
    fLabelWheelOversample->setAbsolutePos(155 - centerAlignDifference, height - fLabelWheelOversample->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelPreGain->getWidth() - fKnobPreGain->getWidth()) / 2.0f;

    fKnobPreGain->setAbsolutePos(width - 255, height - 90);
    fLabelPreGain->setAbsolutePos(width - 255 - centerAlignDifference, height - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelWet->getWidth() - fKnobWet->getWidth()) / 2.0f;

    fKnobWet->setAbsolutePos(width - 175, height - 90);
    fLabelWet->setAbsolutePos(width - 175 - centerAlignDifference, height - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelPostGain->getWidth() - fKnobPostGain->getWidth()) / 2.0f;

    fKnobPostGain->setAbsolutePos(width - 95, height - 90);
    fLabelPostGain->setAbsolutePos(width - 95 - centerAlignDifference, height - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelListWarpType->getWidth() - fKnobWarp->getWidth()) / 2.0f;

    fKnobWarp->setAbsolutePos(300, height - 90);
    fLabelListWarpType->setAbsolutePos(300 - centerAlignDifference, height - fLabelListWarpType->getHeight() - knobLabelMarginBottom);

    fButtonLeftArrow->setAbsolutePos(fLabelListWarpType->getAbsoluteX() - fButtonLeftArrow->getWidth(), fLabelListWarpType->getAbsoluteY());

    fButtonRightArrow->setAbsolutePos(fLabelListWarpType->getAbsoluteX() + fLabelListWarpType->getWidth(), fLabelListWarpType->getAbsoluteY());

    fHandleResize->setAbsolutePos(width - fHandleResize->getWidth(), height - fHandleResize->getHeight());
}

void WolfShaperUI::parameterChanged(uint32_t index, float value)
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
    case paramWarpType:
    {
        const int warpType = std::round(value);

        fGraphWidget->setWarpType((wolf::WarpType)warpType);
        fLabelListWarpType->setSelectedIndex(warpType);

        break;
    }

    case paramWarpAmount:
        fKnobWarp->setValue(value);
        fGraphWidget->setWarpAmount(value);
        break;
    case paramOut:
        fGraphWidget->updateInput(value);
        break;
    default:
        break;
    }
}

void WolfShaperUI::stateChanged(const char *key, const char *value)
{
    if (std::strcmp(key, "graph") == 0)
        fGraphWidget->rebuildFromString(value);

    repaint();
}

void WolfShaperUI::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();

    //background
    beginPath();

    rect(0.f, 0.f, width, height);
    fillColor(WolfShaperConfig::plugin_background);

    fill();

    closePath();

    //shadow at the bottom of the graph
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

void WolfShaperUI::uiIdle()
{
}

bool WolfShaperUI::onMouse(const MouseEvent &ev)
{
    return false;
}

void WolfShaperUI::uiReshape(uint width, uint height)
{
    //setSize(width, height);
    positionWidgets(width, height);
}

bool WolfShaperUI::onKeyboard(const KeyboardEvent &ev)
{
    /* if (ev.press)
    {
        WolfShaperConfig::load();
        repaint();
    } */

    return true;
}

void WolfShaperUI::nanoSwitchClicked(NanoSwitch *nanoSwitch)
{
    const uint switchId = nanoSwitch->getId();
    const int value = nanoSwitch->isDown() ? 1 : 0;

    setParameterValue(switchId, value);

    if (switchId == paramBipolarMode)
    {
        fLabelsBoxBipolarMode->setSelectedIndex(value);
    }
}

void WolfShaperUI::nanoButtonClicked(NanoButton *nanoButton)
{
    if (nanoButton == fButtonResetGraph)
    {
        fGraphWidget->reset();
        return;
    }

    if (nanoButton == fButtonLeftArrow)
    {
        fLabelListWarpType->goPrevious();
    }
    else 
    {
        fLabelListWarpType->goNext();
    }

    const int index = fLabelListWarpType->getSelectedIndex();

    setParameterValue(paramWarpType, index);
    fGraphWidget->setWarpType((wolf::WarpType)index);
}

void WolfShaperUI::nanoWheelValueChanged(NanoWheel *nanoWheel, const int value)
{
    const uint id = nanoWheel->getId();

    setParameterValue(paramOversample, value);

    if (id == paramWarpType)
    {
        fGraphWidget->setWarpType((wolf::WarpType)std::round(value));
    }
}

void WolfShaperUI::nanoKnobValueChanged(NanoKnob *nanoKnob, const float value)
{
    const uint id = nanoKnob->getId();

    setParameterValue(id, value);

    if (id == paramWarpAmount)
    {
        fGraphWidget->setWarpAmount(value);
    }
}

void WolfShaperUI::resizeHandleMoved(int width, int height)
{
    setSize(width, height);
}

UI *createUI()
{
    return new WolfShaperUI();
}

END_NAMESPACE_DISTRHO