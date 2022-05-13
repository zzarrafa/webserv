

#include "listening.hpp"

WS::Listening::Listening(int domain, int type_service,int protocol, int port, u_long interface, int bklg) : Binding(domain,type_service, protocol,port, interface)
{
    backlog = bklg;
    start_listening();
    test_connection(listening);
}

void WS::Listening::start_listening()
{
    listening = listen(getSock(), backlog);
}