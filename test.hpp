#pragma once
#include "server.hpp"

namespace WS{
    class Test : public Server
    {
        private:
        char buffer[30000];
        int new_socket;
        void accepter();
        void handler();
        void responder();
        public:
        Test();
        void launch();
    };
}