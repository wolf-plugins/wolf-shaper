#include "DistrhoUI.hpp"

#include "WaveShaperUI.hpp"

START_NAMESPACE_DISTRHO

namespace Art = DistrhoArtworkNekobi;

WaveShaperUI::WaveShaperUI() : UI(600, 600),
                               graphWidgetSocket(this, getParentWindow())
{
    // knobs (taken from Nekobi, NanoKnobs would be nice)
    Image knobImage(Art::knobData, Art::knobWidth, Art::knobHeight);

    fKnobPre = new ImageKnob(this, knobImage, ImageKnob::Vertical);
    fKnobPre->setId(paramPreGain);
    fKnobPre->setAbsolutePos(41, 540);
    fKnobPre->setRange(0.0f, 2.0f);
    fKnobPre->setDefault(1.0f);
    fKnobPre->setValue(1.0f);
    fKnobPre->setRotationAngle(305);
    fKnobPre->setCallback(this);

    fKnobDryWet = new ImageKnob(this, knobImage, ImageKnob::Vertical);
    fKnobDryWet->setId(paramWet);
    fKnobDryWet->setAbsolutePos(261, 540);
    fKnobDryWet->setRange(0.0f, 1.0f);
    fKnobDryWet->setDefault(1.0f);
    fKnobDryWet->setValue(1.0f);
    fKnobDryWet->setRotationAngle(305);
    fKnobDryWet->setCallback(this);

    fKnobPost = new ImageKnob(this, knobImage, ImageKnob::Vertical);
    fKnobPost->setId(paramPostGain);
    fKnobPost->setAbsolutePos(481, 540);
    fKnobPost->setRange(0.0f, 1.0f);
    fKnobPost->setDefault(1.0f);
    fKnobPost->setValue(1.0f);
    fKnobPost->setRotationAngle(305);
    fKnobPost->setCallback(this);
}

WaveShaperUI::~WaveShaperUI()
{
}

void WaveShaperUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
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
    }

    repaint();
    
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
    editParameter(knob->getId(), true);
}

void WaveShaperUI::imageKnobDragFinished(ImageKnob *knob)
{
    editParameter(knob->getId(), false);
}

void WaveShaperUI::imageKnobValueChanged(ImageKnob *knob, float value)
{
    setParameterValue(knob->getId(), value);
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