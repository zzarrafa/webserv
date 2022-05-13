

#include "socket.hpp"

WS::Socket::Socket(int domain, int type_service, int protocol,  int port, u_long interface)
{
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
   sock = socket(domain, type_service, protocol);
   test_connection(sock);
//    connection == connect_to_network(sock, address);
//    test_connection(connection);
}

void WS::Socket::test_connection(int item_to_test)
{
    if(item_to_test < 0)
    {
        perror("Failed to connect..");
        exit(EXIT_FAILURE);
    }
}
struct sockaddr_in WS::Socket::getAddress()
{
    return address;
}

int WS::Socket::getSock()
{
    return sock;
}

int WS::Socket::getConnection()
{
    return connection;
}
void WS::Socket::setConnection(int con)
{
    connection = con;
}