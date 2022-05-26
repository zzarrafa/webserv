#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "webserv.hpp"
#include <map>

class request
{
private:
    std::string _method;
    std::string _path;
    std::string _version;
    std::string _host;
    std::string _type;
    std::string _body;
    std::string _encoding;
    size_t      _length;
    std::string _connection;
    std::map<std::string, std::string> _headers;
    bool _is_complete;

public:
    request();
    ~request();
    request(const request &src);
    request &operator=(const request &src);
    request(std::string &file_name);

    void set_method(std::string);
    void set_path(std::string);
    void set_version(std::string);
    void set_host(std::string);
    void set_type(std::string);
    void set_body(std::string);
    void set_encoding(std::string);
    void set_length(size_t);
    void set_connection(std::string);
    void add_headers(std::string, std::string);
    std::string get_method();
    std::string get_path();
    std::string get_version();
    std::string get_host();
    std::string get_type();
    std::string get_body();
    std::string get_encoding();
    std::string get_connection();
    size_t get_length();
    bool get_is_complete();
    std::map<std::string, std::string> get_headers();
    void print_request();
    std::string parse_chunked_body(std::ifstream &file);
};

#endif