#include "cgi.hpp"
#include "../webserv.hpp"
extern char **environ;
//https://forhjy.medium.com/42-webserv-cgi-programming-66d63c3b22db

std::vector<std::string> split_cgi(const std::string& str, const std::string& delimiter)
{
	std::vector<std::string> strings;
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;

	while ((pos = str.find(delimiter, prev)) != std::string::npos)
	{
		if (!str.empty())
			strings.push_back(str.substr(prev, pos - prev));
		prev = pos + 1;
	}
	strings.push_back(str.substr(prev));
	return strings;
}

std::pair<std::string, std::map<std::string, std::string> > outtputt(std::string output)
{
	std::pair<std::string, std::map<std::string, std::string> > ret;
	std::map<std::string, std::string> headers;
	std::stringstream ss(output);
	std::string line;
	std::string outputcontent;
	std::vector<std::string> tab;
	outputcontent = "";
	line = "";
	std::string chars = " \t\n\r";
	while(ss.good())
	{
		getline(ss, line, '\n');
		line = Trim(line, chars);
		if(line.length() > 0 )
		{
			tab = split_cgi(line, ": ");
			if (tab[0] == "Set-Cookie")
			{
				headers[tab[0]] = Trim(tab[1], chars);
				setenv("HTTP_COOKIE", headers[tab[0]].c_str(), 1);
			}
			else if (tab[0] == "Status")
			{
				setenv("REDIRECT_STATUS", tab[1].c_str(), 0);
			}
			else if (tab[0] == "Location")
			{
				headers[tab[0]] = tab[1].c_str();
			}
		}
		else
			break;
	}
	while(ss.good())
	{
		getline(ss, line, '\n');
		outputcontent += line + "\n";
	}
	std::cout << "outpout: " << outputcontent << std::endl;
	ret.first = outputcontent;
	ret.second = headers;
	return ret;
}

std::pair <std::string, std::map<std::string, std::string> > executi_cgi(request req, char **av, std::string output)
{
	int fd[2];
	int nfd[2];
	av[1] = strdup(output.c_str());
	std::string test = "";

	char c;
	if(pipe(fd) == -1)
	{
		std::cerr << "pipe error" << std::endl;
		exit(1);
	}
	else if (pipe(nfd) == -1)
	{
		std::cerr<<"pipe error" << std::endl;
		exit(1);
	}
	pid_t pid = fork();
	if (pid==-1)
	{
		std::cerr << "fork error" << std::endl;
		exit(1);
	}
	if (pid > 0)
	{
		close(fd[1]);
		close(fd[0]);
		close(nfd[1]);
		FILE *result;
		result = fdopen(nfd[0], "r");
		while((c = fgetc(result)) != EOF)
		test += c;
		fclose(result);
		close(nfd[0]);
	}
	else if (pid == 0)
	{
		int i = write(fd[1], req.get_body().data(), req.get_body().size());
		int newfd = open(req.get_body().data(), O_RDONLY);
		// system("env");
		dup2(nfd[1],1);
		int ret  = dup2(newfd,0);
		// std::cout << "dub " << ret << std::endl;
		// close(fd[1]);
		// close(fd[0]);
		// close(nfd[1]);
		// close(nfd[0]);
		execve(av[0], av, environ);// "Note, however, that the use of a third argument to the main
		//function is not specified in POSIX.1; according to POSIX.1, the
		//environment should be accessed via the external variable environ(7)" alkitab almo9adaass: man(2) execve;
	}
	int i = 0;
	while(av[i]!= NULL)
	{
		free(av[i]);
		i++;
	}
	free(av);
	return outtputt(test);
}

char **arguments(std::string cgi, std::string path)
{
	char **args = (char**)malloc(sizeof(char *) * 3);

	args[0] = strdup(cgi.c_str());
	args[1] = strdup((split(path, '?')[0]).c_str());
	args[2] = NULL;
	return args;
}

std::pair<std::string, std::map<std::string, std::string> > Cgi::execution(std::string path, request &req, std::string cgipathto)
{
	struct stat status;

	lstat(req.get_body().c_str(), &status);
	if(req.get_method() == "POST")
	{
		if (req.get_header("Content-Type").empty())
			setenv("CONTENT_TYPE","application/x-www-form-urlencoded",1);
	}
	setenv("REDIRECT_STATUS", "true" , 1);
	setenv("SCRIPT_FILENAME",  (split(path, '?')[0]).c_str(), 1);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("QUERY_STRING", req.get_querry().c_str(),1);
	setenv("HTTP_ACCEPT_LANGUAGE","en-US,en;q=0.9",0);
	setenv("HTTP_ACCEPT_ENCODING","",0);
	setenv("HTTP_ACCEPT", "" ,0);
	setenv("SERVER_PROTOCOL","HTTP/1.1",1);
	setenv("REQUEST_SCHEME","http",1);
	if(!req.get_body().empty())
		setenv("CONTENT_LENGTH", std::to_string(status.st_size).c_str(),1);
	if (!req.get_method().empty())
		setenv("REQUEST_METHOD",req.get_method().c_str(),1 );
	if (!req.get_header("cookie").empty())
		setenv("cookie", req.get_header("cookie").c_str(),1);

	char **args = arguments(cgipathto, path);
	return executi_cgi(req, args, split(path, '?')[0]);
}
