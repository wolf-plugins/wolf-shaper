/*
 * Wolf Shaper audio effect based on DISTRHO Plugin Framework (DPF)
 *
 * SPDX-License-Identifier: GPL v3+
 *
 * Copyright (C) 2021 Patrick Desaulniers
 */

#include "DistrhoUI.hpp"

#include "Config.hpp"
#include "Margin.hpp"
#include "Window.hpp"
#include "WolfShaperUI.hpp"

#include <string>

START_NAMESPACE_DISTRHO

WolfShaperUI::WolfShaperUI()
    : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT)
{
    loadSharedResources();

    const uint minWidth = 611;
    const uint minHeight = 438;

    const float width = getWidth();
    const float height = getHeight();
    const double scaleFactor = getScaleFactor();

    const uint knobsLabelBoxWidth = 66 * scaleFactor;
    const uint knobsLabelBoxHeight = 21 * scaleFactor;

    setGeometryConstraints(minWidth * scaleFactor, minHeight * scaleFactor, false, false);

    if (scaleFactor != 1.0)
    {
        setSize(DISTRHO_UI_DEFAULT_WIDTH * scaleFactor, DISTRHO_UI_DEFAULT_HEIGHT * scaleFactor);
    }

    fGraphWidget = new GraphWidget(this, Size<uint>(width - 4 * 2 * scaleFactor,
                                                    height - (4 * 2 - 122) * scaleFactor));

    const float graphBarHeight = 42 * scaleFactor;

    fGraphBar = new WidgetBar(this, Size<uint>(width, graphBarHeight));
    // fGraphBar->setStrokePaint();
    fGraphBar->setStrokeWidth(4.0f * scaleFactor);

    fSwitchRemoveDC = new RemoveDCSwitch(this, Size<uint>(30 * scaleFactor, 29 * scaleFactor));
    fSwitchRemoveDC->setDown(true);
    fSwitchRemoveDC->setCallback(this);
    fSwitchRemoveDC->setId(paramRemoveDC);

    fLabelRemoveDC = new NanoLabel(this, Size<uint>(100 * scaleFactor, 29 * scaleFactor));
    fLabelRemoveDC->setText("CENTER");
    //fLabelRemoveDC->setFontId(chivoBoldId);
    fLabelRemoveDC->setFontSize(14.0f * scaleFactor);
    fLabelRemoveDC->setAlign(ALIGN_LEFT | ALIGN_MIDDLE);
    fLabelRemoveDC->setMargin(Margin(3 * scaleFactor, 0, fSwitchRemoveDC->getWidth() / 2.0f, 0));

    fSwitchBipolarMode = new BipolarModeSwitch(this, Size<uint>(16 * scaleFactor, 34 * scaleFactor));
    fSwitchBipolarMode->setCallback(this);
    fSwitchBipolarMode->setId(paramBipolarMode);

    fLabelsBoxBipolarMode = new GlowingLabelsBox(this, Size<uint>(34 * scaleFactor, 42 * scaleFactor));
    fLabelsBoxBipolarMode->setLabels({"UNI", "BI"});

    fLabelPreGain = new LabelBox(this, Size<uint>(knobsLabelBoxWidth, knobsLabelBoxHeight));
    fLabelPreGain->setText("PRE");

    fKnobPreGain = new VolumeKnob(this, Size<uint>(54 * scaleFactor, 54 * scaleFactor));
    fKnobPreGain->setCallback(this);
    fKnobPreGain->setRange(0.0f, 2.0f);
    fKnobPreGain->setId(paramPreGain);
    fKnobPreGain->setColor(Color(255, 197, 246, 255));
    fKnobPreGain->setValue(1.0f, false);

    fLabelWet = new LabelBox(this, Size<uint>(knobsLabelBoxWidth, knobsLabelBoxHeight));
    fLabelWet->setText("WET");

    fKnobWet = new VolumeKnob(this, Size<uint>(54 * scaleFactor, 54 * scaleFactor));
    fKnobWet->setCallback(this);
    fKnobWet->setRange(0.0f, 1.0f);
    fKnobWet->setId(paramWet);
    fKnobWet->setColor(Color(136, 228, 255));
    fKnobWet->setValue(1.0f, false);

    fLabelPostGain = new LabelBox(this, Size<uint>(knobsLabelBoxWidth, knobsLabelBoxHeight));
    fLabelPostGain->setText("POST");

    fKnobPostGain = new VolumeKnob(this, Size<uint>(54 * scaleFactor, 54 * scaleFactor));
    fKnobPostGain->setCallback(this);
    fKnobPostGain->setRange(0.0f, 1.0f);
    fKnobPostGain->setId(paramPostGain);
    fKnobPostGain->setColor(Color(143, 255, 147, 255));
    fKnobPostGain->setValue(1.0f, false);

    fKnobHorizontalWarp = new VolumeKnob(this, Size<uint>(54 * scaleFactor, 54 * scaleFactor));
    fKnobHorizontalWarp->setCallback(this);
    fKnobHorizontalWarp->setRange(0.0f, 1.0f);
    fKnobHorizontalWarp->setId(paramHorizontalWarpAmount);
    fKnobHorizontalWarp->setColor(Color(255, 225, 169, 255));
    fKnobHorizontalWarp->setValue(0.0f, false);

    fLabelListHorizontalWarpType = new LabelBoxList(this, Size<uint>(knobsLabelBoxWidth + 3 * scaleFactor, knobsLabelBoxHeight));
    fLabelListHorizontalWarpType->setLabels({"–", "BEND +", "BEND -", "BEND +/-", "SKEW +", "SKEW -", "SKEW +/-"});

    fKnobVerticalWarp = new VolumeKnob(this, Size<uint>(54 * scaleFactor, 54 * scaleFactor));
    fKnobVerticalWarp->setCallback(this);
    fKnobVerticalWarp->setRange(0.0f, 1.0f);
    fKnobVerticalWarp->setId(paramVerticalWarpAmount);
    fKnobVerticalWarp->setColor(Color(255, 225, 169, 255));
    fKnobVerticalWarp->setValue(0.f, false);

    fLabelListVerticalWarpType = new LabelBoxList(this, Size<uint>(knobsLabelBoxWidth + 3 * scaleFactor, knobsLabelBoxHeight));
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

#ifndef __EMSCRIPTEN__
    fHandleResize = new ResizeHandle(this, Size<uint>(18 * scaleFactor, 18 * scaleFactor));
    fHandleResize->setCallback(this);
    fHandleResize->setMinSize(minWidth * scaleFactor, minHeight * scaleFactor);
        
    if (getWindow().isResizable())
    {
        fHandleResize->hide();
    }
#endif

    fButtonResetGraph = new ResetGraphButton(this, Size<uint>(32 * scaleFactor, 32 * scaleFactor));
    fButtonResetGraph->setCallback(this);

    fLabelButtonResetGraph = new NanoLabel(this, Size<uint>(50 * scaleFactor, fButtonResetGraph->getHeight()));
    fLabelButtonResetGraph->setText("RESET");
    // fLabelButtonResetGraph->setFontId(dejaVuSansId);
    fLabelButtonResetGraph->setFontSize(15.0f * scaleFactor);
    fLabelButtonResetGraph->setAlign(ALIGN_LEFT | ALIGN_MIDDLE);
    fLabelButtonResetGraph->setMargin(Margin(6 * scaleFactor, 0, std::round(fButtonResetGraph->getHeight() / 2.0f) + 1 * scaleFactor, 0));

    fWheelOversample = new OversampleWheel(this, Size<uint>(47 * scaleFactor, 26 * scaleFactor));
    fWheelOversample->setCallback(this);
    fWheelOversample->setRange(0, 4);

    fLabelWheelOversample = new NanoLabel(this, Size<uint>(85 * scaleFactor, 26 * scaleFactor));
    fLabelWheelOversample->setText("OVERSAMPLE");
    //fLabelWheelOversample->setFontId(chivoBoldId);
    fLabelWheelOversample->setFontSize(14.0f * scaleFactor);
    fLabelWheelOversample->setAlign(ALIGN_LEFT | ALIGN_MIDDLE);
    fLabelWheelOversample->setMargin(Margin(0, 0, fLabelWheelOversample->getHeight() / 2.0f, 0));

    positionWidgets(width, height, scaleFactor);
}

