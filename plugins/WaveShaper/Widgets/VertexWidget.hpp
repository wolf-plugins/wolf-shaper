#ifndef SPOONIE_VERTEX_WIDGET_HPP_INCLUDED
#define SPOONIE_VERTEX_WIDGET_HPP_INCLUDED

#include "Widget.hpp"
#include "Window.hpp"
#include "Geometry.hpp"

#include "Graph.hpp"

class VertexWidget : public NanoWidget
{
  public:
    VertexWidget(Window &parent, uint32_t vertexId)
        : NanoWidget(parent),
          vertexId(vertexId)
    {
        setSize(18, 18);
        setVisible(false);
    }

    uint32_t vertexId;

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
                getParentWindow().hideCursor();
                repaint();
            }
        }
        else
        {
            getParentWindow().showCursor();
        }

        return false;
    }

    bool middleClick(const MouseEvent &ev)
    {
        return false;
    }

    bool rightClick(const MouseEvent &ev)
    {
        return false;
    }

    bool onMotion(const MotionEvent &) override
    {
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

        return true;
    }

  private:
    spoonie::Vertex *vertex;
};

#endif