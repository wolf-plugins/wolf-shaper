#include "WolfWidget.hpp"
#include "Window.hpp"

START_NAMESPACE_DISTRHO

WolfWidget::WolfWidget(Widget *widget) noexcept
    : NanoSubWidget(widget),
      fScaleFactor(getTopLevelWidget()->getScaleFactor())
{
}

void WolfWidget::setDescription(const char *description)
{
    fDescription = description;
}

const char *WolfWidget::getDescription() const
{
    return fDescription;
}

double WolfWidget::getScaleFactor() const
{
    return fScaleFactor;
}

bool WolfWidget::canBeFocused() const
{
    return true;
}

void WolfWidget::setFocus(bool focus)
{
    (void)focus;
}

END_NAMESPACE_DISTRHO
