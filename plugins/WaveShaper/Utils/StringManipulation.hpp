#ifndef SPOONIE_STRING_MANIPULATION_DEFINED_H
#define SPOONIE_STRING_MANIPULATION_DEFINED_H

#include <vector>
#include <string>
#include <cassert>

namespace spoonie
{
std::vector<std::string> split(const char *str, char delimiter)
{
    assert(str != nullptr);

    std::vector<std::string> result;

    do
    {
        const char *begin = str;

        while (*str != delimiter && *str)
            ++str;

        result.push_back(std::string(begin, str));
    } while (0 != *str++);

    return result;
}

/**
 * Take characters from a string until a certain character is found. 
 */
std::string takeUntil(const char *str, char stopCharacter, char const **rest = nullptr)
{
    assert(*str != '\0');

    const char *begin = str;

    while (*str != stopCharacter && *str)
        ++str;

    if(rest != nullptr)
        *rest = str;

    return std::string(begin, str);
}

/**
 * Take a string containing a tuple in the form (1,2,3,n) and return its elements.
 */
std::vector<std::string> tupleToVector(const char *tuple)
{
    if (tuple == nullptr || *tuple == '\0')
        return std::vector<std::string>();

    std::vector<std::string> result;
    int numberOfElements = 0;

    while (*tuple != '(' && *tuple)
        ++tuple;

    if (*tuple == '\0' || *(++tuple) == '\0')
        return std::vector<std::string>();

    do
    {
        result.push_back("");

        while (*tuple != ',' && *tuple != ')' && *tuple)
        {
            if (isalnum(*tuple))
                result[numberOfElements] += *tuple;

            ++tuple;
        }

        if (*tuple == '\0')
            return std::vector<std::string>();

        ++numberOfElements;

        if(*tuple == ')')
            break;

        ++tuple;

    } while (*tuple);

    if(*tuple == '\0')
        return std::vector<std::string>();

    return result;
}
}

#endif