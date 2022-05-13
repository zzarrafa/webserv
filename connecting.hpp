
#pragma once
#include <stdio.h>
 #include "socket.hpp"

 namespace WS{
     class Connecting : public Socket
     {
         public:
         Connecting(int domain, int type_service,int protocol, int port, u_long interface);
         int connect_to_network(int sock, struct sockaddr_in address);
         
        
     };
    
     
 }