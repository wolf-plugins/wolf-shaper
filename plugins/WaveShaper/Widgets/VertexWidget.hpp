#ifndef SPOONIE_VERTEX_WIDGET_HPP_INCLUDED
#define SPOONIE_VERTEX_WIDGET_HPP_INCLUDED

#include "Widget.hpp"
#include "Window.hpp"
#include "Geometry.hpp"

#include "Graph.hpp"

class VertexWidget : public NanoWidget
{
  public:
    VertexWidget(Window &parent, spoonie::Vertex *vertex)
        : NanoWidget(parent),
          vertex(vertex)
    {
        setSize(18, 18);
        setVisible(true);
    }

  protected:
    void onNanoDisplay() override
    {
        const float width = getWidth();
        const float height = getHeight();

        const float centerX = width / 2.0f;
        const float centerY = height / 2.0f;

        const float lineWidth = 2.0f;

        beginPath();

        strokeWidth(lineWidth);
        strokeColor(Color(255, 255, 255, 255));

        circle(centerX, centerY, centerX - lineWidth);

        stroke();

        closePath();
    }

    static int inCircle(Circle<int> circle, Point<int> point)
    {
        const int x = point.getX();
        const int y = point.getY();

        const int xo = circle.getX();
        const int yo = circle.getY();
        const int radius = circle.getSize();

        const int dx = abs(x - xo);

        if (dx > radius)
            return false;

        const int dy = abs(y - yo);

        if (dy > radius)
            return false;

        if (dx + dy <= radius)
            return true;

        return dx * dx + dy * dy <= radius * radius;
    }

    bool leftClick(const MouseEvent &ev)
    {
        const int width = getWidth();
        const int height = getHeight();
        const float centerX = width / 2.0f;
        const float centerY = height / 2.0f;

        if (ev.press)
        {
            Circle<int> circle = Circle<int>(centerX, centerY, centerX);

            if (inCircle(circle, ev.pos))
            {
                fprintf(stderr, "Click on circle widget\n");
                getParentWindow().hideCursor();
                repaint();
            }
        }
        else
        {
            getParentWindow().showCursor();
        }

        return true;
    }

    bool middleClick(const MouseEvent &ev)
    {
        return true;
    }

    bool rightClick(const MouseEvent &ev)
    {
        return true;
    }

    bool onMouse(const MouseEvent &ev) override
    {
        switch (ev.button)
        {
        case 1:
            return leftClick(ev);
        case 2:
            return middleClick(ev);
        case 3:
            return rightClick(ev);
        }

        return false;
    }

  private:
    spoonie::Vertex *vertex;
};

#endif