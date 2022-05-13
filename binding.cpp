

#include "binding.hpp"


WS::Binding::Binding(int domain, int type_service,int protocol, int port, u_long interface) : Socket(domain, type_service,protocol,port, interface)
{
    setConnection (connect_to_network(getSock(), getAddress()));
    test_connection(getConnection());
}
int WS::Binding::connect_to_network(int sock, struct sockaddr_in address)
{
    return bind(sock, (struct sockaddr *)&address, sizeof(address));

}
