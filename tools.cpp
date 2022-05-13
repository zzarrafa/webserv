
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