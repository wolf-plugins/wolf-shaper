#include "MenuWidget.hpp"
#include "Config.hpp"
#include "Application.hpp"
#include <iostream>

START_NAMESPACE_DISTRHO

MenuWidget::MenuWidget( Widget *widget ) noexcept
	: WolfWidget(widget),
	  max_item_w_px(0),
	  hover_i(-1),
	  selected_i(-1),
	  margin(Margin(7,15,7,13)),
	  font_item_size(17.0f),
	  font_item_color(255,255,255),
	  font_item_hover_color(0,0,0),
	  font_section_size(14.0f),
	  font_section_color(100,100,100),
	  background_color(39,39,39),
	  background_hover_color(255,255,255),
	  border_color(CONFIG_NAMESPACE::right_click_menu_border_color),
	  callback(nullptr)
{
}

void MenuWidget::show(const Point<int>& click_pos,
	const Rectangle<int>& parent_widget_bounds)
{
	adaptSize();

	// move the menuwidget so that it always appears within the parent widget
	// bounds
	const auto menu_br = Point<int>(
		click_pos.getX() + getWidth(),
		click_pos.getY() + getHeight()
	);
	const auto parent_br = Point<int>(
		parent_widget_bounds.getX() + parent_widget_bounds.getWidth(),
		parent_widget_bounds.getY() + parent_widget_bounds.getHeight()
	);
	auto show_pos = click_pos;
	if (menu_br.getX() > parent_br.getX()) show_pos.moveBy(-getWidth(), 0);
	if (menu_br.getY() > parent_br.getY()) show_pos.moveBy(0, -getHeight());

	// set the cursor style to the default until the user hovers over an item
	//getWindow().setCursorStyle(Window::CursorStyle::Default);
	// not sure how to do this in current dgl

	NanoSubWidget::setAbsolutePos(show_pos);
	NanoSubWidget::show();
}

void MenuWidget::hide()
{
	NanoSubWidget::hide();
}

void MenuWidget::clear()
{
	items.clear();
	max_item_w_px = 0;
	hover_i = -1;
	selected_i = -1;
}

void MenuWidget::addSection(const char* sectionName)
{
	Item item = Item(sectionName);
	items.push_back(item);
	updateMaxItemWidth(item);
}

void MenuWidget::addItem(int id, const char *label, const char *comment)
{
	DISTRHO_SAFE_ASSERT(id >= 0)
	Item item = Item(id, std::string(label), std::string(comment));
	items.push_back(item);
	updateMaxItemWidth(item);
}

auto MenuWidget::findItemIndex(const std::string& name) -> int
{
	for (size_t i = 0; i < items.size(); ++i) {
		if (items[i].name == name) return i;
	}
	return -1;
}

auto MenuWidget::findItemIndex(const int id) -> int
{
	for (size_t i = 0; i < items.size(); ++i) {
		if (items[i].id == static_cast<uint>(id)) return i;
	}
	return -1;
}

void MenuWidget::setAllItemsEnabled(const bool enabled)
{
	for (auto &item : items) item.enabled = enabled;
}

void MenuWidget::setItemEnabled(const uint index, const bool enabled)
{
	items[index].enabled = enabled;
}

void MenuWidget::setItemEnabled(const std::string &name, const bool enabled)
{
	const int index = findItemIndex(name);
	if (index > 0) setItemEnabled(index, enabled);
}

void MenuWidget::setItemSelected(const uint i)
{
	selected_i = i;
}

void MenuWidget::setRegularFontSize(const uint size) noexcept
{
	this->font_item_size = size;
}

void MenuWidget::setSectionFontSize(const uint size) noexcept
{
	this->font_section_size = size;
}

void MenuWidget::setCallback(Callback *callback) noexcept
{
	this->callback = callback;
}

void MenuWidget::onNanoDisplay()
{
	if (items.size() == 0) return; // don't render an empty menu

	const float h = NanoSubWidget::getHeight();
	const float w = NanoSubWidget::getWidth();

	beginPath();

	textAlign(ALIGN_LEFT | ALIGN_TOP);

	// these four lines are used to calculate the height of a line of text
	Rectangle<float> bounds;
	fontSize(font_item_size);
	textBounds(0, 0, items[0].name.c_str(), NULL, bounds);
	const float text_h = bounds.getHeight();

	fillColor(background_color);
	strokeColor(border_color);
	strokeWidth(3.0f);

	rect(0, 0, w, h);
	fill();
	stroke();

	closePath();

	float vertical_offset = 0;
	bool cur_section_enabled = true;

	translate(margin.left, margin.top);

	for (size_t i = 0; i < items.size(); ++i) {
		Item& item = items[i];

		// black out items if section is disabled
		const bool is_section = item.is_section;
		if (is_section) cur_section_enabled = item.enabled;
		const bool is_enabled = (cur_section_enabled && item.enabled);
		const bool is_hovered = (static_cast<int>(i) == hover_i);

		// draw highlight background for hovered item
		if (is_hovered && !is_section && is_enabled)
		{
			beginPath();
			fillColor(background_hover_color);
			rect(0, vertical_offset, w - margin.right, font_item_size);
			fill();
			closePath();
		}

		beginPath();

		int left_offset=0;
		if (is_section) {
			fontSize(font_section_size);
			fillColor(font_section_color);
		} else {
			left_offset = font_section_size;
			fontSize(font_item_size);
			if (is_enabled) {
				if (is_hovered) fillColor(font_item_hover_color);
				else fillColor(font_item_color);
			} else {
				fillColor(font_section_color);
			}
		}

		text(left_offset, vertical_offset, item.name.c_str(), NULL);

		if (static_cast<int>(i) == selected_i) {
			text(0, vertical_offset, "✓", NULL);
		}

		// render description if an item has one
		if (item.description.size() > 0) {
			Rectangle<float> name_bounds;
			fontSize(font_item_size);
			textBounds(0,0, item.name.c_str(), NULL, name_bounds);

			fontSize(font_section_size);
			fillColor(font_section_color);
			text(name_bounds.getWidth() + font_item_size,
				 vertical_offset, item.description.c_str(), NULL);
		}

		vertical_offset += text_h;

		closePath();
	}
}

