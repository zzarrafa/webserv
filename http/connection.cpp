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
	std::map<int, server_config> fdServer_map;
	std::map<int, server_config> fdClient_map;
	std::vector<int> listOffd;
	int activity;
	int new_socket;
	server_config serv;
	fd_set writefds;
	fd_set readfds;
	int max = 0;

	for (unsigned long i = 0; i < s.get_servers().size(); i++)
		s.get_servers()[i].create_server();
	FD_ZERO(&readfds);
	for (unsigned long i = 0; i < s.get_servers().size(); i++)
	{
		std::cout << "==>" << s.get_servers()[i].get_fd_socket() << std::endl;
		FD_SET(s.get_servers()[i].get_fd_socket(), &readfds);
		listOffd.push_back(s.get_servers()[i].get_fd_socket());
		fdServer_map.insert(std::make_pair(s.get_servers()[i].get_fd_socket(), s.get_servers()[i]));
		if (s.get_servers()[i].get_fd_socket() > max)
			max = s.get_servers()[i].get_fd_socket();
	}

	fd_set copy_read = readfds;
	fd_set copy_write = readfds;
	FD_ZERO(&writefds);
	while (true)
	{
		readfds = copy_read;
		writefds = copy_write;
		activity = select(max + 1, &readfds, &writefds, NULL, NULL);
		if (activity < 0)
			throw std::runtime_error("Select error");
		for (int fd = 0; fd <= max; fd++)
		{
			if (FD_ISSET(fd, &readfds) || FD_ISSET(fd, &writefds))
			{
				if (std::count(listOffd.begin(), listOffd.end(), fd))
				{
					struct sockaddr_in add;
					int c;
					new_socket = accept(fd, (struct sockaddr *)&add, (socklen_t *)&c);
					fcntl(new_socket, F_SETFL, O_NONBLOCK);
					FD_SET(new_socket, &copy_read);
					std::cout << new_socket << std::endl;
					serv = fdServer_map.find(fd)->second;
					fdClient_map.insert(std::make_pair(new_socket, serv));
					if (new_socket > max)
						max = new_socket;
					continue;
				}
				else
				{
					if (FD_ISSET(fd, &readfds))
					{
						server_config server = fdClient_map[fd];
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
							std::ofstream file;
							// file.open("req.txt", std::ios_base::app);
							// file << buffer;
							// file.close();
							if (chunking_map.find(fd) == chunking_map.end())
							{
								request	req(buffer, ret);
								if (req.get_is_complete())
								{
									FD_SET(fd, &copy_write);
									FD_CLR(fd, &copy_read);
								}
								else
									chunking_map.insert(std::make_pair(fd, req));
							}
							else if (chunking_map.find(fd) != chunking_map.end())
							{
								chunking_map[fd].fill_body(buffer, 2, ret);
								if (chunking_map[fd].get_is_complete())
								{
									chunking_map[fd].print_request();
									FD_SET(fd, &copy_write);
									FD_CLR(fd, &copy_read);
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
						char *tello = (char *)("HTTP/1.1 200 OK\nContent-length: 17\n\r\nTello from server");
						write(fd, tello, strlen(tello));
						FD_CLR(fd, &copy_write);
						close(fd);
					}
				}
			}
		}
	}
}
