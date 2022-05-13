#pragma once
#include <stdio.h>
#include <netinet/in.h>
#pragma once
#include "binding.hpp"

namespace WS{
    class Listening : public Binding
    {
        public :
            Listening(int domain, int type_service,int protocol, int port, u_long interface, int bklg);
            void start_listening();
        private:
        int backlog;
        int listening;  
    };
}