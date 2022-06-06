#ifndef SERVER_HPP
#define SERVER_HPP

# include "../webserv.hpp"

class location_config
{
	private:
		std::string					prefix;
		std::vector<std::string>	methods;
		std::vector<std::string>	redirect;
		std::string					root;
		std::string					autoindex;
		std::string					default_file;
		std::string					upload_path;

	public:
		location_config();
		~location_config();
		location_config(const location_config &src);
		location_config &operator=(const location_config &src);

		std::vector<std::string>	get_redirect();
		std::vector<std::string>	get_methods();
		std::string					get_root();
		std::string					get_autoindex();
		std::string					get_default_file();
		std::string					get_cgi_path();
		std::string					get_upload_path();
		std::string					get_prefix();
		void 	set_redirect(std::vector<std::string> redirect);
		void 	set_methods(std::vector<std::string> methods);
		void 	set_root(std::string path);
		void 	set_autoindex(std::string autoindex);
		void 	set_default_file(std::string default_file);
		void 	set_cgi_path(std::string cgi_path);
		void 	set_upload_path(std::string upload_path);
		void 	set_prefix(std::string prefix);
		void	print_location();
};

class server_config
{
	private:
		int					port;
		std::string			host;
		std::string			error_page;
		size_t				max_body_size;
		int					fd_socket;
		std::vector<std::string>			servers;
		std::vector<location_config>		locations;
		std::map<std::string, std::string>	cgi_path;

	public:
		server_config();
		~server_config();
		server_config(const server_config &src);
		server_config &operator=(const server_config &src);

		void	set_port(int);
		void	set_host(std::string);
		void	set_error_page(std::string);
		void	set_max_body_size(size_t);
		void	set_servers(std::vector<std::string>);
		void	set_locations(std::vector<location_config>);
		size_t	get_max_body_size();
		std::string		get_host();
		std::string		get_error_page();
		std::vector<std::string>		get_servers();
		std::vector<location_config>	get_locations();
		int		get_port();
		void	add_location(location_config src);
		void	check_config();
		int		create_server(int port);
		int		get_fd_socket();
		int		longest_match(std::string str, std::string needle);
		void	print_server();
		void	add_cgi_path(std::string key, std::string value);
		location_config			longest_prefix_match(std::string);
		std::map<std::string, std::string>	get_cgi_path();
};

server_config get_server_by_host(std::vector<server_config> servers, std::string host);

#endif