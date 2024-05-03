
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

std::string strToUpper(const std::string &str)
{
    std::string result(str); // Create a copy of str
    for (char &c : result)
    {
        c = std::toupper(c);
    }
    return result;
}

std::string strToLower(const std::string &str)
{
    std::string result(str); // Create a copy of str
    for (char &c : result)
    {
        c = std::tolower(c);
    }
    return result;
}

#endif