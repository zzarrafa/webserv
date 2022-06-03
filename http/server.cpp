#include "../webserv.hpp"

// location
// constructor
location_config::location_config()
{
	this->root = "";
	this->redirect = std::vector<std::string>();
	this->methods = std::vector<std::string>();
	this->autoindex = "";
	this->default_file = "";
	this->cgi_path = "";
	this->upload_path = "";
	this->prefix = "";
}
//copy constructor
location_config::location_config(const location_config &src)
{
	this->root = src.root;
	this->redirect = src.redirect;
	this->methods = src.methods;
	this->autoindex = src.autoindex;
	this->default_file = src.default_file;
	this->cgi_path = src.cgi_path;
	this->upload_path = src.upload_path;
	this->prefix = src.prefix;
}
//assignment operator
location_config &location_config::operator=(const location_config &src)
{
	this->root = src.root;
	this->redirect = src.redirect;
	this->methods = src.methods;
	this->autoindex = src.autoindex;
	this->default_file = src.default_file;
	this->cgi_path = src.cgi_path;
	this->upload_path = src.upload_path;
	this->prefix = src.prefix;
	return *this;
}

//destructor
location_config::~location_config()
{
}

//getters
std::vector<std::string> location_config::get_redirect()
{
	return (this->redirect);
}

std::vector<std::string> location_config::get_methods()
{
	return (this->methods);
}

std::string location_config::get_root()
{
	return (this->root);
}

std::string location_config::get_autoindex()
{
	return (this->autoindex);
}

std::string location_config::get_default_file()
{
	return (this->default_file);
}

std::string location_config::get_cgi_path()
{
	return (this->cgi_path);
}

std::string location_config::get_upload_path()
{
	return (this->upload_path);
}

std::string location_config::get_prefix()
{
	return (this->prefix);
}
//setters
void location_config::set_redirect(std::vector<std::string> redirect)
{
	this->redirect = redirect;
}

void location_config::set_methods(std::vector<std::string> methods)
{
	this->methods = methods;
}

void location_config::set_root(std::string root)
{
	this->root = root;
}

void location_config::set_autoindex(std::string autoindex)
{
	this->autoindex = autoindex;
}

void location_config::set_default_file(std::string default_file)
{
	this->default_file = default_file;
}

void location_config::set_cgi_path(std::string cgi_path)
{
	this->cgi_path = cgi_path;
}

void location_config::set_upload_path(std::string upload_path)
{
	this->upload_path = upload_path;
}

void location_config::set_prefix(std::string prefix)
{
	this->prefix = prefix;
}

int server_config::get_fd_socket()
{
	return fd_socket;
}

//server
//constructor
server_config::server_config()
{
	this->port = 0;
	this->host = "" ;
	this->error_page = "";
	this->max_body_size = 0;
	this->servers = std::vector<std::string>();
	this->locations = std::vector<location_config>();
	this->fd_socket = 0;
}
//copy constructor
server_config::server_config(const server_config &src)
{
	this->port = src.port;
	this->host = src.host;
	this->error_page = src.error_page;
	this->max_body_size = src.max_body_size;
	this->servers = src.servers;
	this->locations = src.locations;
	this->fd_socket = src.fd_socket;

}
//assignment operator
server_config &server_config::operator=(const server_config &src)
{
	this->port = src.port;
	this->host = src.host;
	this->error_page = src.error_page;
	this->max_body_size = src.max_body_size;
	this->servers = src.servers;
	this->locations = src.locations;
	this->fd_socket = src.fd_socket;
	return (*this);
}
//destructor
server_config::~server_config()
{
}
//getters
int server_config::get_port()
{
	return (this->port);
}

std::string server_config::get_host()
{
	return (this->host);
}

std::string server_config::get_error_page()
{
	return (this->error_page);
}

int server_config::get_max_body_size()
{
	return (this->max_body_size);
}

std::vector<std::string> server_config::get_servers()
{
	return (this->servers);
}

std::vector<location_config> server_config::get_locations()
{
	return (this->locations);
}

//setters
void server_config::set_port(int port)
{
	this->port = port;
}

void server_config::set_host(std::string host)
{
	this->host = host;
}

void server_config::set_error_page(std::string error_page)
{
	this->error_page = error_page;
}

void server_config::set_max_body_size(int max_body_size)
{
	this->max_body_size = max_body_size;
}

void server_config::set_servers(std::vector<std::string> servers)
{
	this->servers = servers;
}

void server_config::set_locations(std::vector<location_config> locations)
{
	this->locations = locations;
}

void server_config::add_location(location_config location)
{
	this->locations.push_back(location);
}

int server_config::longest_match(std::string str, std::string needle)
{
	size_t i = 0;
	int last_match = 0;
	int flag = 0;
	while (i < needle.size() && i < str.size())
	{
		if (str[i] == '/')
		{
			if (flag == 1)
				last_match = i;
			else if (flag == 0)
				last_match = i + 1;
			flag = 1;
		}
		if (str[i] != needle[i])
		{
			if ((i - 1) >= 0 && str[i - 1] == '/')
				return (i);
			else
				return last_match;
		}
		i++;
	}
	if ((i < str.size() && str[i] == '/') || (i < needle.size() && needle[i] == '/'))
		return (i + 1);
	else
		return last_match;
}

location_config server_config::longest_prefix_match(std::string prefix)
{
	int best_match = -1;
	int match = 0;
	int index = 0;
	for (size_t i = 0; i < this->locations.size(); i++)
	{
		location_config location = this->locations[i];
		if (strcmp(location.get_prefix().c_str(), prefix.c_str()) == 0)
			return location;
		match = longest_match(location.get_prefix(), prefix);
		if (match > best_match)
		{
			best_match = match;
			index = i;
		}
	}
	return (this->locations[index]);
}



server_config get_server_by_host(std::vector<server_config> servers, std::string host)
{
	std::string host_name(split(host, ':')[0]);
	std::cout << "host: " << host << std::endl;
	std::cout << "host_name: " << host_name << std::endl;
	std::cout << "size: " << servers.size() << std::endl;
	if (is_ip(host_name))
	{
			std::cout << "is ip" << std::endl;
		for (size_t i = 0; i < servers.size(); i++)
		{
			server_config server = servers[i];
			if (strcmp(server.get_host().c_str(), host_name.c_str()) == 0)
				return server;
		}
	}
	else
	{
		for (size_t i = 0; i < servers.size(); i++)
		{
			if (find_string(servers[i].get_servers(), host))
				return servers[i];
		}
	}
	return (server_config());
}

//print server
void server_config::print_server()
{
	std::cout << "port: " << this->port << std::endl;
	std::cout << "host: " << this->host << std::endl;
	std::cout << "error_page: " << this->error_page << std::endl;
	std::cout << "max_body_size: " << this->max_body_size << std::endl;
	std::cout << "servers: " << std::endl;
	for (size_t i = 0; i < this->servers.size(); i++)
		std::cout << this->servers[i] << std::endl;
}
