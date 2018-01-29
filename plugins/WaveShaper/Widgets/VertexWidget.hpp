#ifndef SPOONIE_VERTEX_WIDGET_HPP_INCLUDED
#define SPOONIE_VERTEX_WIDGET_HPP_INCLUDED

#include "Widget.hpp"
#include "Window.hpp"
#include "Geometry.hpp"

#include "Graph.hpp"

START_NAMESPACE_DISTRHO

class VertexWidget : public NanoWidget
{
  public:
    VertexWidget(Window &parent) : NanoWidget(parent)
    {
        setSize(300, 300);
    }

    VertexWidget(Widget *parent, uint32_t vertexId)
        : NanoWidget(parent),
          vertexId(vertexId)
    {
        setSize(300, 300);
    }

    uint32_t vertexId;

  protected:
    void onNanoDisplay() override
    {
        fprintf(stderr, "Render\n");

        beginPath();

        strokeWidth(2.0f);
        strokeColor(Color(255, 255, 255, 255));

        circle(18.0f, 18.0f, 10.0f);

        stroke();

        closePath();
    }

    bool leftClick(const MouseEvent &ev)
    {
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
};

END_NAMESPACE_DISTRHO

#endif