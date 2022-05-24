#ifndef PARSEFILE_HPP
#define PARSEFILE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "server.hpp"
#include "../tools/tools.hpp"

class parsefile{
	private:
		std::vector<server_config> _servers;

	public:
		parsefile();
		~parsefile();
		parsefile(const parsefile &src);
		parsefile &operator=(const parsefile &src);
		parsefile(std::string file_name);
		void parse(std::ifstream &file);
		location_config fill_locations(std::ifstream &file);
		void fill_servers(std::ifstream &file);
		bool is_empty(std::string file_name);
		void print_servers();
};

#endif