#ifndef SPOONIE_MATHF_DEFINED_H
#define SPOONIE_MATHF_DEFINED_H

namespace spoonie
{
template <typename T,
          typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
static T clamp(const T &value, const T min, const T &max)
{
    return std::max(min, std::min(value, max));
}

template <typename T>
static T mirror(const T &value, const T &max)
{
    return max - value;
}

template <typename T>
static T mirror(const T &value, const T &min, const T &max)
{
    return max - value + min;
}

static float normalize(const float &value, const float &max)
{
    return value / max;
}

template <typename T>
static T normalize(const T &value, const T &min, const T &max)
{
    return (value - min) / (max - min);
}

template <typename T>
static T normalizeBipolar(const T &value, const T &min, const T &max)
{
    return 2 * normalize(value, min, max) - 1;
}

template <typename T, typename U>
static bool pointInCircle(DGL_NAMESPACE::Circle<T> circle, DGL_NAMESPACE::Point<U> point)
{
    //source: https://stackoverflow.com/questions/481144/equation-for-testing-if-a-point-is-inside-a-circle
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

template <typename T, typename U,
          typename = typename std::enable_if<std::is_arithmetic<U>::value, U>::type>
static DGL_NAMESPACE::Point<T> flipY(const DGL_NAMESPACE::Point<T> point, const U height)
{
    const T x = point.getX();
    const T y = height - point.getY();

    return DGL_NAMESPACE::Point<T>(x, y);
}

static float logScale(const float value, const float min, const float max)
{
    const float b = std::log(max / min) / (max - min);
    const float a = max / std::exp(max * b);

    return a * std::exp(b * value);
}

static float invLogScale(const float value, const float min, const float max)
{
    const float b = std::log(max / min) / (max - min);
    const float a = max / std::exp(max * b);

    return std::log(value / a) / b;
}

}

#endif