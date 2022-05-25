#include "connection.hpp"


void connection::chihaja(parsefile s)
{
    std::map <int, std::string> chunking_map;
    for (unsigned long i =0; i < s.get_servers().size(); i++)
        s.get_servers()[i].create_server();
    std::map<int, server_config> fdServer_map;
    std::map<int, server_config> fdClient_map;
    std::vector<int> listOffd;
    fd_set readfds;
    FD_ZERO(&readfds);
    int max = 0;
    for(unsigned long i = 0; i < s.get_servers().size(); i++ )
    {
        std::cout << "==>" << s.get_servers()[i].get_fd_socket() << std::endl;
        FD_SET(s.get_servers()[i].get_fd_socket(),&readfds);
        listOffd.push_back(s.get_servers()[i].get_fd_socket());
        fdServer_map.insert(std::make_pair(s.get_servers()[i].get_fd_socket(),s.get_servers()[i] ));
        if (s.get_servers()[i].get_fd_socket()> max)
            max = s.get_servers()[i].get_fd_socket() ;
    }
    int             activity;
    int             new_socket;
    server_config   serv;
    fd_set          copy_read = readfds;
    fd_set          copy_write = readfds;
    fd_set          writefds;
    FD_ZERO(&writefds);
    while (true)
    {
        readfds = copy_read;
        writefds = copy_write;
        activity = select(max + 1, &readfds , &writefds , NULL , NULL);  
        if (activity < 0)
        {
            perror("Select : ");
            exit(0);
        }
        std::cout << "==" <<max << std::endl;
        for (int fd = 0; fd <= max;fd++)
        {
            // address = s.get_servers()[i].get_fd_socket();
            // addrlen = sizeof(address);
            if (FD_ISSET(fd, &readfds) || FD_ISSET(fd, &writefds) )
            {
                if (std::count(listOffd.begin(), listOffd.end(), fd))
                {
                    struct sockaddr_in  add;
                    int c;
                    new_socket = accept(fd,(struct sockaddr *)&add, (socklen_t *)&c);
                    fcntl(new_socket, F_SETFL, O_NONBLOCK);
                    FD_SET(new_socket ,&copy_read);
                    std::cout << new_socket << std::endl;
                    serv = fdServer_map.find(fd)->second;
                    fdClient_map.insert(std::make_pair(new_socket, serv));
                    if (new_socket > max)
                        max = new_socket;
                    continue;
                }
                else
                {       
                    std::cout << "Clinet" << std::endl;
                    if (FD_ISSET(fd, &readfds ))
                    {
                        server_config server = fdClient_map[fd];
                        int ret = read(fd, buffer, server.get_max_body_size());
                        if (ret == server.get_max_body_size())
                        {
                            if (chunking_map.find(fd) != chunking_map.end())
                            {
                                std::string tmp = chunking_map[fd];
                                tmp += buffer;
                                chunking_map[fd] = tmp;
                            }
                            else
                                chunking_map.insert(std::make_pair(fd, buffer));
                        }
                        else if (ret == 0 || ret < server.get_max_body_size())
                        {
                            
                            chunking_map.erase(fd);
                        }
                        else
                            throw std::runtime_error("Error in reading");
                        std::cout << buffer << std::endl;
                        FD_SET(fd, &copy_write);
                        FD_CLR(fd, &copy_read);
                    }
                    else
                    {
                        char *hello = (char *)("HTTP/1.1 200 OK\nContent-length: 17\n\r\nTello from server");
                        write(fd, hello, strlen(hello));
                        FD_CLR(fd, &copy_write);
                        close(fd);
                    }
                }
            }
        } 
    }
}