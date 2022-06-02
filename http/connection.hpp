#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#define SIZE_OF_BUFFER 10000

#include "../webserv.hpp"

class connection
{
    private:
        char buffer[SIZE_OF_BUFFER];

    public:
        connection(){};
        ~connection(){};
        void network_core(parsefile s);
};

#endif