
#include "test.hpp"
#include <iostream>

WS::Test::Test() : Server(AF_INET, SOCK_STREAM,0,80,INADDR_ANY,10)
{
    launch();
}
void WS::Test::accepter()
{
    // struct sockaddr_in
    int address = get_Sock()->getSock();
    int addrlen = sizeof(address);
    new_socket = accept(get_Sock()->getSock(), (struct sockaddr *)&address, (socklen_t *)&addrlen );

    read(new_socket, buffer, 30000);
}

void WS::Test::handler()
{
    std::cout << buffer << std::endl;
}

void WS::Test::responder()
{
    char *hello = (char *)("Hello from server");
    write(new_socket, hello, strlen(hello));
    close(new_socket);
}

 void WS::Test::launch()
 {
     while (true)
     {
         std::cout << "== waiting ==" << std::endl;
         accepter();
         handler();
         responder();
         std::cout << "== done ==" << std::endl;
     }
     
 }