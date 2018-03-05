#ifndef SPOONIE_NANO_SWITCH_HPP_INCLUDED
#define SPOONIE_NANO_SWITCH_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class NanoSwitch : public NanoWidget
{
public:
    class Callback
    {
    public:
        virtual ~Callback() {}
        virtual void nanoSwitchClicked(NanoSwitch* nanoSwitch) = 0;
    };

    explicit NanoSwitch(Window& parent, Size<uint> size) noexcept;
    explicit NanoSwitch(NanoWidget* widget, Size<uint> size) noexcept;

    bool isDown() const noexcept;
    void setDown(bool down) noexcept;

    void setCallback(Callback* callback) noexcept;

protected:
    void onNanoDisplay() override;
    bool onMouse(const MouseEvent&) override;
    virtual void drawUp() = 0;
    virtual void drawDown() = 0;
    
private:
    bool  fIsDown;

    Callback* fCallback;

    DISTRHO_LEAK_DETECTOR(NanoSwitch)
};

END_NAMESPACE_DISTRHO

#endif