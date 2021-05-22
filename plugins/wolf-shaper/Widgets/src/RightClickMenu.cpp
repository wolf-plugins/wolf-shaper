#include "RightClickMenu.hpp"
#include "Config.hpp"
#include <unistd.h>

START_NAMESPACE_DISTRHO

RightClickMenuItem::RightClickMenuItem(int id, const char *label, const char *comment, bool enabled) noexcept : fIsSection(false),
                                                                                                                fId(id),
                                                                                                                fEnabled(enabled),
                                                                                                                fLabel(label),
                                                                                                                fComment(comment),
                                                                                                                fSelected(false)
{
}

int RightClickMenuItem::getId()
{
    return fId;
}

const char *RightClickMenuItem::getLabel()
{
    return fLabel;
}

const char *RightClickMenuItem::getComment()
{
    return fComment;
}

bool RightClickMenuItem::getEnabled()
{
    return fEnabled;
}

void RightClickMenuItem::setEnabled(bool enabled)
{
    fEnabled = enabled;
}

bool RightClickMenuItem::isSection()
{
    return fIsSection;
}

void RightClickMenuItem::setLabel(const char *label)
{
    fLabel = label;
}

bool RightClickMenuItem::getSelected()
{
    return fSelected;
}

void RightClickMenuItem::setSelected(const bool selected)
{
    fSelected = selected;
}

RightClickMenuSection::RightClickMenuSection(const char *label) noexcept : RightClickMenuItem(-1, label, "", false)
{
    fIsSection = true;
}

RightClickMenu::RightClickMenu(Widget  *parent) noexcept : Window(parent->getApp(), parent->getWindow()),
                                                              WolfWidget(parent),
                                                              fParent(parent),
                                                              fFontSize(17.0f),
                                                              fSectionFontSize(14.0f),
                                                              fHoveredIndex(-1),
                                                              fLongestWidth(0.0f),
                                                              fBorderColor(CONFIG_NAMESPACE::right_click_menu_border_color),
                                                              fMargin(Margin(7, 15, 7, 13))
{
    Window::setResizable(false);
    //Window::setBorderless(true);

    loadSharedResources();
}

RightClickMenu::~RightClickMenu()
{
    Window::close();
}

void RightClickMenu::show(int posX, int posY)
{
    //we don't want the mouse to intersect with the popup straight away, so we add a bit of margin
    posX += 2;
    posY += 2;

    adaptSize();

    Window::runAsModal(false);
}

void RightClickMenu::close()
{
    Window::close();
}

void RightClickMenu::setBorderColor(const Color color)
{
    fBorderColor = color;
}

void RightClickMenu::setRegularFontSize(float fontSize)
{
    fFontSize = fontSize;
}

void RightClickMenu::setSectionFontSize(float fontSize)
{
    fSectionFontSize = fontSize;
}

Rectangle<float> RightClickMenu::getBoundsOfItemComment(const int index)
{
    fontSize(fSectionFontSize);
    textAlign(ALIGN_LEFT | ALIGN_TOP);

    Rectangle<float> bounds;
    Rectangle<float> itemBounds = getBoundsOfItem(index);

    textBounds(itemBounds.getX() + itemBounds.getWidth(), index * fFontSize + fMargin.top, fItems[index].getComment(), NULL, bounds);

    return bounds;
}

Rectangle<float> RightClickMenu::getBoundsOfItem(const int index)
{
    fontSize(fFontSize);
    textAlign(ALIGN_LEFT | ALIGN_TOP);

    Rectangle<float> bounds;

    textBounds(0 + fMargin.left, index * fFontSize + fMargin.top, fItems[index].getLabel(), NULL, bounds);

    return bounds;
}

void RightClickMenu::setSectionEnabled(int index, bool enabled)
{
    DISTRHO_SAFE_ASSERT(index >= 0)

    int sectionCount = -1;

    for (size_t i = 0; i < fItems.size(); ++i)
    {
        if (fItems[i].isSection())
        {
            ++sectionCount;

            if (sectionCount == index)
            {
                ++i;

                while (i < fItems.size() && !fItems[i].isSection())
                {
                    fItems[i].setEnabled(enabled);
                    ++i;
                }

                return;
            }
        }
    }
}

void RightClickMenu::adaptSize()
{
    findLongestItem();

    const Size<uint> size = Size<uint>(fLongestWidth + fMargin.left + fMargin.right + 12, fItems.size() * fFontSize + fMargin.top + fMargin.bottom);

    Window::setSize(size);
    NanoSubWidget::setSize(size);
}

RightClickMenuItem *RightClickMenu::getItemById(int id)
{
    //TODO: optimize with binary search or something
    for (size_t i = 0; i < fItems.size(); ++i)
    {
        if (fItems[i].getId() == id)
        {
            return &fItems[i];
        }
    }

    return nullptr;
}

