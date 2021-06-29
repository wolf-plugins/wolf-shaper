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
    /**
	 * This struct contains data for each menu entry, including the sections.
	 * name and description are used for display.
	 * if enabled==false then selection will be grayed out. If a section isn't
	 * enabled, then all items up until the next section will also be grayed
	 * out.
	 * The "id" field is used to address each user-selectable option, it is
	 * irrelevant for a section.
	 */
    struct Item
    {
        const uint id;
        const std::string name;
        const std::string description;
        bool enabled = true;
        bool is_section = false;
        Item(const int _id,
             const std::string &_name,
             const std::string &_description)
            : id(_id),
              name(_name),
              description(_description),
              enabled(true),
              is_section(false)
        {
        }

        // constructor for sections
        Item(const std::string &_name)
            : id(0),
              name(_name),
              description(""),
              enabled(true),
              is_section(true)
        {
        }
    };

    /**
	 * Callback for when a menu item is selected.
	 */
    class Callback
    {
    public:
        virtual ~Callback()
        {
        }
        virtual void menuItemSelected(const int id) = 0;
    };

    explicit MenuWidget(Widget *widget) noexcept;

    /**
	 * shows and hides the widget without affecting the elements
	 */
    void show(const Point<int> &parent_pos,
              const Point<double> &click_pos,
              const Rectangle<int> &mouse_bounds_absolute);
    void hide();

    /**
	 * this must be called from the top-level widget on mouse/motion events
	 * to close the menu when the mouse leaves the graph area or clicks anywhere
	 */
    bool hideOnMouseOutOfBounds(const Point<double> &mouse_pos_absolute);

    /**
	 * clears the list of Items
	 */
    void clear();

    /**
	 * Constructs an Item using either constructor, and adds it to the vector
	 */
    void addItem(int id, const char *label, const char *comment = "");
    void addSection(const char *section_name);

    /**
	 * changes the enabled field of items
	 */
    void setAllItemsEnabled(const bool enabled);
    void setItemEnabled(const uint index, const bool enabled);

    /**
	 * updates selected_i. this should be called externally to remember user
	 * selections after the menu is hidden.
	 */
    void setItemSelected(const uint i);

    /**
	 * set font sizes externally
	 */
    void setRegularFontSize(const uint size) noexcept;
    void setSectionFontSize(const uint size) noexcept;

    /**
	 * Helper functions to get the menu bounds and mouse bounds.
	 */
    template <typename T>
    Rectangle<T> getBounds() const noexcept
    {
        return Rectangle<T>(0,
                            0,
                            static_cast<T>(Widget::getWidth()),
                            static_cast<T>(Widget::getHeight()));
    }

    template <typename T>
    Rectangle<T> getMouseBoundsAbsolute() const noexcept
    {
        return Rectangle<T>(static_cast<T>(mouse_bounds_absolute.getX()),
                            static_cast<T>(mouse_bounds_absolute.getY()),
                            static_cast<T>(mouse_bounds_absolute.getWidth()),
                            static_cast<T>(mouse_bounds_absolute.getHeight()));
    }

    /**
	 * set the callback function to call
	 */
    void setCallback(Callback *callback) noexcept;

    /**
	 * In current DPF, we must manually call onMouse/onMotion if the instance
	 * is not owned by a TopLevelWidget. These methods allow onMouse/onMotion
	 * to be called manually with the same behaviour as if it were called
	 * automatically by a TopLevelWidget.
	 */
    bool mouseEvent(const MouseEvent &ev, const Point<int> &offset);
    bool motionEvent(const MotionEvent &ev, const Point<int> &offset);

protected:
    /**
	 * DPF methods
	 */
    bool onMouse(const MouseEvent &) override;
    bool onMotion(const MotionEvent &) override;
    void onNanoDisplay() override;

private:
    std::vector<Item> items;

    // if the mouse moves outside these bounds, the menu will close.
    // updated on show. assumes the bounds won't change while the menu is open.
    // this is a safe assumption in most cases, because the user will have to
    // click somewhere to change the bounds, which will close the menu.
    /**
	 * if the mouse moves outside these bounds, the menu will close.
	 * updated on "show".
	 * assumes the bounds won't change while the menu is open.
	 * this will be a safe assumption unless window-manager based resizing of
	 * windows is allowed, hopefully we can implement an event callback on
	 * window resize.
	 */
    Rectangle<int> mouse_bounds_absolute;

    /**
	 * This keeps track of the greatest item width in pixels, and is calculated
	 * whenever items are added. This saves us having to recalculate the menu
	 * size every time it is displayed.
	 */
    float max_item_w_px;

    /**
	 * Keeps track of currently hovered/selected indices
	 */
    int hover_i;
    int selected_i;

    /**
	 * Display parameters (with descriptive names!)
	 */
    Margin margin;
    float font_item_size, font_section_size;
    Color font_item_color, font_item_hover_color, font_section_color;
    Color background_color, background_hover_color, border_color;

    Callback *callback;

    /**
	 * update max_item_w_px
	 */
    void updateMaxItemWidth(const Item &item);
    float getItemWidth(const Item &item);

    void adaptSize();
    Rectangle<double> getItemBounds(const int index);
};

END_NAMESPACE_DISTRHO

#endif // WOLF_RIGHT_CLICK_MENU_WIDGET_H_
