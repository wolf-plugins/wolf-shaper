#ifndef WOLF_NANO_BUTTON_HPP_INCLUDED
#define WOLF_NANO_BUTTON_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"
#include "WolfWidget.hpp"

START_NAMESPACE_DISTRHO

class NanoButton : public WolfWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void nanoButtonClicked(NanoButton* nanoButton) = 0;
    };

    enum ButtonState {
        kNanoStateNormal = 0,
        kNanoStateHover,
        kNanoStateDown
    };

    explicit NanoButton(Window& parent, Size<uint> size) noexcept;
    explicit NanoButton(Widget * widget, Size<uint> size) noexcept;

    void setCallback(Callback* callback) noexcept;

    ButtonState getButtonState();

protected:
    void onNanoDisplay() override;

    bool leftClick(const MouseEvent &ev);
    bool middleClick(const MouseEvent &ev);
    bool rightClick(const MouseEvent &ev);

    bool onMouse(const MouseEvent&) override;
    bool onMotion(const MotionEvent&) override;

    virtual void draw() = 0;

private:
    void setButtonState(ButtonState state);
    ButtonState fState;

    bool fHasFocus;
    bool fIsHovered;
    
    Callback* fCallback;

    DISTRHO_LEAK_DETECTOR(NanoButton)
};

END_NAMESPACE_DISTRHO

#endif