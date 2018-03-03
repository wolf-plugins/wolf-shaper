#ifndef SPOONIE_MATHF_DEFINED_H
#define SPOONIE_MATHF_DEFINED_H

#include "Geometry.hpp"

#include <cmath>
#include <type_traits>
#include <algorithm>
#include <cassert>

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

//code adapted from ispc incoming

/*
  Copyright (c) 2010-2011, Intel Corporation
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.


   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
*/

/*
 * Compute the value 2^n, where the exponent is given as an integer.
 */
static double ipow2(int exponent)
{
    if (exponent < 0)
        return 1.0 / ipow2(-exponent);

    double ret = 1.0;

    while (exponent > 16)
    {
        ret *= 65536.0;
        exponent -= 16;
    }

    while (exponent-- > 0)
        ret *= 2.0;

    return ret;
}

/* 
 * Parse a hexadecimal-formatted floating-point number (C99 hex float constant-style). 
 */
static double parseHexFloat(const char *ptr, char **endPointer)
{
    DISTRHO_SAFE_ASSERT(ptr != NULL);
    DISTRHO_SAFE_ASSERT((ptr[0] == '0' && ptr[1] == 'X') || (ptr[0] == '-' && ptr[1] == '0' && ptr[2] == 'X'));

    double sign;

    if (ptr[0] == '-')
    {
        sign = -1.0;
        ptr += 3;
    }
    else
    {
        sign = 1.0;
        ptr += 2;
    }

    // Start initializing the mantissa
    DISTRHO_SAFE_ASSERT(*ptr == '0' || *ptr == '1');
    double mantissa = (*ptr == '1') ? 1.0 : 0.0;

    ++ptr;

    if (*ptr == '.')
    {
        // Is there a fraction part?  If so, the i'th digit we encounter
        // gives the 1/(16^i) component of the mantissa.
        ++ptr;

        double scale = 1.0 / 16.0;

        // Keep going until we come to the 'p', which indicates that we've
        // come to the exponent
        while (*ptr != 'P')
        {
            // Figure out the raw value from 0-15

            int digit;
            if (*ptr >= '0' && *ptr <= '9')
                digit = *ptr - '0';
            else if (*ptr >= 'A' && *ptr <= 'F')
                digit = 10 + *ptr - 'A';

            // And add its contribution to the mantissa
            mantissa += scale * digit;
            scale /= 16.0;
            ++ptr;
        }
    }
    else
    {
        // If there's not a '.', then we better be going straight to the
        // exponent
        DISTRHO_SAFE_ASSERT(*ptr == 'P');
    }

    ++ptr; // skip the 'p'

    // interestingly enough, the exponent is provided base 10..
    int exponent = (int)std::strtol(ptr, endPointer, 10);

    // Does stdlib exp2() guarantee exact results for integer n where can
    // be represented exactly as doubles?  I would hope so but am not sure,
    // so let's be sure.
    return sign * (mantissa * ipow2(exponent));
}
}

#endif