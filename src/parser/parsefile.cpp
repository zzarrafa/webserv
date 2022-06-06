#include "../webserv.hpp"

parsefile::parsefile()
{
}

parsefile::~parsefile()
{
}

parsefile &parsefile::operator=(const parsefile &src)
{
	this->_servers = src._servers;
	this->_ports = src._ports;
	return (*this);
}

parsefile::parsefile(const parsefile &src)
{
	this->_servers = src._servers;
	this->_ports = src._ports;
}

std::vector<server_config> &parsefile::get_servers()
{
	return(_servers);
}

std::map<int, int> &parsefile::get_ports()
{
	return (_ports);
}

void parsefile::set_ports(std::map<int, int> ports)
{
	_ports = ports;
}

bool parsefile::is_empty(std::string file_name)
{
	std::string line;
	std::ifstream file(file_name);
	if (file.peek() == std::ifstream::traits_type::eof())
	{
		file.close();
		return (true);
	}
	while (getline(file, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		if (line.find("server{") != std::string::npos)
		{
			file.close();
			return (false);
		}
	}
	return (true);
}

parsefile::parsefile(std::string file_name)
{
    std::ifstream file;
    file.open(file_name, std::ios::in);
    if (is_empty(file_name))
        throw std::runtime_error("Error : file is empty, or has just new lines or comment");
    if (file.is_open())
        this->parse(file);
    else
        throw std::runtime_error("Error: Can't open file or doesn't exist: ");
    file.close();
}

void parsefile::parse(std::ifstream &file)
{
    std::string line;
    while (std::getline(file, line))
    {
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
        if (line.find("server{") != std::string::npos)
        {
            this->fill_servers(file);
        }
    }
}

void parsefile::fill_servers(std::ifstream &file)
{
	std::string line;
	server_config server;
	size_t i = 0;
	while (std::getline(file, line))
	{
		std::string tmp = line;
		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		if (line.empty() || line.find("#") != std::string::npos)
			continue;
		if (line.find("}") != std::string::npos)
		{
			server.check_config();
			for(size_t i = 0; i < _servers.size(); i++)
			{
				server_config &tmp = _servers[i];
				if (search_for_match(tmp.get_servers(), server.get_servers()) == true && tmp.get_port() == server.get_port())
					throw std::runtime_error("Error: host already exist with same port");
			}
			_servers.push_back(server);
			_ports.insert(std::make_pair(server.get_port(), 0));
			return ;
		}
		if ((i = line.find("=")) == std::string::npos)
			throw std::runtime_error("Invalid syntax in config file");
		std::string key = line.substr(0, i);
		std::string value = line.substr(i + 1);
		if (key.empty() || value.empty())
			throw std::runtime_error("Invalid syntax in config file");
		if (key == "port")
		{
			if (is_number(value) && value.size() < 5)
			{
				if (server.get_port() == 0)
					server.set_port(std::stoi(value));
				else
					throw std::runtime_error("Duplicate port in config file");
			}
			else
				throw std::runtime_error("port must be a number");
		}
		else if (key == "host")
		{
			if (is_ip(value))
			{
				if (server.get_host() == "")
					server.set_host(value);
				else
					throw std::runtime_error("Duplicate host in config file");
			}
			else
				throw std::runtime_error("Host must be an ip, i.e: xxx.xxx.xxx.xxx");
		}
		else if (key == "cgi")
		{
			std::string new_value = tmp.substr(tmp.find("=") + 1);
			std::string chars = " ";
			std::vector<std::string>vec = split(Trim(new_value, chars), ' ');
			server.add_cgi_path(vec[0], vec[1]);
		}
		else if (key == "server_name")
		{
			if (server.get_servers().size() != 0)
				throw std::runtime_error("Duplicate server_name in config file");
			std::string new_value = tmp.substr(tmp.find("=") + 1);
			std::string chars = " ";
			std::vector<std::string>names = split(Trim(new_value, chars), ' ');
			server.set_servers(names);
		}
		else if (key == "error_page")
		{
			if (server.get_error_page() == "")
			{
				if (is_one_string(value))
					server.set_error_page(value);
				else
					throw std::runtime_error("error_page must be a valid path");
			}
			else
				throw std::runtime_error("duplicate error_page in config file");
		}
		else if (key == "body_size_limit")
		{
			if (server.get_max_body_size() == 0)
			{
				if (is_number(value))
					server.set_max_body_size(std::stoi(value) * 1048576);
				else
 					throw std::runtime_error("Max body size must be a number");
			}
			else
				throw std::runtime_error("duplicate body limit size in config file");
		}
		else if (key == "location")
		{
			location_config location = fill_locations(file);
			std::string chars("[");
			location.set_prefix(Trim(value, chars));
			server.add_location(location);
		}
		else
			throw std::runtime_error("Invalid syntax in config file");
	}
}

location_config parsefile::fill_locations(std::ifstream &file)
{
	std::string line;
	location_config location;
	size_t i = 0;
	while (std::getline(file, line))
	{
		std::string tmp = line;
		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		if (line.empty() || line.find("#") != std::string::npos)
			continue;
		if (line.find("]") != std::string::npos)
			return (location);
		if ((i = line.find("=")) == std::string::npos)
			throw std::runtime_error("Invalid syntax in config file");
		std::string key = line.substr(0, i);
		std::string value = line.substr(i + 1);
		if (key.empty() || value.empty())
			throw std::runtime_error("Invalid syntax in config file");
		if (key == "method")
		{
			std::string new_value = tmp.substr(tmp.find("=") + 1);
			std::string chars = " ";
			std::vector<std::string>methods = split(Trim(new_value, chars), ' ');
			if (methods.size() == 0 || methods.size() > 3)
				throw std::runtime_error("Invalid syntax in config file");
			else
			{
				if (location.get_methods().size() == 0)
				{
					for (size_t i = 0; i < methods.size(); i++)
					{
						if (!is_method(methods[i]))
							throw std::runtime_error("Invalid method");
						location.set_methods(methods);
					}
				}
				else
					throw std::runtime_error("Duplicate method in same location");
			}
		}
		else if (key == "redirect")
		{
			std::string new_value = tmp.substr(tmp.find("=") + 1);
			std::string chars = " ";
			std::vector<std::string>redirect = split(Trim(new_value, chars), ' ');
			if (location.get_redirect().size() == 0)
			{
				if (redirect.size() == 2)
				{
					if (is_number(redirect[0]))
						location.set_redirect(redirect);
				}
				else
					throw std::runtime_error("Invalid syntax in config file");
			}
			else
				throw std::runtime_error("duplicate redirect in config file");
		}
		else if (key == "root")
		{
			if (location.get_root() == "")
			{
				if (is_one_string(value))
					location.set_root(value);
				else
					throw std::runtime_error("root must be a valid path");
			}
			else
				throw std::runtime_error("duplicate root in config file");
		}
		else if (key == "autoindex")
		{
			if (location.get_autoindex() == "")
			{
				if (value == "on" || value == "off")
					location.set_autoindex(value);
				else
					throw std::runtime_error("index must be on or off");
			}
			else
				throw std::runtime_error("duplicate index in config file");
		}
		else if (key == "default")
		{
			if (location.get_default_file() == "")
			{
				if (is_one_string(value))
					location.set_default_file(value);
				else
					throw std::runtime_error("default must be a valid path");
			}
			else
				throw std::runtime_error("duplicate default in config file");
		}
		else if (key == "upload")
		{
			if (location.get_upload_path() == "")
			{
				if (is_one_string(value))
				{
					std::string str(location.get_root() + value);
					if (!is_path_exist(str))
						mkdir(str.c_str(), 0777);
					location.set_upload_path(str);
				}
				else
					throw std::runtime_error("upload path must be a valid path");
			}
			else
				throw std::runtime_error("duplicate upload path in config file");
		}
		else
			throw std::runtime_error("Invalid syntax in config file");
	}
	return (location);
}

void parsefile::print_servers()
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "Server " << i + 1 << ":" << std::endl;
		std::cout << " > Host: " << _servers[i].get_host() << std::endl;
		std::cout << " > Port: " << _servers[i].get_port() << std::endl;
		std::cout << " > Body limit: " << _servers[i].get_max_body_size() << std::endl;
		std::cout << " > Servers names: " << std::endl;
		for (size_t k = 0; k < _servers[i].get_servers().size(); k++)
			std::cout << "	- " << _servers[i].get_servers()[k] << std::endl;
		std::cout << " > CGI path: " << std::endl;
		std::map<std::string, std::string> map_tmp = _servers[i].get_cgi_path();
		for (std::map<std::string, std::string>::iterator it = map_tmp.begin(); it != map_tmp.end(); it++)
				std::cout << it->first << " " << it->second << std::endl;
		// std::cout << " > Locations:" << std::endl;
		// for (size_t j = 0; j < _servers[i].get_locations().size(); j++)
		// {
		// 	std::cout << "  > Location " << j + 1 << ":" << std::endl;
		// 	std::cout << "	- Prefix: " << _servers[i].get_locations()[j].get_prefix() << std::endl;
		// 	std::cout << "	- Methods: ";
		// 	for (size_t x = 0; x < _servers[i].get_locations()[j].get_methods().size(); x++)
		// 		std::cout << _servers[i].get_locations()[j].get_methods()[x] << " ";
		// 	std::cout << std::endl;
		// 	std::cout << "	- Root: " << _servers[i].get_locations()[j].get_root() << std::endl;
		// 	std::cout << "	- Autoindex: " << _servers[i].get_locations()[j].get_autoindex() << std::endl;
		// 	std::cout << "	- Default file: " << _servers[i].get_locations()[j].get_default_file() << std::endl;
		// 	// std::cout << "	- Cgi path: " << _servers[i].get_locations()[j].get_cgi_path() << std::endl;
		// 	std::cout << "	- Upload path: " << _servers[i].get_locations()[j].get_upload_path() << std::endl;
		// }
	}
}