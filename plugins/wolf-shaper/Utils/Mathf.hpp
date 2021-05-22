#ifndef WOLF_MATHF_DEFINED_H
#define WOLF_MATHF_DEFINED_H

#include "Geometry.hpp"

#include <cmath>
#include <type_traits>
#include <algorithm>
#include <cassert>

START_NAMESPACE_DISTRHO

namespace wolf
{
template <typename T,
          typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
T clamp(const T &value, const T &min, const T &max)
{
    return std::max(min, std::min(value, max));
}

template <typename T>
T mirror(const T &value, const T &max)
{
    return max - value;
}

template <typename T>
T mirror(const T &value, const T &min, const T &max)
{
    return max - value + min;
}

float normalize(const float &value, const float &max);

template <typename T>
T normalize(const T &value, const T &min, const T &max)
{
    return (value - min) / (max - min);
}

template <typename T>
T normalizeBipolar(const T &value, const T &min, const T &max)
{
    return 2 * normalize(value, min, max) - 1;
}

inline int modulo(int a, int b) 
{
    const int result = a % b;
    return result >= 0 ? result : result + b;
}

template <typename T, typename U>
bool pointInCircle(DGL_NAMESPACE::Circle<T> circle, DGL_NAMESPACE::Point<U> point)
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
DGL_NAMESPACE::Point<T> flipY(const DGL_NAMESPACE::Point<T> point, const U height)
{
    const T x = point.getX();
    const T y = height - point.getY();

    return DGL_NAMESPACE::Point<T>(x, y);
}

float logScale(const float value, const float min, const float max);

float invLogScale(const float value, const float min, const float max);

float randomNumber(const float min, const float max);

float lerp(float a, float b, float f);

int doubleToRadixCString(char *buf, double value, int radix);

int toHexFloat(char *buffer, const double value);

double ipow2(int exponent);

double parseHexFloat(char const* ptr, char **endPointer);
}

END_NAMESPACE_DISTRHO

#endif