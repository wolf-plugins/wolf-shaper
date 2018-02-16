#include "NanoSlider.hpp"

START_NAMESPACE_DISTRHO

NanoSlider::NanoSlider(Window& parent, Size<uint> size) noexcept
    : NanoWidget(parent),
      fMinimum(0.0f),
      fMaximum(1.0f),
      fStep(0.0f),
      fValue(0.5f),
      fValueTmp(fValue),
      fDragging(false),
      fInverted(false),
      fValueIsSet(false),
      fStartedX(0),
      fStartedY(0),
      fCallback(nullptr),
      fStartPos(),
      fEndPos(),
      fSliderArea()
{
	setSize(size);
}

NanoSlider::NanoSlider(Widget* widget, Size<uint> size) noexcept
    : NanoWidget(widget->getParentWindow()),
      fMinimum(0.0f),
      fMaximum(1.0f),
      fStep(0.0f),
      fValue(0.5f),
      fValueTmp(fValue),
      fDragging(false),
      fInverted(false),
      fValueIsSet(false),
      fStartedX(0),
      fStartedY(0),
      fCallback(nullptr),
      fStartPos(),
      fEndPos(),
      fSliderArea()
{
	setSize(size);
}

float NanoSlider::getValue() const noexcept
{
    return fValue;
}

void NanoSlider::setValue(float value, bool sendCallback) noexcept
{
    if (! fValueIsSet)
        fValueIsSet = true;

    if (d_isEqual(fValue, value))
        return;

    fValue = value;

    if (d_isZero(fStep))
        fValueTmp = value;

    repaint();

    if (sendCallback && fCallback != nullptr)
    {
        try {
            fCallback->nanoSliderValueChanged(this, fValue);
        } DISTRHO_SAFE_EXCEPTION("NanoSlider::setValue");
    }
}

void NanoSlider::setStartPos(const Point<int>& startPos) noexcept
{
    fStartPos = startPos;
    _recheckArea();
}

void NanoSlider::setStartPos(int x, int y) noexcept
{
    setStartPos(Point<int>(x, y));
}

void NanoSlider::setEndPos(const Point<int>& endPos) noexcept
{
    fEndPos = endPos;
    _recheckArea();
}

void NanoSlider::setEndPos(int x, int y) noexcept
{
    setEndPos(Point<int>(x, y));
}

void NanoSlider::setInverted(bool inverted) noexcept
{
    if (fInverted == inverted)
        return;

    fInverted = inverted;
    repaint();
}

void NanoSlider::setRange(float min, float max) noexcept
{
    fMinimum = min;
    fMaximum = max;

    if (fValue < min)
    {
        fValue = min;
        repaint();

        if (fCallback != nullptr && fValueIsSet)
        {
            try {
                fCallback->nanoSliderValueChanged(this, fValue);
            } DISTRHO_SAFE_EXCEPTION("NanoSlider::setRange < min");
        }
    }
    else if (fValue > max)
    {
        fValue = max;
        repaint();

        if (fCallback != nullptr && fValueIsSet)
        {
            try {
                fCallback->nanoSliderValueChanged(this, fValue);
            } DISTRHO_SAFE_EXCEPTION("NanoSlider::setRange > max");
        }
    }
}

void NanoSlider::setStep(float step) noexcept
{
    fStep = step;
}

void NanoSlider::setCallback(Callback* callback) noexcept
{
    fCallback = callback;
}

void NanoSlider::onNanoDisplay()
{
#if 0 // DEBUG, paints slider area
    glColor3f(0.4f, 0.5f, 0.1f);
    glRecti(fSliderArea.getX(), fSliderArea.getY(), fSliderArea.getX()+fSliderArea.getWidth(), fSliderArea.getY()+fSliderArea.getHeight());
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
#endif

    const float normValue = (fValue - fMinimum) / (fMaximum - fMinimum);

    int x, y;

    if (fStartPos.getY() == fEndPos.getY())
    {
        // horizontal
        if (fInverted)
            x = fEndPos.getX() - static_cast<int>(normValue*static_cast<float>(fEndPos.getX()-fStartPos.getX()));
        else
            x = fStartPos.getX() + static_cast<int>(normValue*static_cast<float>(fEndPos.getX()-fStartPos.getX()));

        y = fStartPos.getY();
    }
    else
    {
        // vertical
        x = fStartPos.getX();

        if (fInverted)
            y = fEndPos.getY() - static_cast<int>(normValue*static_cast<float>(fEndPos.getY()-fStartPos.getY()));
        else
            y = fStartPos.getY() + static_cast<int>(normValue*static_cast<float>(fEndPos.getY()-fStartPos.getY()));
    }

    //fImage.drawAt(x, y);
}

