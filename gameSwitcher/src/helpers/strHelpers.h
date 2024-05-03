
#ifndef STR_HELPERS_H
#define STR_HELPERS_H

#include <cstdlib>
#include <string>

bool strEndsWith(std::string str, std::string suffix)
{
    if (str.length() < suffix.length())
    {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

bool strStartsWith(std::string str, std::string prefix)
{
    if (str.length() < prefix.length())
    {
        return false;
    }
    return str.compare(0, prefix.length(), prefix) == 0;
}

#endif