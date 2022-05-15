#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <iostream>

class location_config
{
	private:
		std::vector<std::string>	methods;
		std::string					redirect;
		std::string					path;
		bool						autoindex;
		std::string					default_file;
		std::string					cgi_path;
		std::string					upload_path;

	public:
		location_config();
		~location_config();
		location_config(const location_config &src);
		location_config &operator=(const location_config &src);

		std::string					get_redirect();
		std::vector<std::string>	get_methods();
		std::string					get_path();
		bool						get_autoindex();
		std::string					get_default_file();
		std::string					get_cgi_path();
		std::string					get_upload_path();
		void set_redirect(std::string redirect);
		void set_methods(std::vector<std::string> methods);
		void set_path(std::string path);
		void set_autoindex(bool autoindex);
		void set_default_file(std::string default_file);
		void set_cgi_path(std::string cgi_path);
		void set_upload_path(std::string upload_path);
};

class server_config
{
	private:
		int					port;
		std::string			host;
		std::string			error_page;
		int					max_body_size;
		std::vector<std::string> servers;
		std::vector<location_config> locations;

	public:
		server_config();
		~server_config();
		server_config(const server_config &src);
		server_config &operator=(const server_config &src);

		void set_port(int);
		void set_host(std::string);
		void set_error_page(std::string);
		void set_max_body_size(int);
		void set_servers(std::vector<std::string>);
		int get_port();
		std::string get_host();
		std::string get_error_page();
};

#endif