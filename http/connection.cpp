#include "../webserv.hpp"
#include  <set>

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
	std::map<int, server_config> fdServer_map;
	std::map<int, server_config> fdClient_map;
	std::vector<int> listOffd;
	int activity;
	int new_socket;
	server_config serv;
	fd_set writefds;
	fd_set readfds;
	int max = 0;

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	
	for (unsigned long i = 0; i < s.get_servers().size(); i++)
		s.get_servers()[i].create_server();
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
	fd_set copy_write = writefds;
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
					new_socket = accept(fd, NULL, NULL);
					fcntl(new_socket, F_SETFL, O_NONBLOCK);
					FD_SET(new_socket, &copy_read);
					serv = fdServer_map.find(fd)->second;
					fdClient_map.insert(std::make_pair(new_socket, serv));
					if (new_socket > max)
						max = new_socket;
					continue;
				}
				else
				{
					server_config server = fdClient_map[fd];
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
									std::cout<<"complete\n";
									FD_SET(fd, &copy_write);
									FD_CLR(fd, &copy_read);
									serving_map.insert(std::make_pair(fd, req));
								}
								else
									chunking_map.insert(std::make_pair(fd, req));
							}
							else if (chunking_map.find(fd) != chunking_map.end())
							{
								std::cout<<"complete chunked\n";
								chunking_map[fd].fill_body(buffer, 2, ret);
								if (chunking_map[fd].get_is_complete())
								{
									
									chunking_map[fd].print_request();
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
						Response *rep = new Response(server, serving_map[fd]);
						write(fd, rep->get_header().c_str(), rep->get_header().size());
						FD_CLR(fd, &copy_write);
						serving_map.erase(fd);
						delete rep;
						close(fd);
					}
				}
			}
		}
	}
}
