#include "../webserv.hpp"

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
			if (FD_ISSET(fd, &readfds) || FD_ISSET(fd, &writefds))
			{
				if (std::count(listOffd.begin(), listOffd.end(), fd))
				{
					new_socket = accept(fd, NULL, NULL);
					fcntl(new_socket, F_SETFL, O_NONBLOCK);
					FD_SET(new_socket, &copy_read);
					fdClient_map.insert(std::make_pair(new_socket, get_servers_with_same_port(s, s.get_ports()[fd])));
					if (new_socket > max)
						max = new_socket;
					continue;
				}
				else
				{
					std::vector<server_config> server = fdClient_map[fd];
					if (FD_ISSET(fd, &readfds))
					{
						int ret = read(fd, buffer, SIZE_OF_BUFFER);
						//print the buffer
						// print_binary(buffer, ret);
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
						server_config tmp = get_server_by_host(server, serving_map[fd].get_host());
						Response *rep;
						// if (tmp.get_port() == 0)
						// {
						// 	rep = new Response();
						// 	rep->set_status_code(404);
						// 	rep->generate_headers();
						// }
						// else
						serving_map[fd].print_request();
							rep = new Response(tmp, serving_map[fd]);
						usleep(6000);
						write(fd, rep->get_header().c_str(), rep->get_header().size());
						FD_CLR(fd, &copy_write);
						serving_map.erase(fd);
						close(fd);
					}
				}
			}
		}
	}
}
