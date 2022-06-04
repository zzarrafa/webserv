#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include "../webserv.hpp"

class Response
{
    private:
        int status_code;
        std::string status;
        size_t content_lenght;
        std::string content_type;
        std::string body;
        std::string header;
        std::string auto_index;
        size_t written;
        bool is_complete;

    public:
        Response();
        Response(server_config server, request &req);
        ~Response();
        // copy constructor
        Response(const Response &r);
        //assignment operator
        Response &operator=(const Response &r);

        void    set_status_code(int status);
        void     get_headers(std::string file_name);
        void    set_content_lenght(size_t cl);
        void    set_content_type(std::string ct);
        void    set_status(std::string status);
        int     get_status_code();
        size_t  get_content_lenght();
        std::string get_status();
        std::string get_content_type();
        std::string get_body();
        bool isDir(std::string path);
        std::vector<std::string> getfiles();
        void autoindex(std::string path, std::string prefix, std::string root);
        void get_method(server_config &s,std::string path);
        void generate_headers();
        void delete_method(std::string filename);
        // int search_for_path(server_config &s,std::string path);
        std::string get_header();
        void delete_method(server_config &s, std::string path);
        std::string get_file_path(std::string path, std::string prefix);
        int search_for_default(server_config &s,std::string path);
        void get_file(std::string file_name);
        void post_method(server_config &s, request &req);
        void    print_response();
        size_t  get_written();
        void    set_written(size_t written);
        void    set_is_complete(bool is_complete);
        bool    get_is_complete();
};


#endif