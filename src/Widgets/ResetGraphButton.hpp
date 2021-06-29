#ifndef WOLF_RESET_GRAPH_BUTTON_HPP_INCLUDED
#define WOLF_RESET_GRAPH_BUTTON_HPP_INCLUDED

#include "NanoButton.hpp"

START_NAMESPACE_DISTRHO

class ResetGraphButton : public NanoButton
{
public:
    explicit ResetGraphButton(Widget  *widget, Size<uint> size) noexcept;

protected:
    void draw() override;
    
    void drawUp();
    void drawDown();
    void drawHover();

private:
    DISTRHO_LEAK_DETECTOR(ResetGraphButton)
};

END_NAMESPACE_DISTRHO

#endif