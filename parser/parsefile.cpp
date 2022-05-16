#include "parsefile.hpp"

bool is_empty(std::string file_name)
{
	bool ret = true;
	std::string line;
	int i = 0;
	std::ifstream file(file_name);
	if (file.peek() == std::ifstream::traits_type::eof())
	{
		file.close();
		return (true);
	}
	while (getline(file, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		if (line.find("server{") != std::string::npos && line[0] != '#')
			ret = false;
	}
	file.close();
	return (ret);
}

void parse_config_file(std::string file_name)
{
	std::fstream config_file(file_name);
	if (!config_file.is_open())
		throw std::runtime_error("Could not open config file");
	else if (config_file.peek() == std::ifstream::traits_type::eof())
		throw std::runtime_error("Config file is empty");
	else if(config_file.fail() || config_file.bad())
		throw std::runtime_error("Could not read config file");
	else if(is_empty(file_name))
		throw std::runtime_error("Config file is empty");
	if (config_file.good())
	{
		parse_config_file(file_name);
	}
}

bool is_comment(std::string line)
{
	if (line.find("#") != std::string::npos)
		return (true);
	return (false);
}

void	parse_config_file(std::string file_name)
{
	std::string line;
	std::fstream config_file(file_name);
	while (std::getline(config_file, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		if (line.find("server{") != std::string::npos && line[0] != '#')
		{
			set_server(config_file);
		}
	}
}

void	set_location(std::fstream file, server_config &server)
{
	std::string line;
	location_config location;
	while (std::getline(file, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		if (line.find("]") != std::string::npos)
			break ;
		if (line.find("autoindex") != std::string::npos)
		{
			location.set_autoindex(true);
		}
		else if (line.find("default_file") != std::string::npos)
		{
			location.set_default_file(line.substr(line.find("default_file") + 13));
		}
		else if (line.find("cgi_path") != std::string::npos)
		{
			location.set_cgi_path(line.substr(line.find("cgi_path") + 9));
		}
		else if (line.find("upload_path") != std::string::npos)
		{
			location.set_upload_path(line.substr(line.find("upload_path") + 12));
		}
	}
	server.locations.push_back(location);
}

void	set_server(std::fstream config_file)
{
	std::string line;
	server_config server;
	int i = 0;
	while (std::getline(config_file, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		if (line[0] == '#')
		{
			continue;
		}
		else if (line.find("}") != std::string::npos)
		{
			// push to array
			break;
		}
		else if (line.find("port"))
		{
			server.set_port(std::stoi(line.substr(line.find("=") + 1)));
		}
		else if (line.find("host"))
		{
			server.set_host(line.substr(line.find("=") + 1));
		}
		else if (line.find("error_page"))
		{
			server.set_error_page(line.substr(line.find("=") + 1));
		}
		else if (line.find("max_body_size"))
		{
			server.set_max_body_size(std::stoi(line.substr(line.find("=") + 1)));
		}
		else if (line.find("location"))
		{
			set_location(config_file, &server);
		}
	}
}

int     main(int argc, char **argv)
{

	if (argc == 2)
	{
		try
		{
			parse_config_file(argv[1]);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}