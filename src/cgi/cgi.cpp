#include "cgi.hpp"

std::pair<std::string, std::map<std::string, std::string> > Cgi::execution(std::string path, request &req, std::string cgipathto)
{  
    std::cout << "  " << req.get_header("Content-type") << std::endl;
    if(req.get_method() == "POST")
    {
      if (!req.get_header("Content-Type").empty())
        setenv("CONTENT_TYPE", req.get_header("Content-Type").c_str(),1);
    }
    if(!req.get_body().empty())
      setenv("CONTENT_LENGTH", std::to_string(req.get_body().size()).c_str(),1);
    if (!req.get_method().empty())
    setenv("REQUEST_METHOD",req.get_method().c_str(),1 );
    setenv("REDIRECT_STATUS", "true" , 1);
    setenv("SCRIPT_FILENAME", path.c_str(), 1);
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("HTTP_ACCEPT_ENCODING","",0);
    setenv("HTTP_ACCEPT", "" ,0);
    setenv("HTTP_ACCEPT_LANGUAGE","en-US,en;q=0.9",0);
    //bonus
    if (!req.get_header("cookie").empty())
    setenv("cookie", req.get_header("cookie").c_str(),1);
    setenv("SERVER_PROTOCOL","HTTP/1.1",0);
    setenv("REQUEST_SCHEME","http",0);
}

