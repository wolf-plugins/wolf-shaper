#include "Layout.hpp"


START_NAMESPACE_DISTRHO


Anchors::Anchors() : left(true),
					 right(false),
					 top(true),
					 bottom(false)
{
}

Anchors::Anchors(bool left, bool right, bool top, bool bottom) : left(left),
																 right(right),
																 top(top),
																 bottom(bottom)
{
}

RelativePosition::RelativePosition() : left(0),
									   right(0),
									   top(0),
									   bottom(0)
{
}

RelativePosition::RelativePosition(int left, int right, int top, int bottom) : left(left),
																			   right(right),
																			   top(top),
																			   bottom(bottom)
{
}

LayoutItem::LayoutItem(Layout *parent, Widget *widget) : fWidget(widget),
														 fParent(parent),
														 fAnchors(),
														 fRelativePos()
{
}

Widget *LayoutItem::getWidget()
{
	return fWidget;
}

LayoutItem &LayoutItem::setAnchors(int anchors)
{
	if (anchors & Anchors::All)
	{
		fAnchors.left = true;
		fAnchors.right = true;
		fAnchors.top = true;
		fAnchors.bottom = true;

		return *this;
	}

	fAnchors.left = anchors & Anchors::Left;
	fAnchors.right = anchors & Anchors::Right;
	fAnchors.top = anchors & Anchors::Top;
	fAnchors.bottom = anchors & Anchors::Bottom;

	return *this;
}

LayoutItem &LayoutItem::setAnchors(Anchors anchors)
{
	fAnchors = anchors;

	return *this;
}

Anchors LayoutItem::getAnchors()
{
	return fAnchors;
}

LayoutItem &LayoutItem::setRelativePos(const int left, const int right, const int top, const int bottom)
{
	fRelativePos.left = left;
	fRelativePos.right = right;
	fRelativePos.top = top;
	fRelativePos.bottom = bottom;

	return *this;
}

LayoutItem &LayoutItem::setRelativeLeft(const int left)
{
	fRelativePos.left = left;

	return *this;
}

LayoutItem &LayoutItem::setRelativeRight(const int right)
{
	fRelativePos.right = right;

	return *this;
}

LayoutItem &LayoutItem::setRelativeTop(const int top)
{
	fRelativePos.top = top;

	return *this;
}

LayoutItem &LayoutItem::setRelativeBottom(const int bottom)
{
	fRelativePos.bottom = bottom;

	return *this;
}

RelativePosition LayoutItem::getRelativePos()
{
	return fRelativePos;
}

Layout::Layout(Widget *parent) : Widget(parent)
{
	setSize(parent->getSize());
	hide();
}

Layout::Layout(Window &parent) : Widget(parent)
{
	setSize(parent.getSize());
	hide();
}

LayoutItem &LayoutItem::setSize(const uint width, const uint height)
{
	fWidget->setSize(width, height);

	fRelativePos.right = fParent->getWidth() - (fRelativePos.left + fWidget->getWidth());
	fRelativePos.bottom = fParent->getHeight() - (fRelativePos.top + fWidget->getHeight());

	return *this;
}

LayoutItem &LayoutItem::setPosition(const int x, const int y)
{
	fRelativePos.left = x;
	fRelativePos.right = fParent->getWidth() - (fRelativePos.left + fWidget->getWidth());
	fRelativePos.top = y;
	fRelativePos.bottom = fParent->getHeight() - (fRelativePos.top + fWidget->getHeight());

	fWidget->setAbsolutePos(fParent->getAbsoluteX() + fRelativePos.left, fParent->getAbsoluteY() + fRelativePos.top);

	return *this;
}

LayoutItem *Layout::getItem(const uint index)
{
	DISTRHO_SAFE_ASSERT_RETURN(index < getItemCount(), nullptr);

	return &fItems[index];
}

LayoutItem *Layout::getFirstItem()
{
	return getItem(0);
}

LayoutItem *Layout::getLastItem()
{
	return getItem(getItemCount() - 1);
}

size_t Layout::getItemCount()
{
	return fItems.size();
}

LayoutItem &Layout::addItem(Widget *widget)
{
	LayoutItem item = LayoutItem(this, widget);

	fItems.push_back(item);

	onItemAdded(item);

	return fItems[fItems.size() - 1];
}

void Layout::onItemAdded(const LayoutItem &)
{
}

RelativeLayout::RelativeLayout(Widget *parent) : Layout(parent)
{
}

void RelativeLayout::onDisplay()
{
}

void RelativeLayout::repositionItems()
{
	repositionItems(getSize(), getSize());
}

void RelativeLayout::repositionItems(Size<uint> oldSize, Size<uint> newSize)
{
	int absX = getAbsoluteX();
	int absY = getAbsoluteY();

	for (uint i = 0; i < getItemCount(); ++i)
	{
		LayoutItem *item = getItem(i);
		const Anchors anchors = item->getAnchors();

		int deltaWidth = newSize.getWidth() - oldSize.getWidth();
		int deltaHeight = newSize.getHeight() - oldSize.getHeight();

		if (!anchors.right)
		{
			int right = item->getRelativePos().right;

			item->setRelativeRight(right + deltaWidth);
		}
		else if (!anchors.left)
		{
			int left = item->getRelativePos().left;

			item->setRelativeLeft(left + deltaWidth);
		}

		item->getWidget()->setAbsoluteX(absX + item->getRelativePos().left);
		item->getWidget()->setWidth(getWidth() - item->getRelativePos().left - item->getRelativePos().right);

		if (!anchors.bottom)
		{
			int bottom = item->getRelativePos().bottom;

			item->setRelativeBottom(bottom + deltaHeight);
		}
		else if (!anchors.top)
		{
			int top = item->getRelativePos().top;

			item->setRelativeTop(top + deltaHeight);
		}

		item->getWidget()->setAbsoluteY(absY + item->getRelativePos().top);
		item->getWidget()->setHeight(getHeight() - item->getRelativePos().top - item->getRelativePos().bottom);
	}
}

void RelativeLayout::onResize(const ResizeEvent &ev)
{
	repositionItems(ev.oldSize, ev.size);
}

void RelativeLayout::onPositionChanged(const PositionChangedEvent &)
{
	const int absX = getAbsoluteX();
	const int absY = getAbsoluteY();

	for (uint i = 0; i < getItemCount(); ++i)
	{
		LayoutItem *item = getItem(i);

		item->getWidget()->setAbsoluteX(absX + item->getRelativePos().left);
		item->getWidget()->setAbsoluteY(absY + item->getRelativePos().top);
	}
}

StackLayout::StackLayout(Widget *parent) : Layout(parent)
{
}

void StackLayout::onDisplay()
{
}

void StackLayout::repositionItems()
{
	const int absX = getAbsoluteX();
	const int absY = getAbsoluteY();

	int accumulatedWidth = 0;

	for (size_t i = 0; i < getItemCount(); ++i)
	{
		LayoutItem &layoutItem = fItems[i];
		Widget *widget = layoutItem.getWidget();
		widget->setAbsolutePos(absX + accumulatedWidth + layoutItem.getRelativePos().left, absY + layoutItem.getRelativePos().top);

		accumulatedWidth += fItems[i].getWidget()->getWidth();
	}
}

void StackLayout::onResize(const ResizeEvent &ev)
{
	repositionItems();
}

void StackLayout::onPositionChanged(const PositionChangedEvent &)
{
	repositionItems();
}


END_NAMESPACE_DISTRHO
