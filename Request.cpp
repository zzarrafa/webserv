#include "Request.hpp"
#include <iostream>
#include <string>

WS::Request::Request() : buff() , method(), url(),version(0) ,body()
{

}

WS::Request::~Request()
{

}
 void WS::Request::ParseHeader(std::string buff)
{
    //  membuf sbuf(buff, buff + sizeof(buff));
    // std::istream in(&sbuf);
    std::stringstream ss(buff);
    std::string str;
	std::getline(ss, str);
	std::vector<std::string> line = split(str, ' ');

	method = line[0];
	url = line[1];
	version = std::stof(split(line[2], '/')[1]);

    while (std::getline(ss,str))
    {
        std::vector<std::string> vec = split(str,':');

        headers.insert(std::make_pair(vec[0],vec[1]));
    }
    

}
std::string WS::Request::get_method()
{
    return method;
}
std::string WS::Request::get_url()
{
    return url;
}
float WS::Request::get_version()
{
    return version;
}