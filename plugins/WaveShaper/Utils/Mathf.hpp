#ifndef SPOONIE_MATHF_DEFINED_H
#define SPOONIE_MATHF_DEFINED_H

namespace spoonie
{
template <typename T,
          typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
static T clamp(const T &value, const T &min, const T &max)
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
static bool pointInCircle(DGL::Circle<T> circle, DGL::Point<U> point)
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
}

#endif