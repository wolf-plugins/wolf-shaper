#include "Geometry.hpp"

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

float normalize(const float &value, const float &max)
{
    return value / max;
}

float logScale(const float value, const float min, const float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;

    const float b = std::log(max / min) / (max - min);
    const float a = max / std::exp(max * b);

    return a * std::exp(b * value);
}

float invLogScale(const float value, const float min, const float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;

    const float b = std::log(max / min) / (max - min);
    const float a = max / std::exp(max * b);

    return std::log(value / a) / b;
}

float randomNumber(const float min, const float max)
{
    return ((float)std::rand() / (float)RAND_MAX * (max - min)) + min;
}

float lerp(float a, float b, float f)
{
    f = clamp(f, 0.0f, 1.0f);

    return a * (1.0 - f) + (b * f);
}

/* Hexfloat stuff------------------------------------- */

// Adapted from https://cs.chromium.org/chromium/src/v8/src/conversions.cc
// Copyright 2015 The Chromium Authors. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

int doubleToRadixCString(char *buf, double value, int radix)
{
    const char chars[] = "0123456789abcdefghijklmnopqrstuvwxyz";

    // Temporary buffer for the result. We start with the decimal point in the
    // middle and write to the left for the integer part and to the right for the
    // fractional part. 1024 characters for the exponent and 52 for the mantissa
    // either way, with additional space for sign, decimal point and string
    // termination should be sufficient.
    const int kBufferSize = 2200;
    char buffer[kBufferSize];

    int integer_cursor = kBufferSize / 2;
    int fraction_cursor = integer_cursor;

    bool negative = value < 0;

    if (negative)
        value = -value;

    // Split the value into an integer part and a fractional part.
    double integer = std::floor(value);
    double fraction = value - integer;

    // We only compute fractional digits up to the input double's precision.
    double delta = 0.0;

    if (fraction > delta)
    {
        // Insert decimal point.
        buffer[fraction_cursor++] = '.';
        do
        {
            // Shift up by one digit.
            fraction *= radix;
            delta *= radix;

            // Write digit.
            int digit = static_cast<int>(fraction);
            buffer[fraction_cursor++] = chars[digit];

            // Calculate remainder.
            fraction -= digit;

            // Round to even.
            if (fraction > 0.5 || (fraction == 0.5 && (digit & 1)))
            {
                if (fraction + delta > 1)
                {
                    // We need to back trace already written digits in case of carry-over.
                    while (true)
                    {
                        fraction_cursor--;
                        if (fraction_cursor == kBufferSize / 2)
                        {
                            // Carry over to the integer part.
                            integer += 1;
                            break;
                        }
                        char c = buffer[fraction_cursor];

                        // Reconstruct digit.
                        int digit = c > '9' ? (c - 'a' + 10) : (c - '0');

                        if (digit + 1 < radix)
                        {
                            buffer[fraction_cursor++] = chars[digit + 1];
                            break;
                        }
                    }
                    break;
                }
            }
        } while (fraction > delta);
    }

    int exponent;
    std::frexp(integer / radix, &exponent);

    // Compute integer digits. Fill unrepresented digits with zero.
    while (exponent > 0)
    {
        integer /= radix;
        buffer[--integer_cursor] = '0';
        std::frexp(integer / radix, &exponent);
    }

    do
    {
        const double rem = std::fmod(integer, radix);
        buffer[--integer_cursor] = chars[static_cast<int>(rem)];
        integer = (integer - rem) / radix;
    } while (integer > 0);

    // Add sign and terminate string.
    if (negative)
        buffer[--integer_cursor] = '-';

    buffer[fraction_cursor++] = '\0';

    const int len = fraction_cursor - integer_cursor;

    memcpy(buf, buffer + integer_cursor, len * sizeof(char) + 1);

    return len - 1;
}

//Adapted from https://github.com/dankogai/js-hexfloat
/*Copyright (c) 2016 Dan Kogai

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

int toHexFloat(char *buffer, const double value)
{
    if (value == +0.0)
    {
        return std::sprintf(buffer, "0x0p+0");
    }
    else if (value == -0.0)
    {
        return std::sprintf(buffer, "-0x0p+0");
    }

    const char sign = value < 0 ? '-' : '\0';

    double hexDigits = std::abs(value);
    int exponent = 0;

    if (hexDigits < 1)
    {
        while (hexDigits < 1)
        {
            hexDigits *= 2;
            --exponent;
        }
    }
    else
    {
        while (hexDigits >= 2)
        {
            hexDigits /= 2;
            ++exponent;
        }
    }

    const char exponentSign = exponent < 0 ? '\0' : '+';

    int length;

    if (sign)
        length = std::sprintf(buffer, "%c0x", sign);
    else
        length = std::sprintf(buffer, "0x");

    length += doubleToRadixCString(buffer + length, hexDigits, 16);

    if (exponentSign)
        return length + std::sprintf(buffer + length, "p%c%d", exponentSign, exponent);
    else
        return length + std::sprintf(buffer + length, "p%d", exponent);
}

//following functions adapted from ispc
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
double ipow2(int exponent)
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
double parseHexFloat(char const *ptr, char **endPointer)
{
    DISTRHO_SAFE_ASSERT_RETURN(ptr != NULL, 0);
    DISTRHO_SAFE_ASSERT_RETURN((ptr[0] == '0' && ptr[1] == 'x') || (ptr[0] == '-' && ptr[1] == '0' && ptr[2] == 'x'), 0);

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
    DISTRHO_SAFE_ASSERT_RETURN(*ptr == '0' || *ptr == '1', 0);
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
        while (*ptr != 'p')
        {
            // Figure out the raw value from 0-15

            int digit;

            if (*ptr >= '0' && *ptr <= '9')
            {
                digit = *ptr - '0';
            }
            else if (*ptr >= 'a' && *ptr <= 'f')
            {
                digit = 10 + *ptr - 'a';
            }
            else
            {
                fprintf(stderr, "Error while parsing hexfloat: invalid digit");
                return 0;
            }

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
        DISTRHO_SAFE_ASSERT_RETURN(*ptr == 'p', 0);
    }

    ++ptr; // skip the 'p'

    // interestingly enough, the exponent is provided base 10..
    int exponent = (int)std::strtol(ptr, endPointer, 10);

    // Does stdlib exp2() guarantee exact results for integer n where can
    // be represented exactly as doubles?  I would hope so but am not sure,
    // so let's be sure.
    return sign * (mantissa * ipow2(exponent));
}
} // namespace wolf

END_NAMESPACE_DISTRHO
