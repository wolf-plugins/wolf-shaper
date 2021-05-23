/*
 * Wolf Shaper audio effect based on DISTRHO Plugin Framework (DPF)
 *
 * SPDX-License-Identifier: GPL v3+
 *
 * Copyright (C) 2021 Patrick Desaulniers
 */

#include "DistrhoUI.hpp"

#include "WolfShaperUI.hpp"
#include "Window.hpp"
#include "Config.hpp"
#include "Margin.hpp"

#include <string>

START_NAMESPACE_DISTRHO

WolfShaperUI::WolfShaperUI() : UI(611, 662)
{
    const uint minWidth = 611;
    const uint minHeight = 438;

    setGeometryConstraints(minWidth, minHeight, false, false);

    const uint knobsLabelBoxWidth = 66;
    const uint knobsLabelBoxHeight = 21;

    loadSharedResources();

    const float width = getWidth();
    const float height = getHeight();

    fGraphWidget = new GraphWidget(this, Size<uint>(width - 4 * 2, height - 4 * 2 - 122));

    const float graphBarHeight = 42;

    fGraphBar = new WidgetBar(this, Size<uint>(width, graphBarHeight));
    // fGraphBar->setStrokePaint();
    fGraphBar->setStrokeWidth(4.0f);

    fSwitchRemoveDC = new RemoveDCSwitch(this, Size<uint>(30, 29));
    fSwitchRemoveDC->setCallback(this);
    fSwitchRemoveDC->setId(paramRemoveDC);

    fLabelRemoveDC = new NanoLabel(this, Size<uint>(100, 29));
    fLabelRemoveDC->setText("CENTER");
    //fLabelRemoveDC->setFontId(chivoBoldId);
    fLabelRemoveDC->setFontSize(14.0f);
    fLabelRemoveDC->setAlign(ALIGN_LEFT | ALIGN_MIDDLE);
    fLabelRemoveDC->setMargin(Margin(3, 0, fSwitchRemoveDC->getWidth() / 2.0f, 0));

    fSwitchBipolarMode = new BipolarModeSwitch(this, Size<uint>(16, 34));
    fSwitchBipolarMode->setCallback(this);
    fSwitchBipolarMode->setId(paramBipolarMode);

    fLabelsBoxBipolarMode = new GlowingLabelsBox(this, Size<uint>(34, 42));
    fLabelsBoxBipolarMode->setLabels({"UNI", "BI"});

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
    fKnobWet->setColor(Color(136, 228, 255));

    fLabelPostGain = new LabelBox(this, Size<uint>(knobsLabelBoxWidth, knobsLabelBoxHeight));
    fLabelPostGain->setText("POST");

    fKnobPostGain = new VolumeKnob(this, Size<uint>(54, 54));
    fKnobPostGain->setCallback(this);
    fKnobPostGain->setRange(0.0f, 1.0f);
    fKnobPostGain->setId(paramPostGain);
    fKnobPostGain->setColor(Color(143, 255, 147, 255));

    fKnobHorizontalWarp = new VolumeKnob(this, Size<uint>(54, 54));
    fKnobHorizontalWarp->setCallback(this);
    fKnobHorizontalWarp->setRange(0.0f, 1.0f);
    fKnobHorizontalWarp->setId(paramHorizontalWarpAmount);
    fKnobHorizontalWarp->setColor(Color(255, 225, 169, 255));

    fLabelListHorizontalWarpType = new LabelBoxList(this, Size<uint>(knobsLabelBoxWidth + 3, knobsLabelBoxHeight));
    fLabelListHorizontalWarpType->setLabels({"–", "BEND +", "BEND -", "BEND +/-", "SKEW +", "SKEW -", "SKEW +/-"});

    fKnobVerticalWarp = new VolumeKnob(this, Size<uint>(54, 54));
    fKnobVerticalWarp->setCallback(this);
    fKnobVerticalWarp->setRange(0.0f, 1.0f);
    fKnobVerticalWarp->setId(paramVerticalWarpAmount);
    fKnobVerticalWarp->setColor(Color(255, 225, 169, 255));

    fLabelListVerticalWarpType = new LabelBoxList(this, Size<uint>(knobsLabelBoxWidth + 3, knobsLabelBoxHeight));
    fLabelListVerticalWarpType->setLabels({"–", "BEND +", "BEND -", "BEND +/-", "SKEW +", "SKEW -", "SKEW +/-"});

    fButtonLeftArrowHorizontalWarp = new ArrowButton(this, Size<uint>(knobsLabelBoxHeight, knobsLabelBoxHeight));
    fButtonLeftArrowHorizontalWarp->setCallback(this);
    fButtonLeftArrowHorizontalWarp->setId(paramHorizontalWarpType);
    fButtonLeftArrowHorizontalWarp->setArrowDirection(ArrowButton::Left);

    fButtonRightArrowHorizontalWarp = new ArrowButton(this, Size<uint>(knobsLabelBoxHeight, knobsLabelBoxHeight));
    fButtonRightArrowHorizontalWarp->setCallback(this);
    fButtonRightArrowHorizontalWarp->setId(paramHorizontalWarpType);
    fButtonRightArrowHorizontalWarp->setArrowDirection(ArrowButton::Right);

    fButtonLeftArrowVerticalWarp = new ArrowButton(this, Size<uint>(knobsLabelBoxHeight, knobsLabelBoxHeight));
    fButtonLeftArrowVerticalWarp->setCallback(this);
    fButtonLeftArrowVerticalWarp->setId(paramVerticalWarpType);
    fButtonLeftArrowVerticalWarp->setArrowDirection(ArrowButton::Left);

    fButtonRightArrowVerticalWarp = new ArrowButton(this, Size<uint>(knobsLabelBoxHeight, knobsLabelBoxHeight));
    fButtonRightArrowVerticalWarp->setCallback(this);
    fButtonRightArrowVerticalWarp->setId(paramVerticalWarpType);
    fButtonRightArrowVerticalWarp->setArrowDirection(ArrowButton::Right);

    fHandleResize = new ResizeHandle(this, Size<uint>(18, 18));
    fHandleResize->setCallback(this);
    fHandleResize->setMinSize(minWidth, minHeight);

    if (getWindow().isResizable())
    {
        fHandleResize->hide();
    }

    fButtonResetGraph = new ResetGraphButton(this, Size<uint>(32, 32));
    fButtonResetGraph->setCallback(this);

    fLabelButtonResetGraph = new NanoLabel(this, Size<uint>(50, fButtonResetGraph->getHeight()));
    fLabelButtonResetGraph->setText("RESET");
    // fLabelButtonResetGraph->setFontId(dejaVuSansId);
    fLabelButtonResetGraph->setFontSize(15.0f);
    fLabelButtonResetGraph->setAlign(ALIGN_LEFT | ALIGN_MIDDLE);
    fLabelButtonResetGraph->setMargin(Margin(6, 0, std::round(fButtonResetGraph->getHeight() / 2.0f) + 1, 0));

    fWheelOversample = new OversampleWheel(this, Size<uint>(47, 26));
    fWheelOversample->setCallback(this);
    fWheelOversample->setRange(0, 4);

    fLabelWheelOversample = new NanoLabel(this, Size<uint>(85, 26));
    fLabelWheelOversample->setText("OVERSAMPLE");
    //fLabelWheelOversample->setFontId(chivoBoldId);
    fLabelWheelOversample->setFontSize(14.0f);
    fLabelWheelOversample->setAlign(ALIGN_LEFT | ALIGN_MIDDLE);
    fLabelWheelOversample->setMargin(Margin(0, 0, fLabelWheelOversample->getHeight() / 2.0f, 0));
    
    positionWidgets(width, height);
}

