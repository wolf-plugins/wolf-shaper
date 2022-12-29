#include "MenuWidget.hpp"

#include "Application.hpp"
#include "Config.hpp"
#include "Fonts/chivo_bold.hpp"

START_NAMESPACE_DISTRHO

MenuWidget::MenuWidget(Widget *widget) noexcept
    : WolfWidget(widget),
      max_item_w_px(0),
      hover_i(-1),
      selected_i(-1),
      margin(Margin(7*getScaleFactor(), 15*getScaleFactor(), 7*getScaleFactor(), 13*getScaleFactor())),
      font_item_size(17.0f*getScaleFactor()),
      font_section_size(14.0f*getScaleFactor()),
      font_item_color(255, 255, 255),
      font_item_hover_color(0, 0, 0),
      font_section_color(100, 100, 100),
      background_color(39, 39, 39),
      background_hover_color(255, 255, 255),
      border_color(CONFIG_NAMESPACE::right_click_menu_border_color),
      callback(nullptr)
{
    using namespace WOLF_FONTS;
    loadSharedResources();
    hide();
}

void MenuWidget::show(const Point<int> &parent_pos_absolute,
                      const Point<double> &click_pos,
                      const Rectangle<int> &mouse_bounds_absolute)
{
    this->mouse_bounds_absolute = mouse_bounds_absolute;
    adaptSize();

    auto show_pos = Point<int>(click_pos.getX() + parent_pos_absolute.getX(),
                               click_pos.getY() + parent_pos_absolute.getY());

    // move the menuwidget so that it always appears within the parent widget
    // bounds
    const int menu_bottom = show_pos.getY() + getHeight();
    const int menu_right = show_pos.getX() + getWidth();
    if (menu_right > mouse_bounds_absolute.getWidth())
        show_pos.moveBy(-getWidth(), 0);
    if (menu_bottom > mouse_bounds_absolute.getHeight())
        show_pos.moveBy(0, -getHeight());

    // set the cursor style to the default until the user hovers over an item
    // getWindow().setCursorStyle(Window::CursorStyle::Default);
    // not sure how to do this in current dgl

    NanoSubWidget::setAbsolutePos(show_pos);
    NanoSubWidget::show();
}

void MenuWidget::hide()
{
    hover_i = -1; // cant hover over a hidden menu, so set this to invalid
    NanoSubWidget::hide();
}

bool MenuWidget::hideOnMouseOutOfBounds(const Point<double> &mouse_pos_absolute)
{
    if (!isVisible())
        return false;
    const auto mouse_bounds_absolute_double = getMouseBoundsAbsolute<double>();
    if (mouse_bounds_absolute_double.contains(mouse_pos_absolute))
    {
        return false;
    }
    else
    {
        hide();
        return true;
    }
}

void MenuWidget::clear()
{
    items.clear();
    max_item_w_px = 0;
    hover_i = -1;
    selected_i = -1;
}

void MenuWidget::addSection(const char *sectionName)
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

void MenuWidget::setAllItemsEnabled(const bool enabled)
{
    for (auto &item : items)
        item.enabled = enabled;
}

void MenuWidget::setItemEnabled(const uint index, const bool enabled)
{
    items[index].enabled = enabled;
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
    if (items.size() == 0)
        return; // don't render an empty menu

    const float h = NanoSubWidget::getHeight();
    const float w = NanoSubWidget::getWidth();
    const float scaleFactor = getScaleFactor();

    beginPath();

    textAlign(ALIGN_LEFT | ALIGN_TOP);

    // these four lines are used to calculate the height of a line of text
    Rectangle<float> bounds;
    fontSize(font_item_size);
    textBounds(0, 0, items[0].name.c_str(), NULL, bounds);
    const float text_h = bounds.getHeight();

    fillColor(background_color);
    strokeColor(border_color);
    strokeWidth(3.0f * scaleFactor);

    rect(0, 0, w, h);
    fill();
    stroke();

    closePath();

    float vertical_offset = 0;
    bool cur_section_enabled = true;

    translate(margin.left, margin.top);

    for (size_t i = 0; i < items.size(); ++i)
    {
        Item &item = items[i];

        // black out items if section is disabled
        const bool is_section = item.is_section;
        if (is_section)
            cur_section_enabled = item.enabled;
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

        int left_offset = 0;
        if (is_section)
        {
            fontSize(font_section_size);
            fillColor(font_section_color);
        }
        else
        {
            left_offset = font_section_size;
            fontSize(font_item_size);
            if (is_enabled)
            {
                if (is_hovered)
                    fillColor(font_item_hover_color);
                else
                    fillColor(font_item_color);
            }
            else
            {
                fillColor(font_section_color);
            }
        }

        text(left_offset, vertical_offset, item.name.c_str(), NULL);

        if (static_cast<int>(i) == selected_i)
        {
            text(0, vertical_offset, "✓", NULL);
        }

        // render description if an item has one
        if (item.description.size() > 0)
        {
            Rectangle<float> name_bounds;
            fontSize(font_item_size);
            textBounds(0, 0, item.name.c_str(), NULL, name_bounds);

            fontSize(font_section_size);
            fillColor(font_section_color);
            text(name_bounds.getWidth() + font_item_size,
                 vertical_offset,
                 item.description.c_str(),
                 NULL);
        }

        vertical_offset += text_h;

        closePath();
    }
}

