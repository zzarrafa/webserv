#include "../webserv.hpp"

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
    this->_is_complete = src._is_complete;
    this->_chunk = src._chunk;
    this->_offset = src._offset;
    this->_headers_len = src._headers_len;
    this->_headers = src._headers;
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
    this->_is_complete = src._is_complete;
    this->_chunk = src._chunk;
    this->_offset = src._offset;
    this->_headers_len = src._headers_len;
    this->_headers = src._headers;
    return (*this);
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
    std::cout << "> Is complete: " << this->_is_complete << std::endl;
    // std::cout << "• Rest of the Headers: " << std::endl;
    // for (auto std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
        // std::cout << "  - " << it->first << ": " << it->second << std::endl;
    std::cout << "Body file size: " << fsize(this->_body.c_str()) << std::endl;
    std::cout << "• Body file name: " << this->_body << std::endl;
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

void request::set_length(size_t length)
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

size_t request::get_length()
{
    return (this->_length);
}

bool request::get_is_complete()
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

void request::check_if_complete()
{
    if (this->_method == "GET" || this->_method == "DELETE")
    {
        std::cout << "> Request is complete" << std::endl;
        this->_is_complete = true;
    }
    if (this->_length == (size_t)fsize(this->_body.c_str()))
    {
        this->_is_complete = true;
    }
}

request::request(char *buffer, int ret)
{
    //variables
    std::stringstream   ss(buffer);
    std::string         line;
    bool                is_first = true;
    std::string         key;
    std::string         value;
    std::string         chars = " \t\n\r";

    //initialize
    this->_is_complete = false;
    this->_offset = 0;
    this->_headers_len = 0;
    this->_body = "";
    this->_length = 0;

    while (getline(ss, line))
    {
        if (line == "\r")
        {
            this->_offset += 2;
            break;
        }
        else
        {
            this->_offset += line.size() + 1;
            if (is_first)
            {
                std::vector<std::string> tokens;
                tokens = split(line, ' ');
                this->_method = tokens[0];
                this->_path = tokens[1];
                this->_version = tokens[2];
                tokens.clear();
                is_first = false;
            }
            else
            {
                int pos = line.find(":");
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
                else if (key == "Transfer-Encoding")
                    this->_encoding = Trim(value, chars);
                else
                    this->_headers.insert(std::pair<std::string, std::string>(key, value));
            }
        }
    }
    this->check_if_complete();
    if (this->_is_complete)
        return ;
    fill_body(buffer + this->_offset, 1, ret);
}

int     request::find_char(char *buffer, char c)
{
    int i = 0;
    while (buffer[i] != '\0')
    {
        if (buffer[i] == c)
            return (i);
        i++;
    }
    return (-1);
}

int     request::read_hex(char *buffer, int *len)
{
    *len = find_char(buffer, '\r');
    std::string s(buffer, *len);
    return hex_to_int(s);
}

int     calculate_hex(char *buffer)
{
    std::cout << "hiho" << std::endl;
    int i = 0;
    while (buffer[i] != '\r' && buffer[i] != '\0')
        i++;
    int j = 0;
    std::cout << "Calculating hex: [";
    while (j < i)
        std::cout << buffer[j++];
    std::cout << "]\n";
    return (i);
}

char    *request::clean_buffer(char *buffer, int ret, int *counter)
{
    int i = 0;
    *counter = 0;
    while (i < ret)
    {
        if (buffer[i] == '\r' && i + 3 < ret && buffer[i + 1] == '\n' && valid_hex(buffer[i + 2], buffer[i + 3]) && isxdigit(buffer[i + 4]))
        {
            *counter += calculate_hex(buffer + i + 2);
            *counter += 4;
            i += *counter;
        }
        i++;
    }
    char *new_buffer = new char[ret - *counter];
    i = 0;
    int j = 0;
    while (i < ret)
    {
        if (buffer[i] == '\r' && i + 3 < ret && buffer[i + 1] == '\n' && valid_hex(buffer[i + 2], buffer[i + 3]) && isxdigit(buffer[i + 4]))
        {
            std::cout << "Found CR LF" << std::endl;
            i += 2;
            i += calculate_hex(buffer + i);
            i += 2;
        }
        else
            new_buffer[j++] = buffer[i++];
    }
    return (new_buffer);
}


void    request::fill_body(char *buffer, int flag, int ret)
{
    int     fd;
    int     len = 0;

    if (flag == 1)
    {
        std::string body("body");
        srand(time(NULL));
        body += std::to_string(rand());
        this->_body = "tmp/" + body;
        fd = open(this->_body.c_str(), O_RDWR | O_CREAT | O_APPEND, 0666);
        if (this->_encoding == "chunked")
        {
            this->_chunk = read_hex(buffer, &len);
            // std::cout << "to read: " << ret - this->_offset << std::endl;
            buffer += len + 2;
            this->_offset += len + 2;
            write(fd, buffer, ret - this->_offset);
        }
        else
            write(fd, buffer, this->_length);
        close(fd);
    }
    else if (flag == 2)
    {
        // std::cout << "to read: " << ret << std::endl;
        int counter = 0;
        char *new_buffer = clean_buffer(buffer, ret, &counter);
        fd = open(this->_body.c_str(), O_RDWR | O_APPEND, 0666);
        write(fd, new_buffer, ret - counter);
        close(fd);
    }
    if (ret < SIZE_OF_BUFFER && size_t(fsize(this->_body.c_str())) >= this->_length)
        this->_is_complete = true;
}
