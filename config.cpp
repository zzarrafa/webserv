#include "config.hpp"

void	parse_line(std::string line)
{
	split(line, ' ');

}

void parse_config_file(std::string file_name)
{
	server_config *server = new server_config();
	int i = 0;
	std::string line;
	std::ifstream file(file_name);
	while (std::getline(file, line))
	{
		while (line[i] == ' ')
			i++;
		if (line[i] == '#')
			continue;
		else
			parse_line(line);
	}
}

int     main(int argc, char **argv)
{
	if (argc == 2)
	{
		parse_config_file(argv[1]);
	}
}