bool MenuWidget::mouseEvent(const MouseEvent &ev, const Point<int> &offset)
{
    if (!isVisible())
        return false;
    MouseEvent ev_offset = ev;
    // translate from coordinates relative to parent, to coordinates relative
    // to the top left corner of this MenuWidget
    ev_offset.pos =
        Point<double>(ev.pos.getX() + offset.getX() - getAbsoluteX(),
                      ev.pos.getY() + offset.getY() - getAbsoluteY());
    return onMouse(ev_offset);
}

bool MenuWidget::motionEvent(const MotionEvent &ev, const Point<int> &offset)
{
    if (!isVisible())
        return false;
    MotionEvent ev_offset = ev;
    // translate from coordinates relative to parent, to coordinates relative
    // to the top left corner of this MenuWidget
    ev_offset.pos =
        Point<double>(ev.pos.getX() + offset.getX() - getAbsoluteX(),
                      ev.pos.getY() + offset.getY() - getAbsoluteY());
    return onMotion(ev_offset);
}

bool MenuWidget::onMouse(const MouseEvent &ev)
{
    if (ev.press == true)
    {
        // first check if the mouse click even happened inside the menu
        const auto menu_bounds = getBounds<double>();
        if (!menu_bounds.contains(ev.pos))
        {
            hide();
            return false;
        }
        // then check each section to see if the mouse click intersects
        bool cur_section_enabled = true;
        for (size_t i = 0; i < items.size(); ++i)
        {
            const bool is_section = items[i].is_section;
            if (is_section)
                cur_section_enabled = items[i].enabled;
            bool is_enabled = cur_section_enabled && items[i].enabled;

            if (!is_section && is_enabled)
            {
                const auto item_bounds = getItemBounds(i);

                if (item_bounds.contains(ev.pos))
                {
                    callback->menuItemSelected(items[i].id);
                    selected_i = i;
                    hide();
                    return true;
                }
            }
        }
    }
    return false;
}

bool MenuWidget::onMotion(const MotionEvent &ev)
{
    // check if mouse is outside menu and change cursor style
    const auto menu_bounds = getBounds<double>();
    if (menu_bounds.contains(ev.pos))
    {
        // update hover_i
        for (size_t i = 0; i < items.size(); ++i)
        {
            const auto item_bounds = getItemBounds(i);

            if (static_cast<int>(i) != selected_i && !items[i].is_section &&
                item_bounds.contains(ev.pos))
            {
                // getWindow().setCursorStyle(Window::CursorStyle::Grab);
                // not sure how to do this in current dgl
                hover_i = i;
                return true;
            }
        }
        hover_i = -1;
        return true;
    }
    // getWindow().setCursorStyle(Window::CursorStyle::Default);
    // not sure how to do this in current dgl
    hover_i = -1;
    return false;
}

void MenuWidget::updateMaxItemWidth(const Item &item)
{
    max_item_w_px = std::max(max_item_w_px, getItemWidth(item));
}

void MenuWidget::adaptSize()
{
    NanoSubWidget::setSize(
        Size<uint>(max_item_w_px + margin.left + margin.right + font_section_size,
                   items.size() * font_item_size + margin.top + margin.bottom));
}

float MenuWidget::getItemWidth(const Item &item)
{
    if (item.is_section)
    {
        Rectangle<float> bounds;
        fontSize(font_section_size);
        textBounds(0, 0, (item.name + item.description).c_str(), NULL, bounds);
        return bounds.getWidth();
    }
    else
    {
        Rectangle<float> bounds_item, bounds_section;
        fontSize(font_item_size);
        textBounds(0, 0, item.name.c_str(), NULL, bounds_item);
        if (item.description.length() > 0)
        {
            fontSize(font_section_size);
            textBounds(0, 0, item.description.c_str(), NULL, bounds_section);
        }
        return bounds_item.getWidth() + bounds_section.getWidth();
    }
}

Rectangle<double> MenuWidget::getItemBounds(const int index)
{
    fontSize(font_item_size);
    textAlign(ALIGN_LEFT | ALIGN_TOP);
    Rectangle<float> bounds;
    textBounds(margin.left,
               index * font_item_size + margin.top,
               items[index].name.c_str(),
               NULL,
               bounds);
    bounds.setWidth(Widget::getWidth() - margin.right);
    return Rectangle<double>(
        bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

END_NAMESPACE_DISTRHO
