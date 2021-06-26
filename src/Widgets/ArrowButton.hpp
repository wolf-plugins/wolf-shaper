#ifndef WOLF_ARROW_BUTTON_HPP_INCLUDED
#define WOLF_ARROW_BUTTON_HPP_INCLUDED

#include "NanoButton.hpp"

START_NAMESPACE_DISTRHO

class ArrowButton : public NanoButton
{
public:
    enum ArrowDirection
    {
        Left,
        Right
    };

    explicit ArrowButton(Widget  *widget, Size<uint> size) noexcept;
    void setArrowDirection(ArrowDirection direction);

protected:
    void drawBackground();
    void drawArrow();
    void draw() override;
    
private:
    ArrowDirection fArrowDirection;

    DISTRHO_LEAK_DETECTOR(ArrowButton)
};

END_NAMESPACE_DISTRHO

#endif