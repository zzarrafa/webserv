#include "cgi.hpp"

std::pair<std::string, std::map<std::string, std::string> > Cgi::execution(std::string path, request &req, std::string cgipathto)
{  
    std::cout << " HIHIHIHIHI " << req.get_header("Content-type") << std::endl;
    //if(req.get_method() == "POST")
      //  if (!req.get_header("Content-type").empty())
        

}
