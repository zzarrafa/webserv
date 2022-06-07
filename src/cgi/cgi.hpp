#ifndef CGI_HPP
#define CGI_HPP

#include "../webserv.hpp"

class Cgi
{
    public:
        static std::pair<std::string, std::map<std::string , std::string> > execution(std::string path, request &req, std::string cgipathto);

};

#endif