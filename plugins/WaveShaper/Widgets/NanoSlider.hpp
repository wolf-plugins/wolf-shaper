#ifndef SPOONIE_NANO_SLIDER_HPP_INCLUDED
#define SPOONIE_NANO_SLIDER_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class NanoSlider : public NanoWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void nanoSliderDragStarted(NanoSlider* nanoSlider) = 0;
        virtual void nanoSliderDragFinished(NanoSlider* nanoSlider) = 0;
        virtual void nanoSliderValueChanged(NanoSlider* nanoSlider, float value) = 0;
    };

    explicit NanoSlider(Window& parent, Size<uint> size) noexcept;
    explicit NanoSlider(Widget* widget, Size<uint> size) noexcept;

    float getValue() const noexcept;
    void setValue(float value, bool sendCallback = false) noexcept;

    void setStartPos(const Point<int>& startPos) noexcept;
    void setStartPos(int x, int y) noexcept;
    void setEndPos(const Point<int>& endPos) noexcept;
    void setEndPos(int x, int y) noexcept;

    void setInverted(bool inverted) noexcept;
    void setRange(float min, float max) noexcept;
    void setStep(float step) noexcept;

    void setCallback(Callback* callback) noexcept;

protected:
     void onNanoDisplay() override;
     bool onMouse(const MouseEvent&) override;
     bool onMotion(const MotionEvent&) override;

private:
    float fMinimum;
    float fMaximum;
    float fStep;
    float fValue;
    float fValueTmp;

    bool fDragging;
    bool fInverted;
    bool fValueIsSet;
    int  fStartedX;
    int  fStartedY;

    Callback* fCallback;

    Point<int> fStartPos;
    Point<int> fEndPos;
    Rectangle<int> fSliderArea;

    void _recheckArea() noexcept;

    // these should not be used
    void setAbsoluteX(int) const noexcept {}
    void setAbsoluteY(int) const noexcept {}
    void setAbsolutePos(int, int) const noexcept {}
    void setAbsolutePos(const Point<int>&) const noexcept {}

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NanoSlider)
};

END_NAMESPACE_DISTRHO

#endif