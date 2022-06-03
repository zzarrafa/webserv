#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <fcntl.h>
#include "../webserv.hpp"
#include "../http/request.hpp"
#include "../http/response.hpp"

class Cgi
{
    public:
        static std::pair<std::string, std::map<std::string , std::string> > execution(std::string path, request &req, std::string cgipathto);

};

#endif