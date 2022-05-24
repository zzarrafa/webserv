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
        std::string _user_agent;
        std::string _host;
        std::string _type;
        std::string _body;
        std::string _encoding;
        int         _length;
        std::string _connection;
        std::map<std::string, std::string> _headers;

    public:
        request();
        request(std::ifstream &file);
        ~request();
        request(const request &src);
        request &operator=(const request &src);

        void set_method(std::string);
        void set_path(std::string);
        void set_version(std::string);
        void set_user_agent(std::string);
        void set_host(std::string);
        void set_type(std::string);
        void set_body(std::string);
        void set_encoding(std::string);
        void set_length(int);
        void set_connection(std::string);
        void add_headers(std::string, std::string);
        std::string get_method();
        std::string get_path();
        std::string get_version();
        std::string get_user_agent();
        std::string get_host();
        std::string get_type();
        std::string get_body();
        std::string get_encoding();
        std::string get_connection();
        int         get_length();
        std::map<std::string, std::string> get_headers();

        void print_request();
};


#endif