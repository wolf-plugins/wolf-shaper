#ifndef WOLF_RIGHT_CLICK_MENU_H
#define WOLF_RIGHT_CLICK_MENU_H

#include "Widget.hpp"
#include "Window.hpp"
#include "NanoVG.hpp"
#include "NanoLabel.hpp"
#include "WolfWidget.hpp"

#include <vector>

START_NAMESPACE_DISTRHO

class RightClickMenuItem
{
public:
  RightClickMenuItem(int id, const char *label, const char *comment = "", bool enabled = true) noexcept;

  int getId();
  bool getEnabled();
  void setEnabled(bool enabled);
  const char *getLabel();
  void setLabel(const char *label);
  const char *getComment();
  bool hasComment();

  bool getSelected();
  void setSelected(const bool selected);

  bool isSection();

protected:
  bool fIsSection;

private:
  int fId;
  bool fEnabled;
  const char *fLabel;
  const char *fComment;

  bool fSelected;

  DGL_NAMESPACE::Rectangle<float> fBounds;
};

class RightClickMenuSection : public RightClickMenuItem
{
public:
  RightClickMenuSection(const char *label) noexcept;
};

class RightClickMenu : private Window,
                       public WolfWidget
{
public:
  class Callback
  {
  public:
    virtual ~Callback() {}
    virtual void rightClickMenuItemSelected(RightClickMenuItem *rightClickMenuItem) = 0;
  };

  RightClickMenu(Widget  *parent) noexcept;
  ~RightClickMenu();

  void show(int posX, int posY);
  void close();
  void addItem(int id, const char *label, const char *comment = "");
  void addSection(const char *sectionName);
  void setBorderColor(const Color color);
  void setRegularFontSize(float fontSize);
  void setSectionFontSize(float fontSize);
  RightClickMenuItem *getItemById(int id);
  void setCallback(Callback *callback) noexcept;
  void setSectionEnabled(int index, bool enabled);

protected:
  void onNanoDisplay() override;
  //void onFocusOut() override;
  bool onMouse(const MouseEvent &ev) override;
  bool onMotion(const MotionEvent &ev) override;
  void adaptSize();
  bool onClose() override;

  DGL_NAMESPACE::Rectangle<float> getBoundsOfItem(const int index);
  DGL_NAMESPACE::Rectangle<float> getBoundsOfItemComment(const int index);

private:
  void findLongestItem();

  std::vector<RightClickMenuItem> fItems;
  Widget *fParent;

  float fFontSize;
  float fSectionFontSize;

  float fHoveredIndex;
  float fLongestWidth;
  Color fBorderColor;
  Margin fMargin;

  Callback *fCallback;
};

#endif

END_NAMESPACE_DISTRHO
