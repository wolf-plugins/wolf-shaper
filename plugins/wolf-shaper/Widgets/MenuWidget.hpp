/* created by rei de vries 2021-02-17
 * recreates RightClickMenu as a widget to avoid opening a new window
 */

#ifndef WOLF_RIGHT_CLICK_MENU_WIDGET_H_
#define WOLF_RIGHT_CLICK_MENU_WIDGET_H_

#include "Widget.hpp"
#include "NanoVG.hpp"
#include "WolfWidget.hpp"
#include "Margin.hpp"
#include "Window.hpp"
#include <vector>
#include <array>
#include <string>
#include <utility>

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
		{
		}

		// constructor for sections
		Item(const std::string& _name)
			: id(-1),
			  name(_name),
			  description(""),
			  enabled(true),
			  is_section(true)
		{
		}
	};

	class Callback
	{
	public:
		virtual ~Callback() {}
		virtual void menuItemSelected(const int id) = 0;
	};

	explicit MenuWidget( Widget *widget ) noexcept;

	// shows and hides the widget without affecting the elements
	void show(const Point<int>& click_pos,
		const Rectangle<int>& parent_widget_bounds);
	void hide();

	// clear all sections and items
	void clear();

	void addItem(int id, const char *label, const char *comment = "");
	void addSection(const char *sectionName);

	// find the index of the first item with a matching name/id
	// obviously this won't work if two items have the same name/id
	auto findItemIndex(const std::string& name) -> int;
	auto findItemIndex(const int id) -> int;

	// if you disable an item that's a section header, all the items
	// until the next section header will be disabled too
	void setAllItemsEnabled(const bool enabled);
	void setItemEnabled(const uint index, const bool enabled);
	void setItemEnabled(const std::string& name, const bool enabled);

	// updates selected_i
	void setItemSelected(const uint  i);

	void setRegularFontSize(const uint size) noexcept;
	void setSectionFontSize(const uint size) noexcept;

	template<typename T>
	auto getBounds() const -> Rectangle<T>
	{
		return Rectangle<T>(0, 0,
			static_cast<T>(Widget::getWidth()),
			static_cast<T>(Widget::getHeight())
		);
	}

	void setCallback(Callback *callback) noexcept;

	// must manually call onMouse/onMotion if the parent isn't a
	// TopLevelWidget
	auto mouseEvent(const MouseEvent &ev) -> bool;
	auto motionEvent(const MotionEvent &ev) -> bool;

protected:
	auto onMouse( const MouseEvent &)  -> bool override;
	auto onMotion(const MotionEvent &) -> bool override;
	void onNanoDisplay() override;

	DGL_NAMESPACE::Rectangle<float> getBoundsOfItem(const int i);

private:
	std::vector<Item> items;

	// if the mouse moves outside these bounds, the menu will close.
	// updated on show. assumes the bounds won't change while the menu is open.
	// this is a safe assumption in most cases, because the user will have to
	// click somewhere to change the bounds, which will close the menu.
	Rectangle<int> parent_widget_bounds;

	float max_item_w_px;

	int hover_i;
	int selected_i;

	Margin margin;
	float font_item_size;
	Color font_item_color;
	Color font_item_hover_color;
	float font_section_size;
	Color font_section_color;
	Color background_color;
	Color background_hover_color;
	Color border_color;

	Callback *callback;

	void updateMaxItemWidth(const Item& item);
	void adaptSize();
	auto getItemWidthPx(const Item& item) -> float;
	auto getItemBoundsPx(const int index) -> Rectangle<float>;
};

END_NAMESPACE_DISTRHO

#endif // GRAPHDYN_MENUWIDGET_H_