WolfShaperUI::~WolfShaperUI()
{
}

void WolfShaperUI::positionWidgets(uint width, uint height, double scaleFactor)
{
    //TODO: Clean that up

    const float graphMargin = 8 * scaleFactor;
    const float bottomBarSize = 102 * scaleFactor;
    const float graphBarHeight = fGraphBar->getHeight();
    const float graphBarMargin = 6 * scaleFactor;

    const float graphWidth = width - graphMargin * 2;
    const float graphHeight = height - graphMargin * 2 - bottomBarSize - graphBarHeight;
    const float graphBottom = graphMargin + graphHeight;

    fGraphWidget->setSize(graphWidth, graphHeight);
    fGraphWidget->setAbsolutePos(graphMargin, graphMargin);

    fGraphBar->setWidth(width);
    fGraphBar->setAbsolutePos(0, graphBottom + graphBarMargin);
    fGraphBar->setFillPaint(radialGradient(width / 2.0f, graphBarHeight / 2.0f, graphBarHeight, width / 2.0f, Color(71, 74, 80, 255), Color(40, 42, 46, 255)));

    fSwitchRemoveDC->setAbsolutePos(24 * scaleFactor, height - 38 * scaleFactor);
    fLabelRemoveDC->setAbsolutePos(24 * scaleFactor + fSwitchRemoveDC->getWidth(), height - 38 * scaleFactor);

    fSwitchBipolarMode->setAbsolutePos(31 * scaleFactor, height - 86 * scaleFactor);
    fLabelsBoxBipolarMode->setAbsolutePos(53 * scaleFactor, height - 90 * scaleFactor);

    const float graphBarMiddleY = fGraphBar->getAbsoluteY() + fGraphBar->getHeight() / 2.0f;

    fButtonResetGraph->setAbsolutePos(20 * scaleFactor, graphBarMiddleY - fButtonResetGraph->getHeight() / 2.0f);
    fLabelButtonResetGraph->setAbsolutePos(fButtonResetGraph->getAbsoluteX() + fButtonResetGraph->getWidth(), fButtonResetGraph->getAbsoluteY());

    fWheelOversample->setAbsolutePos(width - fWheelOversample->getWidth() - 35 * scaleFactor, graphBarMiddleY - fWheelOversample->getHeight() / 2.0f);
    fLabelWheelOversample->setAbsolutePos(fWheelOversample->getAbsoluteX() - fLabelWheelOversample->getWidth(), fWheelOversample->getAbsoluteY());

    const float knobLabelMarginBottom = 12 * scaleFactor;

    float centerAlignDifference = (fLabelPreGain->getWidth() - fKnobPreGain->getWidth()) / 2.0f;

    fKnobPreGain->setAbsolutePos(width - 225 * scaleFactor, height - 90 * scaleFactor);
    fLabelPreGain->setAbsolutePos(width - 225 * scaleFactor - centerAlignDifference, height - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelWet->getWidth() - fKnobWet->getWidth()) / 2.0f;

    fKnobWet->setAbsolutePos(width - 155 * scaleFactor, height - 90 * scaleFactor);
    fLabelWet->setAbsolutePos(width - 155 * scaleFactor - centerAlignDifference, height - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelPostGain->getWidth() - fKnobPostGain->getWidth()) / 2.0f;

    fKnobPostGain->setAbsolutePos(width - 85 * scaleFactor, height - 90 * scaleFactor);
    fLabelPostGain->setAbsolutePos(width - 85 * scaleFactor - centerAlignDifference, height - fLabelPreGain->getHeight() - knobLabelMarginBottom);

    centerAlignDifference = (fLabelListHorizontalWarpType->getWidth() - fKnobHorizontalWarp->getWidth()) / 2.0f;

    fKnobHorizontalWarp->setAbsolutePos(fKnobPreGain->getAbsoluteX() - 230 * scaleFactor, height - 90 * scaleFactor);
    fLabelListHorizontalWarpType->setAbsolutePos(fKnobPreGain->getAbsoluteX() - 230 * scaleFactor - centerAlignDifference,
                                                 height - fLabelListHorizontalWarpType->getHeight() - knobLabelMarginBottom);

    fButtonLeftArrowHorizontalWarp->setAbsolutePos(fLabelListHorizontalWarpType->getAbsoluteX() - fButtonLeftArrowHorizontalWarp->getWidth(), fLabelListHorizontalWarpType->getAbsoluteY());
    fButtonRightArrowHorizontalWarp->setAbsolutePos(fLabelListHorizontalWarpType->getAbsoluteX() + fLabelListHorizontalWarpType->getWidth(), fLabelListHorizontalWarpType->getAbsoluteY());

    centerAlignDifference = (fLabelListVerticalWarpType->getWidth() - fKnobVerticalWarp->getWidth()) / 2.0f;

    fKnobVerticalWarp->setAbsolutePos(fKnobPreGain->getAbsoluteX() - 110 * scaleFactor, height - 90 * scaleFactor);
    fLabelListVerticalWarpType->setAbsolutePos(fKnobPreGain->getAbsoluteX() - 110 * scaleFactor - centerAlignDifference,
                                               height - fLabelListVerticalWarpType->getHeight() - knobLabelMarginBottom);

    fButtonLeftArrowVerticalWarp->setAbsolutePos(fLabelListVerticalWarpType->getAbsoluteX() - fButtonLeftArrowVerticalWarp->getWidth(), fLabelListVerticalWarpType->getAbsoluteY());
    fButtonRightArrowVerticalWarp->setAbsolutePos(fLabelListVerticalWarpType->getAbsoluteX() + fLabelListVerticalWarpType->getWidth(), fLabelListVerticalWarpType->getAbsoluteY());

#ifndef __EMSCRIPTEN__
    fHandleResize->setAbsolutePos(width - fHandleResize->getWidth(), height - fHandleResize->getHeight());
#endif
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
    case paramBipolarMode: {
        const bool down = value >= 0.50f;

        fSwitchBipolarMode->setDown(down);
        fLabelsBoxBipolarMode->setSelectedIndex(down ? 1 : 0);
        break;
    }
    case paramHorizontalWarpType: {
        const int warpType = std::round(value);

        fGraphWidget->setHorizontalWarpType((wolf::WarpType)warpType);
        fLabelListHorizontalWarpType->setSelectedIndex(warpType);
        break;
    }
    case paramHorizontalWarpAmount:
        fKnobHorizontalWarp->setValue(value);
        fGraphWidget->setHorizontalWarpAmount(value);
        break;
    case paramVerticalWarpType: {
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
}

void WolfShaperUI::onNanoDisplay()
{
    const float width = getWidth();
    const float height = getHeight();
    const float scaleFactor = getScaleFactor();

    //background
    beginPath();

    rect(0.f, 0.f, width, height);
    fillColor(WolfShaperConfig::plugin_background);

    fill();

    closePath();

    //shadow at the bottom of the graph
    beginPath();

    const int shadowHeight = 8 * scaleFactor;
    const int shadowMargin = 2 * scaleFactor;

    const float graphMargin = 8 * scaleFactor;
    const float bottomBarSize = 102 * scaleFactor;
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

bool WolfShaperUI::onMouse(const MouseEvent &ev)
{
    fGraphWidget->hideMenuOnMouseOut(ev.pos);
    return UI::onMouse(ev);
}

bool WolfShaperUI::onMotion(const MotionEvent &ev)
{
    fGraphWidget->hideMenuOnMouseOut(ev.pos);
    return UI::onMotion(ev);
}

void WolfShaperUI::uiReshape(uint width, uint height)
{
    UI::uiReshape(width, height);
    positionWidgets(width, height, getScaleFactor());
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

void WolfShaperUI::nanoWheelValueChanged(NanoWheel *, const int value)
{
    setParameterValue(paramOversample, value);
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
