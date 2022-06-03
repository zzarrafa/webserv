#include "../webserv.hpp"

void print_binary(char *s , int len)
{
	printf("=>[");
	for(int i = 0; i < len; i++)
	{
		printf("%c", s[i]);
	}
	printf("]\n");
}

void connection::network_core(parsefile s)
{
	std::map<int, request> chunking_map;
	std::map<int, request> serving_map;
	std::map<int, std::vector<server_config> > fdServer_map;
	std::map<int, std::vector<server_config> > fdClient_map;
	std::vector<int> listOffd;
	int activity;
	int new_socket;
	server_config serv;
	fd_set writefds;
	fd_set readfds;
	int max = 0;

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
    for (auto std::map<int, int>::iterator it = s.get_ports().begin(); it != s.get_ports().end(); ++it)
	{
        it->second = create_server(it->first);
		FD_SET(it->second, &readfds);
		listOffd.push_back(it->second);
		fdServer_map.insert(std::make_pair(it->second, get_servers_with_same_port(s, it->first)));
		if (it->second > max)
			max = it->second;
	}
	fd_set copy_read = readfds;
	fd_set copy_write = writefds;


	// std::map<int, int> fd_map;
	// for (auto std::map<int, int>::iterator it = fd_map.begin(); it != fd_map.end(); ++it)
	// {
	// 	printf(">>[%d] ", it->first);
	// 	printf("[%d]\n", it->second);
	// }


	s.set_ports(switch_map(s.get_ports()));



	while (true)
	{
		readfds = copy_read;
		writefds = copy_write;
		activity = select(max + 1, &readfds, &writefds, NULL, NULL);
		if (activity < 0)
			throw std::runtime_error("Select error");
		for (int fd = 0; fd <= max; fd++)
		{
			// fd_map = s.get_ports();
			// for (auto std::map<int, int>::iterator it = fd_map.begin(); it != fd_map.end(); ++it)
			// {
				// printf(">>[%d] ", it->first);
				// printf("[%d]\n", it->second);
			// }
			if (FD_ISSET(fd, &readfds) || FD_ISSET(fd, &writefds))
			{
				if (std::count(listOffd.begin(), listOffd.end(), fd))
				{
					new_socket = accept(fd, NULL, NULL);
					fcntl(new_socket, F_SETFL, O_NONBLOCK);
					FD_SET(new_socket, &copy_read);
					// function that returns server
					// serv = get_right_server(fdServer_map[fd], fd);
					// print s.get_ports content
					// std::cout << "fd: " << fd << std::endl;
					// std::cout << "///////>>>>>>" << s.get_ports()[fd] << std::endl;
					// std::cout << "•••>" << new_socket;
					fdClient_map.insert(std::make_pair(new_socket, get_servers_with_same_port(s, s.get_ports()[fd])));
					if (new_socket > max)
						max = new_socket;
					continue;
				}
				else
				{
					std::vector<server_config> server = fdClient_map[fd];
					// std::cout << "map size: " << fdClient_map.size() << std::endl;
					// std::cout << "server size: " << server.size() << std::endl;
					// std::cout << "fd: " << fd << std::endl;
					// for (auto std::map<int, std::vector<server_config> >::iterator it = fdClient_map.begin(); it !=fdClient_map.end(); it++)
					// {
						// std::cout << it->first << " >>>>> ";
						// for (size_t i = 0; i < it->second.size(); i++)
							// it->second[i].print_server();
					// }
					// std::cout << "server size inside map: " << fdClient_map[fd].size() << std::endl;
					// std::cout << "***************************" << std::endl;
					if (FD_ISSET(fd, &readfds))
					{
						int ret = read(fd, buffer, SIZE_OF_BUFFER);
						if (ret == -1)
						{
							if (errno != EAGAIN)
							{
								close(fd);
								FD_CLR(fd, &copy_read);
								fdClient_map.erase(fd);
								listOffd.erase(std::remove(listOffd.begin(), listOffd.end(), fd), listOffd.end());
							}
						}
						else if (ret > 0)
						{
							if (chunking_map.find(fd) == chunking_map.end())
							{
								request	req(buffer, ret);
								if (req.get_is_complete())
								{
									FD_SET(fd, &copy_write);
									FD_CLR(fd, &copy_read);
									serving_map.insert(std::make_pair(fd, req));
								}
								else
									chunking_map.insert(std::make_pair(fd, req));
							}
							else if (chunking_map.find(fd) != chunking_map.end())
							{
								chunking_map[fd].fill_body(buffer, 2, ret);
								if (chunking_map[fd].get_is_complete())
								{
									// chunking_map[fd].print_request();
									FD_SET(fd, &copy_write);
									FD_CLR(fd, &copy_read);
									serving_map.insert(std::make_pair(fd, chunking_map[fd]));
									chunking_map.erase(fd);
								}
							}
						}
						else if (ret == 0)
						{
							close(fd);
							FD_CLR(fd, &copy_read);
							fdClient_map.erase(fd);
							listOffd.erase(std::remove(listOffd.begin(), listOffd.end(), fd), listOffd.end());
						}
						else
							throw std::runtime_error("Read error");
					}
					else
					{
						// serving_map[fd].print_request();
						// print serving_map
						// std::cout << "fd: " << fd << std::endl;
						// std::cout << "size: " << serving_map.size() << std::endl;

						// for (auto std::map<int, request>::iterator it = serving_map.begin(); it != serving_map.end(); it++)
						// {
							// std::cout << it->first << " >>>[][][][]>> ";
							// it->second.print_request();
						// }
						// serving_map[fd].print_request();
						server_config tmp = get_server_by_host(server, serving_map[fd].get_host());
						Response *rep;
						tmp.print_server();
						if (tmp.get_port() == 0)
						{
							// char *tello = (char *)("HTTP/1.1 404 OK\nContent-length: 17\n\r\nTello from server");
							rep = new Response();
							rep->set_status_code(404);
							rep->generate_headers();
						}
						else
							rep = new Response(tmp, serving_map[fd]);
						// std::cout << ">/>/>/>" << server.size() << std::endl;
						// server[0].print_server();
						// get_server_by_host(server, serving_map[fd].get_host()).print_server();
						write(fd, rep->get_header().c_str(), rep->get_header().size());
                        // write(fd, tello, strlen(tello));
						FD_CLR(fd, &copy_write);
						serving_map.erase(fd);
						// delete rep;
						close(fd);
					}
				}
			}
		}
	}
}
