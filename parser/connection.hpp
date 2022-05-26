#include <iostream>
#include "parsefile.hpp"
#include "server.hpp"
#include "../request.hpp"
#include "../response.hpp"
#include <map>
#include <errno.h> 
#include <unistd.h>   
#include <arpa/inet.h>    
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h>

#define SIZE_OF_BUFFER 1024


class connection
{
public:
    connection(/* args */){};
    ~connection(){};
    void chihaja(parsefile s);
private:
char buffer[30000];


};
