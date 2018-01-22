#ifndef SPOONIE_GRAPH_WIDGET_HPP_INCLUDED
#define SPOONIE_GRAPH_WIDGET_HPP_INCLUDED

#include "Widget.hpp"
#include "Window.hpp"

#include "Graph.hpp"
#include "VertexWidget.hpp"

START_NAMESPACE_DISTRHO

class GraphWidget : public NanoWidget
{
  public:
    GraphWidget(UI *ui, Window &parent)
        : NanoWidget(parent),
          ui(ui)
    {
        setSize(ui->getWidth(), ui->getHeight());
        vertexWidget = new VertexWidget(getParentWindow(), lineEditor.getVertexAtIndex(0));
    }

  protected:
    void drawSubGrid()
    {
    }

    void drawGrid()
    {
        const float width = getWidth();
        const float height = getHeight();

        const float lineWidth = 2.0f;

        const float squaresPerRow = 10.0f;

        const float verticalStep = width / squaresPerRow;
        const float horizontalStep = height / squaresPerRow;

        const Color gridForegroundColor = Color(51, 51, 51, 255);
        const Color gridBackgroundColor = Color(25, 24, 26, 255);
        const Color subGridColor = Color(25, 24, 26, 255);

        //vertical
        for (int i = 0; i < squaresPerRow; ++i)
        {
            const float posX = i * verticalStep;

            //background
            beginPath();
            strokeWidth(lineWidth);
            strokeColor(gridBackgroundColor);

            moveTo(posX + lineWidth, 0.0f);
            lineTo(posX + lineWidth, height);

            stroke();
            closePath();

            //foreground
            beginPath();
            strokeWidth(lineWidth);
            strokeColor(gridForegroundColor);

            moveTo(posX, 0.0f);
            lineTo(posX, height);

            stroke();
            closePath();

            //subgrid
            beginPath();
            strokeWidth(lineWidth);
            strokeColor(subGridColor);

            moveTo(posX + verticalStep / 2.0f, 0.0f);
            lineTo(posX + verticalStep / 2.0f, height);

            stroke();
            closePath();
        }

        //horizontal
        for (int i = 0; i < squaresPerRow; ++i)
        {
            const float posY = i * horizontalStep;

            //background
            beginPath();
            strokeWidth(lineWidth);

            moveTo(0.0f, posY + lineWidth);
            lineTo(width, posY + lineWidth);

            strokeColor(gridBackgroundColor);

            stroke();
            closePath();

            //foreground
            beginPath();
            strokeWidth(lineWidth);

            moveTo(0.0f, posY);
            lineTo(width, posY);

            strokeColor(gridForegroundColor);

            stroke();
            closePath();

            //subgrid
            beginPath();
            strokeWidth(lineWidth);
            strokeColor(subGridColor);

            moveTo(0.0f, posY + horizontalStep / 2.0f);
            lineTo(width, posY + horizontalStep / 2.0f);

            stroke();
            closePath();
        }
    }

    void drawBackground()
    {
        const float width = getWidth();
        const float height = getHeight();

        const float centerX = width / 2.0f;
        const float centerY = height / 2.0f;

        beginPath();

        rect(0.f, 0.f, width, height);
        Paint gradient = radialGradient(centerX, centerY, 1.0f, centerX, Color(42, 42, 42, 255), Color(33, 32, 39, 255));
        fillPaint(gradient);

        fill();

        closePath();
    }

    void drawGraphLine(float lineWidth, Color color)
    {
        const float width = getWidth();
        const float height = getHeight();

        beginPath();

        strokeColor(color);
        strokeWidth(lineWidth);

        moveTo(0.0f, height);

        const float iteration = 1.0f / width * 2.0f;

        for (float i = 0; i < 1.0f; i += iteration)
        {
            lineTo(i * width, height - lineEditor.getValueAt(i) * height);
        }

        lineTo(width, height - lineEditor.getValueAt(1.0f) * height);

        stroke();

        closePath();
    }

    void onNanoDisplay() override
    {
        drawBackground();
        drawGrid();
        drawGraphLine(3.0f, Color(169, 29, 239, 255)); //outer
        //drawGraphLine(1.0f, Color(245, 112, 188, 255)); //inner
    }

    bool onScroll(const ScrollEvent &ev) override
    {
        float oldTension = lineEditor.getVertexAtIndex(0)->tension;
        lineEditor.setTensionAtIndex(0, spoonie::clamp(oldTension + 0.50f * -ev.delta.getY(), -100.0f, 100.0f));

        ui->setState("graph", lineEditor.serialize());

        repaint();

        return true;
    }

    bool leftClick(const MouseEvent &ev)
    {
        return true;
    }

    bool middleClick(const MouseEvent &ev)
    {
        return true;
    }

    bool rightClick(const MouseEvent &ev)
    {
        if (ev.press)
        {
            const float width = getWidth();
            const float height = getHeight();

            const float x = spoonie::normalize(ev.pos.getX(), width);
            const float y = spoonie::normalize(height - ev.pos.getY(), height);

            lineEditor.insertVertex(x, y);
            ui->setState("graph", lineEditor.serialize());

            repaint();
        }
        else
        {
        }

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
    bool mouseDown;
    Point<int> mouseDownLocation;
    spoonie::Graph lineEditor;
    VertexWidget* vertexWidget;
    UI *ui;
};

END_NAMESPACE_DISTRHO

#endif