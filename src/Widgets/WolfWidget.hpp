#ifndef WOLF_WIDGET_HPP_INCLUDED
#define WOLF_WIDGET_HPP_INCLUDED

#include "Widget.hpp"
#include "NanoVG.hpp"

START_NAMESPACE_DISTRHO

class WolfWidget : public NanoSubWidget
{
public:
  WolfWidget(Widget *widget) noexcept;

  void setDescription(const char *description);
  const char *getDescription();

  bool canBeFocused();
  void setFocus(bool focus);

protected:
  void mouseWarp(); //TODO: design this

private:
  uint *fFocusedWidgetId; //only one focused widget at a time
  const char *fDescription;

  DISTRHO_LEAK_DETECTOR(WolfWidget)
};

END_NAMESPACE_DISTRHO

#endif
