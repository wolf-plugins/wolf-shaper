#ifndef SPOONIE_GRAPH_WIDGET_HPP_INCLUDED
#define SPOONIE_GRAPH_WIDGET_HPP_INCLUDED

#include "Widget.hpp"
#include "Window.hpp"

#include "Graph.hpp"

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

        vertexWidgets[0] = DGL::Circle<int>(0, 0, absoluteVertexSize);
        vertexWidgets[1] = DGL::Circle<int>(getWidth(), getHeight(), absoluteVertexSize);

        for (int i = 2; i < spoonie::maxVertices; ++i)
        {
            vertexWidgets[i] = DGL::Circle<int>(0, 0, absoluteVertexSize);
        }
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
    }

    template <typename T>
    DGL::Point<T> flipY(const DGL::Point<T> point)
    {
        const T x = point.getX();
        const T y = getHeight() - point.getY();

        return DGL::Point<T>(x, y);
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

        moveTo(0.0f, 0.0f);

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
        const DGL::Circle<int> vertex = vertexWidgets[index];

        const int posX = vertex.getX();
        const int posY = vertex.getY();
        const float size = vertex.getSize();

        beginPath();

        if (grabbedWidget != nullptr && vertex == *grabbedWidget)
            fillColor(Color(255, 100, 100, 255));
        else
            fillColor(Color(255, 255, 255, 255));

        strokeWidth(2.0f);
        strokeColor(Color(0, 0, 0, 255));

        circle(posX, posY, size);

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

    void hoverCircle(DGL::Circle<int> *circle, int index)
    {
        hoveredWidget = circle;
        focusedWidgetIndex = index;
    }

    void insertVertex(DGL::Point<int> pos)
    {
        int i = lineEditor.getVertexCount();

        while ((i > 0) && (pos.getX() < vertexWidgets[i - 1].getX()))
        {
            vertexWidgets[i] = vertexWidgets[i - 1];
            --i;
        }

        vertexWidgets[i] = DGL::Circle<int>(pos.getX(), pos.getY(), absoluteVertexSize);
        hoverCircle(&vertexWidgets[i], i);

        const float width = getWidth();
        const float height = getHeight();

        const float x = spoonie::normalize(pos.getX(), width);
        const float y = spoonie::normalize(pos.getY(), height);

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

            if (hoveredWidget)
            {
                grabbedWidget = hoveredWidget;
                hoveredWidget = nullptr;

                expandCircle(grabbedWidget);
            }
        }
        else
        {
            mouseDown = false;

            if (grabbedWidget)
            {
                hoveredWidget = grabbedWidget;
                grabbedWidget = nullptr;

                shrinkCircle(hoveredWidget);
            }
        }

        return true;
    }

    bool middleClick(const MouseEvent &ev)
    {
        return true;
    }

    bool rightClick(const MouseEvent &ev)
    {
        const Point<int> point = flipY(ev.pos);

        if (ev.press)
        {
            insertVertex(point);
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

    template <typename T, typename U>
    bool pointInCircle(DGL::Circle<T> circle, DGL::Point<U> point)
    {
        const float radius = circle.getSize();

        const T x = point.getX();
        const T xo = circle.getX();

        const T dx = std::abs(x - xo);

        if (dx > radius)
            return false;

        const T y = point.getY();
        const T yo = circle.getY();

        const T dy = std::abs(y - yo);

        if (dy > radius)
            return false;

        if (dx + dy <= radius)
            return true;

        return dx * dx + dy * dy <= radius * radius;
    }

    void expandCircle(DGL::Circle<int> *circle)
    {
        circle->setSize(absoluteVertexSize + 1.0f);

        repaint();
    }

    void shrinkCircle(DGL::Circle<int> *circle)
    {
        circle->setSize(absoluteVertexSize);

        repaint();
    }

    bool onMotion(const MotionEvent &ev) override
    {
        const Point<int> point = flipY(ev.pos);

        if (hoveredWidget)
        {
            if (!pointInCircle(*hoveredWidget, point))
            {
                shrinkCircle(hoveredWidget);
                hoveredWidget = nullptr;

                focusedWidgetIndex = -1;
            }
        }
        else if (grabbedWidget)
        {
            const Circle<int> left = vertexWidgets[focusedWidgetIndex - 1];
            const Circle<int> right = vertexWidgets[focusedWidgetIndex + 1];

            const int clampedX = spoonie::clamp(point.getX(), left.getX(), right.getX());

            grabbedWidget->setX(clampedX);
            grabbedWidget->setY(point.getY());

            const float normalizedX = clampedX / (float)getWidth();
            const float normalizedY = point.getY() / (float)getHeight();

            lineEditor.getVertexAtIndex(focusedWidgetIndex)->setPosition(normalizedX, normalizedY);

            repaint();
        }
        else //check for hovered widget
        {
            for (int i = 0; i < lineEditor.getVertexCount(); ++i)
            {
                if (i < lineEditor.getVertexCount() - 1)
                {
                    if (pointInCircle(tensionWidgets[i], point))
                    {
                        hoverCircle(&tensionWidgets[i], i);

                        return true;
                    }
                }
                if (pointInCircle(vertexWidgets[i], point))
                {
                    hoverCircle(&vertexWidgets[i], i);

                    return true;
                }
            }
        }

        return false;
    }

  private:
    bool mouseDown;
    Point<int> mouseDownLocation;

    spoonie::Graph lineEditor;
    DGL::Circle<int> vertexWidgets[spoonie::maxVertices];
    DGL::Circle<int> tensionWidgets[spoonie::maxVertices - 1];

    DGL::Circle<int> *hoveredWidget = nullptr;
    DGL::Circle<int> *grabbedWidget = nullptr;
    int focusedWidgetIndex = -1;

    const float absoluteVertexSize = 8.0f;
    UI *ui;
};

END_NAMESPACE_DISTRHO

#endif