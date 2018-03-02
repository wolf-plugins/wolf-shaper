#ifndef SPOONIE_STRING_MANIPULATION_DEFINED_H
#define SPOONIE_STRING_MANIPULATION_DEFINED_H

#include <vector>
#include <string>

namespace spoonie 
{
std::vector<std::string> split(const char *str, char delimiter)
{
    std::vector<std::string> result;

    do
    {
        const char *begin = str;

        while(*str != delimiter && *str)
            ++str;

        result.push_back(std::string(begin, str));
    } while (0 != *str++);

    return result;
}

std::string takeUntil(const char *str, char stopCharacter)
{
    const char *begin = str;

    while(*str != stopCharacter && *str)
        ++str;

    return std::string(begin, str);
}

}

#endif