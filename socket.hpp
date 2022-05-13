#pragma once
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>

namespace WS{
    class Socket
    {
        private:
        struct sockaddr_in address;
        int sock;
        int connection;
        public:
        Socket(int domain, int type_service, int protocol, int port, u_long interface);
        virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;
        void test_connection(int );
        struct sockaddr_in getAddress();
        int getSock();
        int getConnection();
        void setConnection(int con);
    };
 
}