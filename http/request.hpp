#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../webserv.hpp"

class request
{
private:
    std::string                         _method;
    std::string                         _path;
    std::string                         _version;
    std::string                         _host;
    std::string                         _type;
    std::string                         _body;
    std::string                         _encoding;
    size_t                              _length;
    std::string                         _connection;
    std::map<std::string, std::string>  _headers;
    bool                                _is_complete;
    int                                 _headers_len;
    size_t                              _chunk;
    size_t                              _offset;

public:
    request();
    ~request();
    request(const request &src);
    request &operator=(const request &src);
    request(char *buffer, int ret);

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

    std::map<std::string, std::string> get_headers();
    std::string parse_chunked_body(std::string str, int len);
    bool    get_is_complete();
    void    print_request();
    size_t  get_length();
    int     read_hex(char *buff, int *len);
    int     find_char(char *buffer, char c);
    void    fill_body(char *buffer, int flag, int ret);
    void    check_if_complete();
    char    *clean_buffer(char *buffer, int ret, int *counter);
};

#endif