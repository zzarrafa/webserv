#include "../webserv.hpp"
#include "../http/response.hpp"

std::string& leftTrim(std::string& str, std::string& chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rightTrim(std::string& str, std::string& chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& Trim(std::string& str, std::string& chars)
{
    return (rightTrim(leftTrim(str, chars), chars));
}

bool is_method(std::string str)
{
	if (str == "GET" || str == "POST" || "DELETE" )
		return (true);
	return (false);
}

void print_binary(char *s, int len)
{
	printf("=>[");
	for (int i = 0; i < len; i++)
	{
		printf("%c", s[i]);
	}
	printf("]\n");
}

std::vector<server_config> get_servers_with_same_port(parsefile &pf, int port)
{
	std::vector<server_config> vec;
	for (size_t i = 0 ; i < pf.get_servers().size() ; i++)
	{
		// std::cerr << i << std::endl;
		if (pf.get_servers()[i].get_port() == port)
		{
			// std::cerr << "port : " << port << std::endl;
			// std::cerr << "server : " << pf.get_servers()[i].get_port() << std::endl;
			vec.push_back(pf.get_servers()[i]);
		}
	}
	// for (size_t i = 0; i < vec.size(); i++)
	// {
	// 	std::cout << "server" << i << ">" << std::endl;
	// 	// vec[i].print_server();
	// }
	// std::cout << "vec size: " << vec.size() << std::endl;
	return (vec);
}

int create_server(int port)
{
	int opt = 1;
	struct sockaddr_in address;
	int fd_socket;
	if( (fd_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	fcntl(fd_socket, F_SETFL, O_NONBLOCK);
	if( setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if (bind(fd_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(fd_socket, 1000) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	// std::cout << "Created " << fd_socket <<  " For Port " << port << std::endl;
	return (fd_socket);
}

bool is_ip(std::string str)
{
	int i = 0;
	int dot_count = 0;
	int num_count = 0;
	while (str[i])
	{
		if (str[i] == '.')
			dot_count++;
		else if (str[i] >= '0' && str[i] <= '9')
			num_count++;
		else
			return (false);
		i++;
	}
	if (dot_count == 3 && num_count > 0)
		return (true);
	return (false);
}

std::vector<std::string> split(std::string str, char delim)
{
	std::vector<std::string> ret;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim))
		ret.push_back(token);
	return (ret);
}

bool is_number(std::string str)
{
	int i = 0;
	int num_count = 0;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			num_count++;
		else
			return (false);
		i++;
	}
	if (num_count > 0)
		return (true);
	return (false);
}

bool is_one_string(std::string str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			return (false);
		i++;
	}
	return (true);
}

std::string   split_file_path(std::string type)
{
	const char *ok;
    ok = strrchr(type.c_str(), '.');
	if (!ok)
		return ("");
	std::string res(ok);
	return res;
}

std::string		read_line(char *buffer, size_t size)
{
	std::string line;
	for (size_t i = 0; i < size; i++)
	{
		if (buffer[i] == '\n')
		{
			line.append(buffer, i);
			return (line);
		}
	}
	return (std::string(""));
}

std::string     get_file_type(std::string type)
{
    std::string res;

    res = split_file_path(type);
    if (!res.empty())
	{
		if (res == ".html") return "text/html";
		else if (res == ".htm") return "text/html";
		else if (res == ".shtml") return "text/html";
		else if (res == ".css") return "text/css";
		else if (res == ".xml") return "text/xml";
		else if (res == ".csv") return "text/csv";
		else if (res == ".gif") return "image/gif";
		else if (res == ".ico") return "image/x-icon";
		else if (res == ".jpeg") return "image/jpeg";
		else if (res == ".jpg") return "image/jpeg";
		else if (res == ".js") return "application/javascript";
		else if (res == ".json") return "application/json";
		else if (res == ".png") return "image/png";
		else if (res == ".pdf") return "application/pdf";
		else if (res == ".svg") return "image/svg+xml";
		else if (res == ".txt") return "text/plain";
		else if (res == ".atom") return "application/atom+xml";
		else if (res == ".rss") return "application/rss+xml";
		else if (res == ".webp") return "image/webp";
		else if (res == ".3gpp") return	"video/3gpp";
		else if (res == ".3gp") return "video/3gpp";
		else if (res == ".ts") return "video/mp2t";
		else if (res == ".mp4") return "video/mp4";
		else if (res == ".mpeg") return "video/mpeg";
		else if (res == ".mpg") return "video/mpeg";
		else if (res == ".mov") return "video/quicktime";
		else if (res == ".webm") return "video/webm";
		else if (res == ".flv") return "video/x-flv";
		else if (res == ".m4v") return "video/x-m4v";
		else if (res == ".mng") return "video/x-mng";
		else if (res == ".asx") return "video/x-ms-asf";
		else if (res == ".asf") return "video/x-ms-asf";
		else if (res == ".wmv") return "video/x-ms-wmv";
		else if (res == ".avi") return "video/x-msvideo";
		else if (res == ".mml") return "text/mathml";
		else if (res == ".txt") return "text/plain";
		else if (res == ".jad") return "text/vnd.sun.j2me.app-descriptor";
		else if (res == ".wml") return "text/vnd.wap.wml";
		else if (res == ".htc") return "text/x-component";
		else if (res == ".png") return "image/png";
		else if (res == ".tif") return "image/tiff";
		else if (res == ".tiff") return "image/tiff";
		else if (res == ".wbmp") return "image/vnd.wap.wbmp";
		else if (res == ".ico") return "image/x-icon";
		else if (res == ".jng") return "image/x-jng";
		else if (res == ".bmp") return "image/x-ms-bmp";
		else if (res == ".svg") return "image/svg+xml";
		else if (res == ".svgz") return "image/svg+xml";
		else if (res == ".mid") return "audio/midi";
		else if (res == ".midi") return "audio/midi";
		else if (res == ".kar") return "audio/midi";
		else if (res == ".mp3") return "audio/mpeg";
		else if (res == ".ogg") return "audio/ogg";
		else if (res == ".m4a") return "audio/x-m4a";
		else if (res == ".ra") return "audio/x-realaudio";
		else if (res == ".pl") return "application/x-perl";
		else if (res == ".py") return "application/x-python";
		else if (res == ".php") return "application/x-php";
		else if (res == ".cpp") return "application/x-c++";
		else if (res == ".c") return "application/x-c";
	}
    return "plain/text";
}

std::string     get_file_ext(std::string res)
{
    if (!res.empty())
	{
		if ("text/html" == res) return(".html");
		else if ("text/css" == res) return(".css");
		else if ("text/xml" == res) return(".xml");
		else if ("text/csv" == res) return(".csv");
		else if ("image/gif" == res) return(".gif");
		else if ("image/x-icon" == res) return(".ico");
		else if ("image/jpeg" == res) return(".jpeg");
		else if ("application/javascript" == res) return(".js");
		else if ("application/json" == res) return(".json");
		else if ("image/png" == res) return(".png");
		else if ("application/pdf" == res) return(".pdf");
		else if ("image/svg+xml" == res) return(".svg");
		else if ("text/plain" == res) return(".txt");
		else if ("application/atom+xml" == res) return(".atom");
		else if ("application/rss+xml" == res) return(".rss");
		else if ("image/webp" == res) return(".webp");
		else if ("video/3gpp" == res) return(".3gpp");
		else if ("video/3gpp" == res) return(".3gp");
		else if ("video/mp2t" == res) return(".ts");
		else if ("video/mp4" == res) return(".mp4");
		else if ("video/mpeg" == res) return(".mpeg");
		else if ("video/mpeg" == res) return(".mpg");
		else if ("video/quicktime" == res) return(".mov");
		else if ("video/webm" == res) return(".webm");
		else if ("video/x-flv" == res) return(".flv");
		else if ("video/x-m4v" == res) return(".m4v");
		else if ("video/x-mng" == res) return(".mng");
		else if ("video/x-ms-asf" == res) return(".asx");
		else if ("video/x-ms-asf" == res) return(".asf");
		else if ("application/javascript" == res) return(".js");
		else if ("video/x-ms-wmv" == res) return(".wmv");
		else if ("video/x-msvideo" == res) return(".avi");
		else if ("audio/midi" == res) return(".mid");
		else if ("audio/midi" == res) return(".midi");
		else if ("audio/midi" == res) return(".kar");
		else if ("audio/mpeg" == res) return(".mp3");
		else if ("audio/ogg" == res) return(".ogg");
		else if ("audio/x-m4a" == res) return(".m4a");
		else if ("audio/x-realaudio" == res) return(".ra");
		else if ("application/x-perl" == res) return(".pl");
		else if ("application/x-python" == res) return(".py");
		else if ("application/x-php" == res) return(".php");
		else if ("application/x-c++" == res) return(".cpp");
		else if ("application/x-c" == res) return(".c");
		else if ("application/zip" == res) return(".zip");
	}
    return "";
}

std::string	readable_fs(double size) {
    int i = 0;
	char buf[30];
    const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    while (size > 1024) {
        size /= 1024;
        i++;
    }
    sprintf(buf, "%.*f %s", i, size, units[i]);
    return  std::string(buf);
}

std::string get_time(time_t unix_timestamp)
{
	char time_buf[80];
	struct tm ts;
	ts = *localtime(&unix_timestamp);
	strftime(time_buf, sizeof(time_buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);

	return std::string(time_buf);
}

std::vector<Fileinfos>	listofFiles(std::string path)
{

	struct dirent *info;
	std::vector<Fileinfos>	list;
	DIR *dir;
	std::string s = path;
	dir = opendir(s.c_str());
	while ((info = readdir(dir)))
	{
			Fileinfos obj;
			obj.file_name = info->d_name;
			struct stat result;
			if(stat((path + "/" + obj.file_name).c_str(), &result)==0)
			{
				obj.date =	get_time(result.st_mtime);
				obj.size = 	readable_fs(result.st_size);

			}else{
				perror("stat");
			}
			list.push_back(obj);
	}
	closedir(dir);
	return list;
}

std::string	formatted_time(void)
{
	time_t	current;
	struct tm * timeinfo;
	char	buffer[80];

	time(&current);
	timeinfo = localtime (&current);

	strftime(buffer, 80, "%a,%e %b %Y %X %Z", timeinfo);

	return std::string(buffer);
}

size_t hex_to_int(std::string str)
{
	size_t ret = 0;
    std::istringstream ss(str);
    ss >> std::hex >> ret;
	return (ret);
}

off_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;
    return -1;
}

bool is_hex(char c)
{
	if (c >= '0' && c <= '9')
		return (true);
	if (c >= 'a' && c <= 'f')
		return (true);
	if (c >= 'A' && c <= 'F')
		return (true);
	return (false);
}

bool valid_hex(char c1, char c2)
{
	if (c1 == '0' && c2 == '\r')
		return (true);
	if (is_hex(c1) && is_hex(c1))
		return (true);
	return (false);
}

bool exists_test (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

bool file_exists(std::string filename)
{
	int fd = open(filename.c_str(), O_RDONLY);
	std::cout << "fd: >>>>" << fd << std::endl;
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}

bool is_path_exist(const std::string s)
{
	struct stat buffer;
	return (stat (s.c_str(), &buffer) == 0);
}

// find srting position in vector
int find_string_position(std::vector<std::string> vec, std::string str)
{
	int i = 0;
	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		if (*it == str)
			return (i);
		i++;
	}
	return (-1);
}

bool find_string(std::vector<std::string> vec, std::string str)
{
    for (size_t i = 0; i < vec.size(); i++)
    {
        if (vec[i] == str)
            return true;
    }
    return false;
}

std::string get_file_name(std::string path, std::string prefix)
{
	char *tmp = strdup(path.c_str());
	tmp += prefix.size();
	// std::cout << "9lawi" << prefix << std::endl;
	// std::cout << "size :" << prefix.size() << std::endl;
	// std::cout << "path inside getfile" << path << std::endl;
	// std::cout << "tmp: " << tmp << std::endl;
	return std::string(tmp);
}

std::map<int, int> switch_map(std::map<int, int> map)
{
	std::map<int, int>	new_map;
	std::map<int, int>::iterator it = map.begin();

	while (it != map.end())
	{
		new_map.insert(std::pair<int, int>(it->second, it->first));
		it++;
	}
	return (new_map);
}

int	first_carriage_return(char *buf, int size)
{
	int i = 0;
	while (i < size)
	{
		if (buf[i] == '\r')
			return (i);
		i++;
	}
	return (i);
}

bool is_valid_chunk(char *buf, int size, int debug)
{
	debug = 0;
	int i = 2;
	if (size > 5)
	{
		if (buf[0] == '\r' && buf[1] == '\n' && buf[2] == '0' && buf[3] == '\r' && buf[4] == '\n')
			return true;
		if (buf[0] == '\r' && buf[1] == '\n')
		{
			if (first_carriage_return(buf + i, size - i) < 7)
			{
				while (i < size)
				{
					if (buf[i] == '\r')
					{
						std::cout << "breeak" << std::endl;
						break ;
					}
					if (!isxdigit(buf[i]))
						return false;
					i++;
				}
				return true;
			}
			else
				return false;
		}
	}
	return false;
}

char    *get_buffer(size_t written, size_t len, std::string file, size_t *size)
{
	char	*buf;
	int		fd;
	int		ret;

	if (len - written <= SIZE_OF_BUFFER)
		len -= written;
	else
		len = SIZE_OF_BUFFER;
	buf = (char *)malloc(sizeof(char) * len);
	fd = open(file.c_str(), O_RDONLY);
	lseek(fd, written, SEEK_SET);
	if (fd == -1)
	{
		perror("open");
		return (NULL);
	}
	ret = read(fd, buf, len);
	if (ret == -1)
	{
		perror("read");
		return (NULL);
	}
	close(fd);
	*size = ret;
	return (buf);
}

char    *get_buffer_with_headers(Response *rep, size_t *size)
{
	char *buf;
	char tmp[SIZE_OF_BUFFER];
	buf = (char *)malloc(sizeof(char) * SIZE_OF_BUFFER);
	strncpy(buf, rep->get_header().c_str(), rep->get_header().size());
	size_t i = rep->get_header().size();
	int j = 0;
	int fd = open(rep->get_body().c_str(), O_RDONLY);
	int ret = read(fd, tmp, SIZE_OF_BUFFER - rep->get_header().size());
	*size = ret + rep->get_header().size();
	while (j < ret)
	{
		buf[i] = tmp[j];
		i++;
		j++;
	}
	close(fd);
	return (buf);
}

std::string	remove_repeated_slashes(std::string path)
{
	std::string new_path = "";
	for (size_t i = 0; i < path.size(); i++)
	{
		if (path[i] == '/')
		{
			if (i == 0 || path[i - 1] != '/')
				new_path += '/';
		}
		else
			new_path += path[i];
	}
	return (new_path);
}

std::string get_file_content(std::string path)
{
	std::string content = "";
	std::ifstream file(path.c_str());
	if (file.is_open())
	{
		std::string line;
		while (getline(file, line))
		{
			content += line;
			content += "\n";
		}
		file.close();
	}
	return (content);
}