bool NanoSlider::onMouse(const MouseEvent& ev)
{
    if (ev.button != 1)
        return false;

    if (ev.press)
    {
        if (! fSliderArea.contains(ev.pos))
            return false;

        float vper;
        const int x = ev.pos.getX();
        const int y = ev.pos.getY();

        if (fStartPos.getY() == fEndPos.getY())
        {
            // horizontal
            vper = float(x - fSliderArea.getX()) / float(fSliderArea.getWidth());
        }
        else
        {
            // vertical
            vper = float(y - fSliderArea.getY()) / float(fSliderArea.getHeight());
        }

        float value;

        if (fInverted)
            value = fMaximum - vper * (fMaximum - fMinimum);
        else
            value = fMinimum + vper * (fMaximum - fMinimum);

        if (value < fMinimum)
        {
            fValueTmp = value = fMinimum;
        }
        else if (value > fMaximum)
        {
            fValueTmp = value = fMaximum;
        }
        else if (d_isNotZero(fStep))
        {
            fValueTmp = value;
            const float rest = std::fmod(value, fStep);
            value = value - rest + (rest > fStep/2.0f ? fStep : 0.0f);
        }

        fDragging = true;
        fStartedX = x;
        fStartedY = y;

        if (fCallback != nullptr)
            fCallback->nanoSliderDragStarted(this);

        setValue(value, true);

        return true;
    }
    else if (fDragging)
    {
        if (fCallback != nullptr)
            fCallback->nanoSliderDragFinished(this);

        fDragging = false;
        return true;
    }

    return false;
}

bool NanoSlider::onMotion(const MotionEvent& ev)
{
    if (! fDragging)
        return false;

    const bool horizontal = fStartPos.getY() == fEndPos.getY();
    const int x = ev.pos.getX();
    const int y = ev.pos.getY();

    if ((horizontal && fSliderArea.containsX(x)) || (fSliderArea.containsY(y) && ! horizontal))
    {
        float vper;

        if (horizontal)
        {
            // horizontal
            vper = float(x - fSliderArea.getX()) / float(fSliderArea.getWidth());
        }
        else
        {
            // vertical
            vper = float(y - fSliderArea.getY()) / float(fSliderArea.getHeight());
        }

        float value;

        if (fInverted)
            value = fMaximum - vper * (fMaximum - fMinimum);
        else
            value = fMinimum + vper * (fMaximum - fMinimum);

        if (value < fMinimum)
        {
            fValueTmp = value = fMinimum;
        }
        else if (value > fMaximum)
        {
            fValueTmp = value = fMaximum;
        }
        else if (d_isNotZero(fStep))
        {
            fValueTmp = value;
            const float rest = std::fmod(value, fStep);
            value = value - rest + (rest > fStep/2.0f ? fStep : 0.0f);
        }

        setValue(value, true);
    }
    else if (horizontal)
    {
        if (x < fSliderArea.getX())
            setValue(fInverted ? fMaximum : fMinimum, true);
        else
            setValue(fInverted ? fMinimum : fMaximum, true);
    }
    else
    {
        if (y < fSliderArea.getY())
            setValue(fInverted ? fMaximum : fMinimum, true);
        else
            setValue(fInverted ? fMinimum : fMaximum, true);
    }

    return true;
}

void NanoSlider::_recheckArea() noexcept
{
    if (fStartPos.getY() == fEndPos.getY())
    {
        // horizontal
        fSliderArea = Rectangle<int>(fStartPos.getX(),
                                     fStartPos.getY(),
                                     fEndPos.getX() + static_cast<int>(getWidth()) - fStartPos.getX(),
                                     static_cast<int>(getHeight()));
    }
    else
    {
        // vertical
        fSliderArea = Rectangle<int>(fStartPos.getX(),
                                     fStartPos.getY(),
                                     static_cast<int>(getWidth()),
                                     fEndPos.getY() + static_cast<int>(getHeight()) - fStartPos.getY());
    }
}

END_NAMESPACE_DISTRHO