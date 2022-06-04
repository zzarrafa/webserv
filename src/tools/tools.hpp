#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "../webserv.hpp"

typedef     struct{
    std::string     file_name;
    std::string     date;
    std::string     size;
}           Fileinfos;

bool    is_method(std::string str);
bool    is_ip(std::string str);
bool    is_number(std::string str);
bool    is_one_string(std::string str);
bool    valid_hex(char c1, char c2);

std::vector<std::string> split(std::string str, char delim);
std::vector<Fileinfos>      listofFiles(std::string path);
std::string                 &Trim(std::string& str, std::string& chars);
std::string                 formatted_time(void);
std::string                 get_file_type(std::string type);
size_t      hex_to_int(std::string str);
off_t       fsize(const char *filename);
bool exists_test (const std::string& name);
bool find_string(std::vector<std::string> vec, std::string str);
std::string get_file_name(std::string path, std::string prefix);
int create_server(int port);
std::vector<server_config> get_servers_with_same_port(parsefile &pf, int port);
int find_string_position(std::vector<std::string> vec, std::string str);
std::map<int, int> switch_map(std::map<int, int> map);
std::string     get_file_ext(std::string res);
bool is_path_exist(const std::string s);
bool is_valid_chunk(char *buf, int size, int debug) ;
int first_carriage_return(char *buf, int size);
void print_binary(char *s, int len);
std::string read_line(char *buffer, size_t size);

#endif