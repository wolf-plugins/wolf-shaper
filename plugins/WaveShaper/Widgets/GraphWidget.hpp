#ifndef SPOONIE_GRAPH_WIDGET_HPP_INCLUDED
#define SPOONIE_GRAPH_WIDGET_HPP_INCLUDED

#include "Widget.hpp"
#include "Window.hpp"

#include "Graph.hpp"

#include "GeometricalWidgets.hpp"
#include "Mathf.hpp"

START_NAMESPACE_DISTRHO

class GraphWidget : public NanoWidget
{
  public:
    GraphWidget(UI *ui, Window &parent)
        : NanoWidget(parent),
          ui(ui)
    {
        setSize(ui->getWidth(), ui->getHeight());

        vertexWidgets[0] = spoonie::Circle(0.0f, 0.0f, absoluteVertexSize);
        vertexWidgets[1] = spoonie::Circle(1.0f, 1.0f, absoluteVertexSize);
    }

    void rebuildFromString(const char *serializedGraph)
    {
        lineEditor.rebuildFromString(serializedGraph);
    }

    void updateAnimations()
    {
    }

  protected:
    void flipY() //(0,0) at the bottom-left corner makes more sense for this plugin
    {
        transform(1.0f, 0.0f, 0.0f, -1.0f, 0.0f, getHeight());
        normalizeCoordinates();
    }

    void normalizeCoordinates() {
        glTranslatef(0.0f, 0.0f, -10.0f);
    }

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
            lineTo(i * width, lineEditor.getValueAt(i) * height);
        }

        lineTo(width, lineEditor.getValueAt(1.0f) * height);

        stroke();

        closePath();
    }

    void drawVertex(int index)
    {
        const spoonie::Circle vertex = vertexWidgets[index];

        const float posX = vertex.x * getWidth();
        const float posY = vertex.y * getHeight();

        beginPath();

        fillColor(vertex.color);

        strokeWidth(2.0f);
        strokeColor(Color(0, 0, 0, 255));

        circle(posX, posY, vertex.absoluteDiameter);

        fill();
        stroke();

        closePath();
    }

    void drawTensionHandle(int index)
    {
        const float width = 8;
        const float height = width;

        beginPath();

        fillColor(Color(255, 255, 255, 255));

        strokeWidth(2.0f);
        strokeColor(Color(0, 0, 0, 255));

        const spoonie::Vertex *vertexLeft = lineEditor.getVertexAtIndex(index);
        const spoonie::Vertex *vertexRight = lineEditor.getVertexAtIndex(index + 1);

        const float normalizedCenterX = (vertexLeft->x + vertexRight->x) / 2.0f;
        const float centerX = normalizedCenterX * getWidth();

        const float posY = lineEditor.getValueAt(normalizedCenterX) * getHeight();

        circle(centerX, posY, width);

        fill();
        stroke();

        closePath();
    }

    void drawGraphVertices()
    {
        for (int i = 0; i < lineEditor.getVertexCount() - 1; ++i)
        {
            drawVertex(i);
            drawTensionHandle(i);
        }

        drawVertex(lineEditor.getVertexCount() - 1);
    }

    void onNanoDisplay() override
    {
        flipY();

        drawBackground();
        drawGrid();
        drawGraphLine(3.0f, Color(169, 29, 239, 255)); //outer
        //drawGraphLine(1.0f, Color(245, 112, 188, 255)); //inner
        drawGraphVertices();
    }

    bool onScroll(const ScrollEvent &ev) override
    {
        float oldTension = lineEditor.getVertexAtIndex(0)->tension;
        lineEditor.setTensionAtIndex(0, spoonie::clamp(oldTension + 0.50f * -ev.delta.getY(), -100.0f, 100.0f));

        ui->setState("graph", lineEditor.serialize());

        repaint();

        return true;
    }

    void insertVertex(float x, float y)
    {
        vertexWidgets[lineEditor.getVertexCount()] = spoonie::Circle(x, y, absoluteVertexSize);
        lineEditor.insertVertex(x, y);

        ui->setState("graph", lineEditor.serialize());

        repaint();
    }

    bool leftClick(const MouseEvent &ev)
    {
        if (ev.press)
        {
            mouseDown = true;
            mouseDownLocation = ev.pos;
        }
        else
        {
            mouseDown = false;
        }

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
            const float y = spoonie::normalize(ev.pos.getY(), height);

            insertVertex(x, y);
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

    bool pointInCircle(spoonie::Circle circle, Point<float> point)
    {
        const float radius = circle.absoluteDiameter;

        const float x = point.getX() * getWidth();
        const float xo = circle.x * getWidth();

        const float dx = std::abs(x - xo);

        if (dx > radius)
            return false;

        const float y = point.getY() * getHeight();
        const float yo = circle.y * getHeight();

        const float dy = std::abs(y - yo);

        if (dy > radius)
            return false;

        if (dx + dy <= radius)
            return true;

        return dx * dx + dy * dy <= radius * radius;
    }

    bool onMotion(const MotionEvent &ev) override
    {
        const float x = (float)ev.pos.getX() / (float)getWidth();
        const float y = 1.0f - (float)ev.pos.getY() / (float)getHeight();
        const Point<float> point = Point<float>(x, y);

        for (int i = 0; i < lineEditor.getVertexCount(); ++i)
        {
            if (pointInCircle(vertexWidgets[i], point))
            {
                vertexWidgets[i].color = Color(200, 150, 150, 255);
                repaint();

                return true;
            }
        }

        return false;
    }

  private:
    bool mouseDown;
    Point<int> mouseDownLocation;

    spoonie::Graph lineEditor;
    spoonie::Circle vertexWidgets[spoonie::maxVertices];
    spoonie::Circle tensionHandles[spoonie::maxVertices - 1];

    const float absoluteVertexSize = 8.0f;
    UI *ui;
};

END_NAMESPACE_DISTRHO

#endif