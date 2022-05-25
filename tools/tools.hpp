#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
 #include <sys/stat.h>
 #include <sys/types.h>
 #include <dirent.h>
 #include<fstream>
# include <fcntl.h>

typedef struct{

    std::string         file_name;
    std::string         date;
    std::string         size;
}   Fileinfos;

std::vector<std::string> split(std::string str, char delim);
std::string& Trim(std::string& str, std::string& chars);
bool is_method(std::string str);
bool is_ip(std::string str);
bool is_number(std::string str);
bool is_one_string(std::string str);
std::vector<Fileinfos>	listofFiles(std::string path);
std::string	formatted_time(void);
std::string     get_file_type(std::string type);

#endif