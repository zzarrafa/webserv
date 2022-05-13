

#include "listening.hpp"
#include <unistd.h>
namespace WS
{
    class Server
    {
        private:
        Listening *socket;
        virtual void accepter() = 0;
        virtual void handler() = 0;
        virtual void responder() =0;
        public:
        Server(int domain, int service, int protocol, int port, u_long interface, int bklg);
        virtual void launch() = 0;
        Listening * get_Sock();
    };
    
} // namespace name
