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

void	set_server(std::fstream config_file)
{
	std::string line;
	server_config server;
	int i = 0;
	while (std::getline((config_file, line)))
	{
		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		if (line[0] == '#')
		{
			continue;
		}
		else if (line.find("}") != std::string::npos)
		{
			break;
		}
		else if (line.find("port=") != std::string::npos)
		{
			server.port = std::stoi(line.substr(5));
		}
		else if (line.find("ip=") != std::string::npos)
		{
			server.ip = line.substr(3);
		}
		else if (line.find("name=") != std::string::npos)
		{
			server.name = line.substr(5);
		}
		else if (line.find("max_players=") != std::string::npos)
		{
			server.max_players = std::stoi(line.substr(12));
		}
		else if (line.find("password=") != std::string::npos)
		{
			server.password = line.substr(9);
		}
		else if (line.find("map=") != std::string::npos)
		{
			server.map = line.substr(4);
		}
		else if (line.find("game_mode=") != std::string::npos)
		{
			server.game_mode = line.substr(10);
		}
		else if (line.find("max_players=") != std::string::npos)
		{
			server.max_players = std::stoi(line.substr(12));
		}
		else if (line.find("max_bombs=") != std::string::npos)
		{
			server.max_bombs = std::stoi(line.substr(10));
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