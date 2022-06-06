#ifndef CGI_HPP
#define CGI_HPP

#include "../webserv.hpp"

class Cgi
{
    public:
        static std::pair<std::string, std::map<std::string, std::string> > outtputt(std::string output);
        static std::vector<std::string> split_cgi(const std::string &str, const std::string &delimiter);
        static std::pair<std::string, std::map<std::string, std::string> > executi_cgi(request req, char **av, std::string output);
        static char **arguments(std::string cgi, std::string path);
        static std::pair<std::string, std::map<std::string, std::string> > execution(std::string path, request &req, std::string cgipathto);
};

#endif