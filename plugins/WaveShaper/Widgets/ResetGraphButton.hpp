#ifndef SPOONIE_RESET_GRAPH_BUTTON_HPP_INCLUDED
#define SPOONIE_RESET_GRAPH_BUTTON_HPP_INCLUDED

#include "NanoButton.hpp"

START_NAMESPACE_DISTRHO

class ResetGraphButton : public NanoButton
{
public:
    explicit ResetGraphButton(Window &parent, Size<uint> size) noexcept;
    explicit ResetGraphButton(NanoWidget *widget, Size<uint> size) noexcept;

protected:
    void drawUp() override;
    void drawDown() override;
    void drawHover() override;

private:
    DISTRHO_LEAK_DETECTOR(ResetGraphButton)
};

END_NAMESPACE_DISTRHO

#endif