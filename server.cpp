#include "server.hpp"


WS::Server::Server(int domain, int service, int protocol, int port, u_long interface, int bklg)
{
    socket = new Listening(domain, service, protocol, port, interface, bklg);
    // delete socket;
    // socket = new 
}

WS::Listening * WS::Server::get_Sock()
{
    return socket;
}