auto MenuWidget::mouseEvent(const MouseEvent& ev) -> bool
{
	if (!isVisible()) return false;
	return onMouse(ev);
}

auto MenuWidget::motionEvent(const MotionEvent& ev) -> bool
{
	if (!isVisible()) return false;
	return onMotion(ev);
}

auto MenuWidget::onMouse(const MouseEvent& ev) -> bool
{
	const auto bounds = getBounds<float>();
	const auto mouse_pos = Point<float>(
		static_cast<float>(ev.pos.getX()),
		static_cast<float>(ev.pos.getY())
	);

//	callback->propagateMouseEvent(ev);
	std::cout << " called onmouse" << std::endl;
	std::cout << "    pos: " << mouse_pos.getX() << "x" << mouse_pos.getY()
		<< " bounds: " << bounds.getX() << "x" << bounds.getY()
		<< " w" << bounds.getWidth() << " h" << bounds.getHeight()
		<< std::endl;

	if (ev.press == true) {
		std::cout << " mouse pressed" << std::endl;
		if (!bounds.contains(mouse_pos)) {
			NanoSubWidget::hide();
			return true;
		}

		bool cur_section_enabled = true;
		for (size_t i = 0; i < items.size(); ++i) {
			const bool is_section = items[i].is_section;
			if (is_section) cur_section_enabled = items[i].enabled;
			bool is_enabled = cur_section_enabled && items[i].enabled;

			if (!is_section && is_enabled) {
				Rectangle<float> bounds = getItemBoundsPx(i);
				bounds.setWidth(Widget::getWidth() - margin.right);

				if (bounds.contains(mouse_pos)) {
					callback->menuItemSelected(items[i].id);
					selected_i = i;
					NanoSubWidget::hide();
					return true;
				}
			}
		}
	}
	return false;
}

auto MenuWidget::onMotion(const MotionEvent& ev) -> bool
{
	// check if mouse has moved outside the bounds of the parent widget
	const auto mouse_pos_absolute = Point<int>(
		ev.pos.getX() + getAbsoluteX(),
		ev.pos.getY() + getAbsoluteY()
	);
	if (!parent_widget_bounds.contains(mouse_pos_absolute)) {
		hide();
	}
	std::cout << " called onMotion" << std::endl;

	// check if mouse is outside menu and change cursor style
	const auto menu_bounds = getBounds<double>();
	if (menu_bounds.contains(ev.pos)) {
		// update hover_i
		for (size_t i = 0; i < items.size(); ++i) {
			Rectangle<float> bounds = getItemBoundsPx(i);
			bounds.setWidth(Widget::getWidth() - margin.right);
			const Point<float> mouse_pos = Point<float>(
				static_cast<float>(ev.pos.getX()),
				static_cast<float>(ev.pos.getY())
			);

			if (static_cast<int>(i) != selected_i
				&& !items[i].is_section
				&& bounds.contains(mouse_pos)) {
				//getWindow().setCursorStyle(Window::CursorStyle::Grab);
				// not sure how to do this in current dgl
				hover_i = i;
				return true;
			}
		}
		return true;
	}
	//getWindow().setCursorStyle(Window::CursorStyle::Default);
	// not sure how to do this in current dgl
	hover_i = -1;
	return false;
}

void MenuWidget::updateMaxItemWidth(const Item& item)
{
	max_item_w_px = std::max(max_item_w_px, getItemWidthPx(item));
}

void MenuWidget::adaptSize()
{
	NanoSubWidget::setSize(Size<uint>(
		max_item_w_px + margin.left + margin.right + font_section_size,
		items.size()*font_item_size + margin.top + margin.bottom
	));
}

auto MenuWidget::getItemWidthPx(const Item& item) -> float
{
	if (item.is_section) {
		Rectangle<float> bounds;
		fontSize(font_section_size);
		textBounds(0, 0, (item.name + item.description).c_str(), NULL, bounds);
		return bounds.getWidth();
	} else {
		Rectangle<float> bounds_item, bounds_section;
		fontSize(font_item_size);
		textBounds(0,0,item.name.c_str(), NULL, bounds_item);
		if (item.description.length() > 0)
		{
			fontSize(font_section_size);
			textBounds(0,0,item.description.c_str(), NULL, bounds_section);
		}
		return bounds_item.getWidth() + bounds_section.getWidth();
	}
}

auto MenuWidget::getItemBoundsPx(const int index) -> Rectangle<float>
{
	fontSize(font_item_size);
	textAlign(ALIGN_LEFT | ALIGN_TOP);
	Rectangle<float> bounds;
	textBounds(margin.left, index*font_item_size + margin.top,
		items[index].name.c_str(), NULL, bounds);
	return bounds;
}

END_NAMESPACE_DISTRHO
