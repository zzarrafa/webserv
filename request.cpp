#include "request.hpp"

request::request()
{
}

request::~request()
{
}

request::request(const request &src)
{
    this->_method = src._method;
    this->_path = src._path;
    this->_version = src._version;
    this->_host = src._host;
    this->_type = src._type;
    this->_body = src._body;
    this->_connection = src._connection;
    this->_encoding = src._encoding;
    this->_length = src._length;
}

request &request::operator=(const request &src)
{
    this->_method = src._method;
    this->_path = src._path;
    this->_version = src._version;
    this->_host = src._host;
    this->_type = src._type;
    this->_body = src._body;
    this->_connection = src._connection;
    this->_encoding = src._encoding;
    this->_length = src._length;
    return (*this);
}

void request::set_method(std::string method)
{
    this->_method = method;
}

void request::set_path(std::string path)
{
    this->_path = path;
}

void request::set_version(std::string version)
{
    this->_version = version;
}

void request::set_host(std::string host)
{
    this->_host = host;
}

void request::set_type(std::string type)
{
    this->_type = type;
}

void request::set_body(std::string body)
{
    this->_body = body;
}

void request::set_connection(std::string connection)
{
    this->_connection = connection;
}

void request::set_encoding(std::string encoding)
{
    this->_encoding = encoding;
}

void request::set_length(int length)
{
    this->_length = length;
}

std::string request::get_method()
{
    return (this->_method);
}

std::string request::get_path()
{
    return (this->_path);
}

std::string request::get_version()
{
    return (this->_version);
}

std::string request::get_host()
{
    return (this->_host);
}

std::string request::get_type()
{
    return (this->_type);
}

std::string request::get_body()
{
    return (this->_body);
}

std::string request::get_encoding()
{
    return (this->_encoding);
}

std::string request::get_connection()
{
    return (this->_connection);
}

int request::get_length()
{
    return (this->_length);
}

bool    request::get_is_complete()
{
    return (this->_is_complete);
}

std::map<std::string, std::string> request::get_headers()
{
    return (this->_headers);
}

void request::add_headers(std::string key, std::string value)
{
    this->_headers.insert(std::pair<std::string, std::string>(key, value));
}

request   request::parse_header(std::string str)
{
    std::string     line;
    std::string     key;
    std::string     value;
    std::string     tmp;
    std::string     chars = " \t\n\r";
    bool is_first = true;
    int             pos;
    std::stringstream ss(str);
    request req;

    while (getline(ss, line, '\r'))
    {
        if (is_first)
        {
            is_first = false;
            std::vector<std::string> tokens;
            tokens = split(line, ' ');
            req._method = tokens[0];
            req._path = tokens[1];
            req._version = tokens[2];
        }
        else
        {
            pos = line.find(":");
            key = line.substr(0, pos);
            value = line.substr(pos + 1, line.size());
            if (key == "Host")
                req._host = Trim(value, chars);
            else if (key == "Content-Type")
                req._type = Trim(value, chars);
            else if (key == "Content-Length")
                req._length = std::stoi(value);
            else if (key == "Connection")
                req._connection = Trim(value, chars);
            else if (key == "Accept-Encoding")
                req._encoding = Trim(value, chars);
            else
                req._headers.insert(std::pair<std::string, std::string>(key, value));
        }
    }
    if (req._method == "GET" || req._method == "DELETE")
        req._is_complete = true;
    return (req);
}

request::request(std::string str)
{
    std::string     line;
    std::string     key;
    std::string     value;
    std::string     tmp;
    std::string     chars = " \t\n\r";
    bool            is_first = true;
    int             pos;
    bool            is_body = false;
    std::stringstream ss(str);


    while (getline(ss, line))
    {
        if (is_first)
        {
            is_first = false;
            std::vector<std::string> tokens;
            tokens = split(line, ' ');
            this->_method = tokens[0];
            this->_path = tokens[1];
            this->_version = tokens[2];
        }
        else if (is_body)
        {
            this->_body += line;
            this->_body += "\n";
        }
        else if (line.empty())
        {
            is_body = true;
            continue;
        }
        else
        {
            pos = line.find(":");
            key = line.substr(0, pos);
            value = line.substr(pos + 1, line.size());
            if (key == "Host")
                this->_host = Trim(value, chars);
            else if (key == "Content-Type")
                this->_type = Trim(value, chars);
            else if (key == "Content-Length")
                this->_length = std::stoi(value);
            else if (key == "Connection")
                this->_connection = Trim(value, chars);
            else if (key == "Accept-Encoding")
                this->_encoding = Trim(value, chars);
            else
                this->_headers.insert(std::pair<std::string, std::string>(key, value));
        }
    }
}

void request::print_request()
{
    std::cout << "> Method: " << this->_method << std::endl;
    std::cout << "> Path: " << this->_path << std::endl;
    std::cout << "> Version: " << this->_version << std::endl;
    std::cout << "> Host: " << this->_host << std::endl;
    std::cout << "> Type: " << this->_type << std::endl;
    std::cout << "> Connection: " << this->_connection << std::endl;
    std::cout << "> Encoding: " << this->_encoding << std::endl;
    std::cout << "> Length: " << this->_length << std::endl;
    std::cout << "• Rest of the Headers: " << std::endl;
    for (auto std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
    {
        std::cout << "  - " << it->first << ": " << it->second << std::endl;
    }
    std::cout << "• Body: " << this->_body << std::endl;
}
