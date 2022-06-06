#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "../webserv.hpp"

class Response;

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
bool    file_exists(std::string filename);
bool    is_path_exist(const std::string s);
bool    exists_test (const std::string& name);
bool    find_string(std::vector<std::string> vec, std::string str);
bool    is_valid_chunk(char *buf, int size, int debug) ;
bool    search_for_match(std::vector<std::string> vec1, std::vector<std::string> vec2);

std::map<int, int> switch_map(std::map<int, int> map);
std::vector<std::string> split(std::string str, char delim);
std::vector<Fileinfos>      listofFiles(std::string path);
std::vector<server_config> get_servers_with_same_port(parsefile &pf, int port);

std::string     &Trim(std::string& str, std::string& chars);
std::string     formatted_time(void);
std::string     get_file_type(std::string type);
std::string     get_file_name(std::string path, std::string prefix);
std::string     get_file_ext(std::string res);
std::string     remove_repeated_slashes(std::string path);
std::string     get_file_content(std::string path);
std::string     read_line(char *buffer, size_t size);

size_t  hex_to_int(std::string str);
off_t   fsize(const char *filename);
int     create_server(int port);
int     find_string_position(std::vector<std::string> vec, std::string str);
int     first_carriage_return(char *buf, int size);

void    print_binary(char *s, int len);
char    *get_buffer(size_t written, size_t len, std::string file, size_t *size);
char    *get_buffer_with_headers(Response *rep, size_t *size);


#endif