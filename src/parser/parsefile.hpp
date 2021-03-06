#ifndef PARSEFILE_HPP
#define PARSEFILE_HPP

#include "../webserv.hpp"
#include "../http/server.hpp"

class parsefile{
	private:
		std::vector<server_config> _servers;
		std::map<int, int> _ports;

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
		std::vector<server_config> &get_servers();
		void create_server();
		std::map<int, int> &get_ports();
		void set_ports(std::map<int, int> ports);
};

#endif