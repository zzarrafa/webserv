#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#define SIZE_OF_BUFFER 10000

#include "../webserv.hpp"

class connection
{
public:
    connection(/* args */){};
    ~connection(){};
    void chihaja(parsefile s);
private:
char buffer[30000];


};

#endif