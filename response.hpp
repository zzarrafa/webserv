#pragma once
#include <iostream>
#include <string>
#include "request.hpp"
#include "parser/server.hpp"
 #include <sys/stat.h>
 #include <sys/types.h>
 #include <dirent.h>
 #include<fstream>
#include<sstream>
#include "tools/tools.hpp"
# include <fcntl.h>
#include <stdio.h>
#include "parser/parsefile.hpp"


namespace WS{
class Response
{
private:
    int status_code;
    std::string status;
    int content_lenght;
    std::string content_type;
    std::string body;
    std::string header;
    
   
    


public:
    Response(/* args */);
    Response(server_config &server, request &req);
    ~Response();
    void set_status_code(int status);
    void set_content_lenght(int cl);
    void set_content_type(std::string ct);
    void set_status(std::string status);
    int get_status_code();
    int get_content_lenght();
    std::string get_status();
    std::string get_content_type();
   int search_for_path(server_config &s,std::string path);
    void writeToFd(int fd);
    bool isDir(std::string path);
    std::vector<std::string> getfiles();
    void autoindex(std::string path);
    void get_method(server_config &s,std::string path);
    void get_file(std::string file_name);
    void generate_errors();
    void delete_method(std::string filename);



    

};
}