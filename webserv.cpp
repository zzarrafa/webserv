#include "webserv.hpp"

int main(int argc, char **argv)
{
    try
    {
        if (argc == 2)
        {
            connection con;

            parsefile config(argv[1]);
            for (auto std::map<int, int>::iterator it = config.get_ports().begin(); it != config.get_ports().end(); it++)
				std::cout << it->first << " " << it->second << std::endl;
            // config.print_servers();
            // config.print_servers();
            // location_config location = config.get_servers()[0].longest_prefix_match("/Users/tools/");
            // std::cout << location.get_prefix() << std::endl;
            // config.get_servers();


            // socket that takes the config file instance
            // code here
            // std::cout << config.get_servers().size() << std::endl;
            con.network_core(config);
        }
        else
        {
            std::cout << "Usage: ./server <config_file>" << std::endl;
            return 1;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}