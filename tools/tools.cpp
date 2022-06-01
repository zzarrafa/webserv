#include "../webserv.hpp"

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
	std::string res(ok);
	return res;
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
		else if (res == ".ra"	) return "audio/x-realaudio";
		else if (res == ".pl"	) return "application/x-perl";
		else if (res == ".py"	) return "application/x-python";
		else if (res == ".php"	) return "application/x-php";
		else if (res == ".cpp"	) return "application/x-c++";
		else if (res == ".c"	) return "application/x-c";
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
    std::cout << "path " << path << std::endl;
    std::cout << "prefix " << prefix << std::endl;
    char *buffer = strdup(path.c_str());
    buffer += prefix.size();
    return std::string(buffer); 
}