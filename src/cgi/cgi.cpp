
#include "cgi.hpp"
#include "../webserv.hpp"
extern char **environ;//hbet l execve bash t3rf lash dert had lglobal variable dzab
//https://forhjy.medium.com/42-webserv-cgi-programming-66d63c3b22db

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
        tab = split(line, ': ');
        if (tab[0] == "Set-Cookie")
        {
          headers[tab[0]] = Trim(tab[1], chars);
          setenv("HTTP_COOKIE", headers[tab[0]].c_str(), 1);
        }
        else if (tab[0] == "Status")
        {
          setenv("REDIRECT_STATUS", tab[1].c_str(), 0);
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
  ret.first = outputcontent;
  ret.second = headers;
  return ret;
}


std::pair <std::string, std::map<std::string, std::string> > executi_cgi(request req, char **av, std::string output)
{
  int fd[2];
  int nfd[2];
  int status;
  char c;
  std::string cgioutputt = "";
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
    close(nfd[1]);
    close(fd[0]);
    FILE *result;
    result = fdopen(nfd[0], "r"); 
    while((c = fgetc(result)) != EOF)
    {
      cgioutputt += c;
    }
    fclose(result);
    close(nfd[0]);

  }
    else if (pid == 0)
    {
      write(fd[1], req.get_body().data(), req.get_body().size());
      dup2(nfd[1],1);
      dup2(fd[0],1);
      close(fd[1]);
      close(fd[0]);
      close(nfd[1]);
      close(nfd[0]);
      execve(av[0], av, environ);// "Note, however, that the use of a third argument to the main
       //function is not specified in POSIX.1; according to POSIX.1, the
       //environment should be accessed via the external variable environ(7)" alkitab almo9adaass: man(2) execve;
    }
    return outtputt(cgioutputt);
}

char **arguments(std::string cgi, std::string path)
{
  char **zbi = (char**)malloc(sizeof(char *) * 3);
  zbi[0] = strdup(cgi.c_str());
  zbi[1] = strdup(path.c_str());
  zbi[2] = NULL;
  return zbi;
} 
std::pair<std::string, std::map<std::string, std::string> > Cgi::execution(std::string path, request &req, std::string cgipathto)
{  

    char **args = arguments(cgipathto, path);
    std::cout << "  " << req.get_header("Content-type") << std::endl;
    if(req.get_method() == "POST")
    {
      if (!req.get_header("Content-Type").empty())
        setenv("CONTENT_TYPE", req.get_header("Content-Type").c_str(),1);
    }
    if(!req.get_body().empty())
      setenv("CONTENT_LENGTH", std::to_string(req.get_body().size()).c_str(),1);
    if (!req.get_method().empty())
    setenv("REQUEST_METHOD",req.get_method().c_str(),1 );
    setenv("REDIRECT_STATUS", "true" , 1);
    setenv("SCRIPT_FILENAME", path.c_str(), 1);
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("HTTP_ACCEPT_ENCODING","",0);
    setenv("HTTP_ACCEPT", "" ,0);
    setenv("HTTP_ACCEPT_LANGUAGE","en-US,en;q=0.9",0);
    //bonus
    if (!req.get_header("cookie").empty())
      setenv("HTTP_COOKIE", req.get_header("cookie").c_str(),1);
    setenv("SERVER_PROTOCOL","HTTP/1.1",0);
    setenv("REQUEST_SCHEME","http",0);

    return executi_cgi(req, args, path);
}
