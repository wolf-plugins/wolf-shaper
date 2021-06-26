/* created by rei de vries 2021-02-17
 * recreates RightClickMenu as a widget to avoid opening a new window
 */

#ifndef WOLF_RIGHT_CLICK_MENU_WIDGET_H_
#define WOLF_RIGHT_CLICK_MENU_WIDGET_H_

#include "Margin.hpp"
#include "NanoVG.hpp"
#include "Widget.hpp"
#include "Window.hpp"
#include "WolfWidget.hpp"

#include <array>
#include <string>
#include <utility>
#include <vector>

START_NAMESPACE_DISTRHO

class MenuWidget : public WolfWidget
{
  public:
    struct Item
    {
        const uint id;
        const std::string name;
        const std::string description;
        bool enabled = true;
        bool is_section = false;
        Item(const int _id,
          const std::string& _name,
          const std::string& _description)
          : id(_id),
            name(_name),
            description(_description),
            enabled(true),
            is_section(false)
        {}

        // constructor for sections
        Item(const std::string& _name)
          : id(-1),
            name(_name),
            description(""),
            enabled(true),
            is_section(true)
        {}
    };

    class Callback
    {
      public:
        virtual ~Callback() {}
        virtual void menuItemSelected(const int id) = 0;
    };

    explicit MenuWidget(Widget* widget) noexcept;

    // shows and hides the widget without affecting the elements
    void show(const Point<int>& parent_pos,
      const Point<double>& click_pos,
      const Rectangle<int>& mouse_bounds_absolute);
    void hide();
    bool hideOnMouseOutOfBounds(const Point<double>& mouse_pos_absolute);

    // clear all sections and items
    void clear();

    void addItem(int id, const char* label, const char* comment = "");
    void addSection(const char* section_name);

    // find the index of the first item with a matching name/id
    // obviously this won't work if two items have the same name/id
    int findItemIndex(const std::string& name);
    int findItemIndex(const int id);

    // if you disable an item that's a section header, all the items
    // until the next section header will be disabled too
    void setAllItemsEnabled(const bool enabled);
    void setItemEnabled(const uint index, const bool enabled);
    void setItemEnabled(const std::string& name, const bool enabled);

    // updates selected_i
    void setItemSelected(const uint i);

    void setRegularFontSize(const uint size) noexcept;
    void setSectionFontSize(const uint size) noexcept;

    template<typename T>
    Rectangle<T> getBounds() const noexcept
    {
        return Rectangle<T>(0,
          0,
          static_cast<T>(Widget::getWidth()),
          static_cast<T>(Widget::getHeight()));
    }

    template<typename T>
    Rectangle<T> getMouseBoundsAbsolute() const noexcept
    {
        return Rectangle<T>(static_cast<T>(mouse_bounds_absolute.getX()),
          static_cast<T>(mouse_bounds_absolute.getY()),
          static_cast<T>(mouse_bounds_absolute.getWidth()),
          static_cast<T>(mouse_bounds_absolute.getHeight()));
    }

    void setCallback(Callback* callback) noexcept;

    // must manually call onMouse/onMotion if the parent isn't a TopLevelWidget
    // also must manually offset by the required amount from the parent widget
    bool mouseEvent(const MouseEvent& ev, const Point<int>& offset);
    bool motionEvent(const MotionEvent& ev, const Point<int>& offset);

  protected:
    bool onMouse(const MouseEvent&) override;
    bool onMotion(const MotionEvent&) override;
    void onNanoDisplay() override;

    DGL_NAMESPACE::Rectangle<float> getBoundsOfItem(const int i);

  private:
    std::vector<Item> items;

    // if the mouse moves outside these bounds, the menu will close.
    // updated on show. assumes the bounds won't change while the menu is open.
    // this is a safe assumption in most cases, because the user will have to
    // click somewhere to change the bounds, which will close the menu.
    Rectangle<int> mouse_bounds_absolute;

    float max_item_w_px;

    int hover_i;
    int selected_i;

    Margin margin;
    float font_item_size, font_section_size;
    Color font_item_color, font_item_hover_color, font_section_color;
    Color background_color, background_hover_color, border_color;

    Callback* callback;

    void updateMaxItemWidth(const Item& item);
    void adaptSize();
    float getItemWidth(const Item& item);
    Rectangle<double> getItemBounds(const int index);
};

END_NAMESPACE_DISTRHO

#endif // WOLF_RIGHT_CLICK_MENU_WIDGET_H_
