#ifndef SPOONIE_NANO_KNOB_HPP_INCLUDED
#define SPOONIE_NANO_KNOB_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class NanoKnob : public NanoWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void nanoKnobDragStarted(NanoKnob* nanoKnob) = 0;
        virtual void nanoKnobDragFinished(NanoKnob* nanoKnob) = 0;
        virtual void nanoKnobValueChanged(NanoKnob* nanoKnob, float value) = 0;
    };

    explicit NanoKnob(Window& parent) noexcept;
    explicit NanoKnob(Widget* widget) noexcept;

    float getValue() const noexcept;
    void setValue(float value, bool sendCallback = false) noexcept;

    void setDefault(float def) noexcept;
    void setRange(float min, float max) noexcept;
    void setStep(float step) noexcept;
    void setUsingLogScale(bool yesNo) noexcept;
    void setCallback(Callback* callback) noexcept;

protected:
     void onNanoDisplay() override;
     bool onMouse(const MouseEvent&) override;
     bool onMotion(const MotionEvent&) override;
     bool onScroll(const ScrollEvent&) override;

private:
    float fMinimum;
    float fMaximum;
    float fStep;
    float fValue;
    float fValueDef;
    float fValueTmp;
    bool  fUsingDefault;
    bool  fUsingLog;

    int  fRotationAngle;
    bool fDragging;
    int  fLastX;
    int  fLastY;

    Callback* fCallback;

    bool fIsReady;

    void setRotationAngle(const int angle);

    DISTRHO_LEAK_DETECTOR(NanoKnob)
};

END_NAMESPACE_DISTRHO