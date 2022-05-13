

#include "connecting.hpp"

WS::Connecting::Connecting(int domain, int type_service,int protocol, int port, u_long interface) : Socket(domain, type_service,protocol,port, interface)
{
    setConnection (connect_to_network(getSock(), getAddress()));
    test_connection(getConnection());
}
int WS::Connecting::connect_to_network(int sock, struct sockaddr_in address)
{
    return connect(sock, (struct sockaddr *)&address, sizeof(address));

}