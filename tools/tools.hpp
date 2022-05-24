#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

std::vector<std::string> split(std::string str, char delim);
std::string& Trim(std::string& str, std::string& chars);
bool is_method(std::string str);
bool is_ip(std::string str);
bool is_number(std::string str);
bool is_one_string(std::string str);

#endif