#ifndef SPOONIE_NANO_BUTTON_HPP_INCLUDED
#define SPOONIE_NANO_BUTTON_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class NanoButton : public NanoWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void nanoButtonClicked(NanoButton* nanoButton) = 0;
    };

    explicit NanoButton(Window& parent, Size<uint> size) noexcept;
    explicit NanoButton(NanoWidget* widget, Size<uint> size) noexcept;

    void setCallback(Callback* callback) noexcept;

protected:
    void onNanoDisplay() override;

    bool leftClick(const MouseEvent &ev);
    bool middleClick(const MouseEvent &ev);
    bool rightClick(const MouseEvent &ev);

    bool onMouse(const MouseEvent&) override;
    bool onMotion(const MotionEvent&) override;

    virtual void drawUp() = 0;
    virtual void drawDown() = 0;
    virtual void drawHover() = 0;

private:
    enum State {
        kNanoStateNormal = 0,
        kNanoStateHover,
        kNanoStateDown
    };

    void setButtonState(State state);
    State state;

    bool fHasFocus;
    Callback* fCallback;

    DISTRHO_LEAK_DETECTOR(NanoButton)
};

END_NAMESPACE_DISTRHO

#endif