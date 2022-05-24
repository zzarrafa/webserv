#include "webserv.hpp"

int main(int argc, char **argv)
{
    try
    {
        if (argc == 2)
        {
            parsefile config(argv[1]);
            config.print_servers();
            // socket that takes the config file instance
            // code here
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