WolfShaperUI::~WolfShaperUI()
{
}

void WolfShaperUI::positionWidgets(uint width, uint height)
{
    //TODO: Clean that up

    const float graphMargin = 8;
    const float bottomBarSize = 102;
    const float graphBarHeight = fGraphBar->getHeight();
    const float graphBarMargin = 6;

    const float graphWidth = width - graphMargin * 2;
    const float graphHeight = height - graphMargin * 2 - bottomBarSize - graphBarHeight;
    const float graphBottom = graphMargin + graphHeight;

    fGraphWidget->setSize(graphWidth, graphHeight);
    fGraphWidget->setAbsolutePos(graphMargin, graphMargin);

    fGraphBar->setWidth(width);
    fGraphBar->setAbsolutePos(0, graphBottom + graphBarMargin);
    fGraphBar->setFillPaint(radialGradient(width / 2.0f, graphBarHeight / 2.0f, graphBarHeight, width / 2.0f, Color(71, 74, 80, 255), Color(40, 42, 46, 255)));

    fSwitchRemoveDC->setAbsolutePos(24, height - 38);
    fLabelRemoveDC->setAbsolutePos(24 + fSwitchRemoveDC->getWidth(), height - 38);

    fSwitchBipolarMode->setAbsolutePos(31, height - 86);
    fLabelsBoxBipolarMode->setAbsolutePos(53, height - 90);

    const float graphBarMiddleY = fGraphBar->getAbsoluteY() + fGraphBar->getHeight() / 2.0f;

    fButtonResetGraph->setAbsolutePos(20, graphBarMiddleY - fButtonResetGraph->getHeight() / 2.0f);
    fLabelButtonResetGraph->setAbsolutePos(fButtonResetGraph->getAbsoluteX() + fButtonResetGraph->getWidth(), fButtonResetGraph->getAbsoluteY());

    fWheelOversample->setAbsolutePos(width - fWheelOversample->getWidth() - 35, graphBarMiddleY - fWheelOversample->getHeight() / 2.0f);
    fLabelWheelOversample->setAbsolutePos(fWheelOversample->getAbsoluteX() - fLabelWheelOversample->getWidth(), fWheelOversample->getAbsoluteY());

    const float knobLabelMarginBottom = 12;

    float centerAlignDifference = (fLabelPreGain->getWidth() - fKnobPreGain->getWidth()) / 2.0f;

    fKnobPreGain->setAbsolutePos(width - 225, height - 90);
    fLabelPreGain->setAbsolutePos(width - 225 - centerAlignDifference, height - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelWet->getWidth() - fKnobWet->getWidth()) / 2.0f;

    fKnobWet->setAbsolutePos(width - 155, height - 90);
    fLabelWet->setAbsolutePos(width - 155 - centerAlignDifference, height - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelPostGain->getWidth() - fKnobPostGain->getWidth()) / 2.0f;

    fKnobPostGain->setAbsolutePos(width - 85, height - 90);
    fLabelPostGain->setAbsolutePos(width - 85 - centerAlignDifference, height - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelListHorizontalWarpType->getWidth() - fKnobHorizontalWarp->getWidth()) / 2.0f;

    fKnobHorizontalWarp->setAbsolutePos(fKnobPreGain->getAbsoluteX() - 230, height - 90);
    fLabelListHorizontalWarpType->setAbsolutePos(fKnobPreGain->getAbsoluteX() - 230 - centerAlignDifference, height - fLabelListHorizontalWarpType->getHeight() - knobLabelMarginBottom);

    fButtonLeftArrowHorizontalWarp->setAbsolutePos(fLabelListHorizontalWarpType->getAbsoluteX() - fButtonLeftArrowHorizontalWarp->getWidth(), fLabelListHorizontalWarpType->getAbsoluteY());
    fButtonRightArrowHorizontalWarp->setAbsolutePos(fLabelListHorizontalWarpType->getAbsoluteX() + fLabelListHorizontalWarpType->getWidth(), fLabelListHorizontalWarpType->getAbsoluteY());

    centerAlignDifference = (fLabelListVerticalWarpType->getWidth() - fKnobVerticalWarp->getWidth()) / 2.0f;

    fKnobVerticalWarp->setAbsolutePos(fKnobPreGain->getAbsoluteX() - 110, height - 90);
    fLabelListVerticalWarpType->setAbsolutePos(fKnobPreGain->getAbsoluteX() - 110 - centerAlignDifference, height - fLabelListVerticalWarpType->getHeight() - knobLabelMarginBottom);

    fButtonLeftArrowVerticalWarp->setAbsolutePos(fLabelListVerticalWarpType->getAbsoluteX() - fButtonLeftArrowVerticalWarp->getWidth(), fLabelListVerticalWarpType->getAbsoluteY());
    fButtonRightArrowVerticalWarp->setAbsolutePos(fLabelListVerticalWarpType->getAbsoluteX() + fLabelListVerticalWarpType->getWidth(), fLabelListVerticalWarpType->getAbsoluteY());

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
    case paramHorizontalWarpType:
    {
        const int warpType = std::round(value);

        fGraphWidget->setHorizontalWarpType((wolf::WarpType)warpType);
        fLabelListHorizontalWarpType->setSelectedIndex(warpType);

        break;
    }
    case paramHorizontalWarpAmount:
        fKnobHorizontalWarp->setValue(value);
        fGraphWidget->setHorizontalWarpAmount(value);
        break;
    case paramVerticalWarpType:
    {
        const int warpType = std::round(value);

        fGraphWidget->setVerticalWarpType((wolf::WarpType)warpType);
        fLabelListVerticalWarpType->setSelectedIndex(warpType);

        break;
    }
    case paramVerticalWarpAmount:
        fKnobVerticalWarp->setValue(value);
        fGraphWidget->setVerticalWarpAmount(value);
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

    const float graphMargin = 8;
    const float bottomBarSize = 102;
    const float graphBarHeight = fGraphBar->getHeight();

    const float graphHeight = height - graphMargin * 2 - bottomBarSize - graphBarHeight;
    const float graphBottom = graphMargin + graphHeight;

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

bool WolfShaperUI::onMouse(const MouseEvent &)
{
    return false;
}

void WolfShaperUI::uiReshape(uint width, uint height)
{
    //setSize(width, height);
    positionWidgets(width, height);
}

bool WolfShaperUI::onKeyboard(const KeyboardEvent &)
{
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

    bool horizontal = false;

    if (nanoButton == fButtonLeftArrowHorizontalWarp)
    {
        fLabelListHorizontalWarpType->goPrevious();
        horizontal = true;
    }
    else if (nanoButton == fButtonRightArrowHorizontalWarp)
    {
        fLabelListHorizontalWarpType->goNext();
        horizontal = true;
    }
    else if (nanoButton == fButtonLeftArrowVerticalWarp)
    {
        fLabelListVerticalWarpType->goPrevious();
    }
    else if (nanoButton == fButtonRightArrowVerticalWarp)
    {
        fLabelListVerticalWarpType->goNext();
    }

    if (horizontal)
    {
        const int index = fLabelListHorizontalWarpType->getSelectedIndex();

        setParameterValue(paramHorizontalWarpType, index);
        fGraphWidget->setHorizontalWarpType((wolf::WarpType)index);
    }
    else
    {
        const int index = fLabelListVerticalWarpType->getSelectedIndex();

        setParameterValue(paramVerticalWarpType, index);
        fGraphWidget->setVerticalWarpType((wolf::WarpType)index);
    }
}

void WolfShaperUI::nanoWheelValueChanged(NanoWheel *nanoWheel, const int value)
{
    const uint id = nanoWheel->getId();

    setParameterValue(paramOversample, value);

    if (id == paramHorizontalWarpType)
    {
        fGraphWidget->setHorizontalWarpType((wolf::WarpType)std::round(value));
    }
    else if (id == paramVerticalWarpType)
    {
        fGraphWidget->setVerticalWarpType((wolf::WarpType)std::round(value));
    }
}

void WolfShaperUI::nanoKnobValueChanged(NanoKnob *nanoKnob, const float value)
{
    const uint id = nanoKnob->getId();

    setParameterValue(id, value);

    if (id == paramHorizontalWarpAmount)
    {
        fGraphWidget->setHorizontalWarpAmount(value);
    }
    else if (id == paramVerticalWarpAmount)
    {
        fGraphWidget->setVerticalWarpAmount(value);
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