bool RightClickMenuItem::hasComment()
{
    return strcmp(fComment, "") != 0;
}

void RightClickMenu::findLongestItem()
{
    fLongestWidth = 0.0f;

    for (size_t i = 0; i < fItems.size(); ++i)
    {
        float itemWidth = getBoundsOfItem(i).getWidth();

        if (fItems[i].hasComment())
        {
            itemWidth += getBoundsOfItemComment(i).getWidth();
        }

        if (itemWidth > fLongestWidth)
        {
            fLongestWidth = itemWidth;
        }
    }
}

void RightClickMenu::addSection(const char *sectionName)
{
    RightClickMenuSection section = RightClickMenuSection(sectionName);

    fItems.push_back(section);
}

void RightClickMenu::addItem(int id, const char *label, const char *comment)
{
    DISTRHO_SAFE_ASSERT(id >= 0)

    RightClickMenuItem item = RightClickMenuItem(id, label, comment, true);

    fItems.push_back(item);
}

void RightClickMenu::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void RightClickMenu::onNanoDisplay()
{
    const float width = NanoSubWidget::getWidth();
    const float height = NanoSubWidget::getHeight();

    beginPath();

    Rectangle<float> bounds;

    fontSize(fFontSize);
    textAlign(ALIGN_LEFT | ALIGN_TOP);
    textBounds(0, 0, fItems[0].getLabel(), NULL, bounds);

    fillColor(Color(39, 39, 39, 255));

    strokeColor(fBorderColor);
    strokeWidth(3.0f);

    rect(0, 0, width, height);
    fill();
    stroke();

    closePath();

    float verticalOffset = 0;

    translate(fMargin.left, fMargin.top);

    for (size_t i = 0; i < fItems.size(); ++i)
    {
        const Color itemTextColorEnabled = i == fHoveredIndex ? Color(0, 0, 0) : Color(255, 255, 255);
        const Color itemTextColorDisabled = Color(100, 100, 100);

        if (i == fHoveredIndex)
        {
            beginPath();

            fillColor(Color(255, 255, 255));
            rect(0, verticalOffset, width - fMargin.right, fFontSize);
            fill();

            closePath();
        }

        beginPath();

        if (fItems[i].isSection())
        {
            fontSize(fSectionFontSize);
        }
        else
        {
            fontSize(fFontSize);
        }

        if (fItems[i].getEnabled() == true)
        {
            fillColor(itemTextColorEnabled);
        }
        else
        {
            fillColor(itemTextColorDisabled);
        }

        text(fItems[i].isSection() ? 0 : 14, verticalOffset, fItems[i].getLabel(), NULL);

        if (fItems[i].hasComment())
        {
            fontSize(fSectionFontSize);
            fillColor(itemTextColorDisabled);

            text(getBoundsOfItem(i).getWidth() + 14 + 4, verticalOffset, fItems[i].getComment(), NULL);
        }

        if (fItems[i].getSelected())
        {
            fontSize(fFontSize);
            fillColor(itemTextColorEnabled);
            text(0, verticalOffset, "✓", NULL);
        }

        verticalOffset += bounds.getHeight();

        closePath();
    }
}

bool RightClickMenu::onMouse(const MouseEvent &ev)
{
    if (ev.press == true)
    {
        for (size_t i = 0; i < fItems.size(); ++i)
        {
            Rectangle<float> itemBounds = getBoundsOfItem(i);
            itemBounds.setWidth(Widget::getWidth() - fMargin.right);

            if (fItems[i].getEnabled() == true && itemBounds.contains(Point<float>(ev.pos.getX(), ev.pos.getY())) && !fItems[i].isSection())
            {
                fCallback->rightClickMenuItemSelected(&fItems[i]);

                close();

                return true;
            }
        }
    }

    return true;
}

bool RightClickMenu::onMotion(const MotionEvent &ev)
{
    Widget::repaint();

    for (size_t i = 0; i < fItems.size(); ++i)
    {
        Rectangle<float> itemBounds = getBoundsOfItem(i);
        itemBounds.setWidth(Widget::getWidth() - fMargin.right);

        if (fItems[i].getEnabled() == true && itemBounds.contains(Point<float>(ev.pos.getX(), ev.pos.getY())) && !fItems[i].isSection())
        {
            fHoveredIndex = i;
            return true;
        }
    }

    fHoveredIndex = -1;
    return true;
}

/* void RightClickMenu::onFocusOut()
{
    close();
} */

bool RightClickMenu::onClose()
{
    fHoveredIndex = -1;

    return true;
}

END_NAMESPACE_DISTRHO
