#include "server.hpp"

// location
// constructor
location_config::location_config()
{
	this->path = "";
	this->redirect = "";
	this->methods = std::vector<std::string>();
	this->autoindex = false;
	this->default_file = "";
	this->cgi_path = "";
	this->upload_path = "";
}
//copy constructor
location_config::location_config(const location_config &src)
{
	this->path = src.path;
	this->redirect = src.redirect;
	this->methods = src.methods;
	this->autoindex = src.autoindex;
	this->default_file = src.default_file;
	this->cgi_path = src.cgi_path;
	this->upload_path = src.upload_path;
}
//assignment operator
location_config::location_config &location_config::operator=(const location_config &src)
{
	this->path = src.path;
	this->redirect = src.redirect;
	this->methods = src.methods;
	this->autoindex = src.autoindex;
	this->default_file = src.default_file;
	this->cgi_path = src.cgi_path;
	this->upload_path = src.upload_path;
	return (*this);
}
//destructor
location_config::~location_config()
{
}
//getters
std::string location_config::get_redirect()
{
	return (this->redirect);
}

std::vector<std::string> location_config::get_methods()
{
	return (this->methods);
}

std::string location_config::get_path()
{
	return (this->path);
}

bool location_config::get_autoindex()
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
//setters
void location_config::set_redirect(std::string redirect)
{
	this->redirect = redirect;
}

void location_config::set_methods(std::vector<std::string> methods)
{
	this->methods = methods;
}

void location_config::set_path(std::string path)
{
	this->path = path;
}

void location_config::set_autoindex(bool autoindex)
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

//server
//constructor
server_config::server_config()
{
	this->port = 8080;
	this->host = "" ;
	this->error_page = "";
	this->max_body_size = 0;
	this->servers = std::vector<std::string>();
	this->locations = std::vector<location_config>();
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
}
//assignment operator
server_config::server_config &server_config::operator=(const server_config &src)
{
	this->port = src.port;
	this->host = src.host;
	this->error_page = src.error_page;
	this->max_body_size = src.max_body_size;
	this->servers = src.servers;
	this->locations = src.locations;
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
