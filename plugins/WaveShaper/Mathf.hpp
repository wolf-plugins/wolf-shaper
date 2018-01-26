#ifndef SPOONIE_MATHF_DEFINED_H
#define SPOONIE_MATHF_DEFINED_H

namespace spoonie
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

float normalize(const float &value, const float &max)
{
    return value / max;
}

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
}

#endif