#ifndef SPOONIE_TEST_WIDGET_HPP_INCLUDED
#define SPOONIE_TEST_WIDGET_HPP_INCLUDED

#include "Widget.hpp"
#include "Window.hpp"
#include "Geometry.hpp"

#include "Graph.hpp"
#include "VertexWidget.hpp"

START_NAMESPACE_DISTRHO

class TestWidget : public Widget
{
  public:
    TestWidget(Window &parent)
        : Widget(parent),
          vertexWidget(this, 0)
    {
        setSize(300, 300);
    }

  protected:
    void onDisplay() override
    {
        fprintf(stderr, "On display, test widget\n");
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

  private:
    VertexWidget vertexWidget;
};

END_NAMESPACE_DISTRHO

#endif