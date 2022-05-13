 
#pragma once
 #include <stdio.h>
 #include "socket.hpp"

 namespace WS{
     class Binding : public Socket
     {
         public:
         Binding(int domain, int type_service,int protocol, int port, u_long interface);
         int connect_to_network(int sock, struct sockaddr_in address); 
        
     };
    
     
 }