
#include "tools.hpp"
std::vector<std::string> split(std::string &str, char delim)
{
    std::stringstream ss(str);
    std::vector<std::string> ret;
    std::string obj;

    while(std::getline(ss,obj,delim))
    {
        ret.push_back(obj);
    }
    return ret;
}

std::string& leftTrim(std::string& str, std::string& chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rightTrim(std::string& str, std::string& chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& Trim(std::string& str, std::string& chars)
{
    return (rightTrim(leftTrim(str, chars), chars));
}
