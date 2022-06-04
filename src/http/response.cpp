#include "response.hpp"

Response::Response()
{
    this->status_code = 200;
    this->auto_index = "";
    this->content_lenght = 0;
    this->written = 0;
    this->is_complete = false;
}

Response::~Response()
{

}

//copy constructor
Response::Response(const Response &src)
{
    this->status_code = src.status_code;
    this->status = src.status;
    this->content_lenght = src.content_lenght;
    this->content_type = src.content_type;
    this->body = src.body;
    this->header = src.header;
    this->status_code = src.status_code;

}

void Response::print_response()
{
    std::cout << "> Status code: " << this->status_code << std::endl;
    std::cout << "> Status: " << this->status << std::endl;
    std::cout << "> Content lenght: " << this->content_lenght << std::endl;
    std::cout << "> Content type: " << this->content_type << std::endl;
    std::cout << "> Body: " << this->body << std::endl;
    std::cout << "> Header: " << this->header << std::endl;
}

// assignment operator
Response &Response::operator=(const Response &src)
{
    this->status_code = src.status_code;
    this->status = src.status;
    this->content_lenght = src.content_lenght;
    this->content_type = src.content_type;
    this->body = src.body;
    this->header = src.header;
    this->status_code = src.status_code;
    return (*this);
}

std::string Response::get_body()
{
    return this->body;
}

void Response::set_status_code(int status)
{
    this->status_code = status;
}
void Response::set_content_lenght(size_t cl)
{
    this->content_lenght = cl;
}
void Response::set_content_type(std::string ct)
{
    this->content_type = ct;
}
void Response::set_status(std::string status)
{
    this->status = status;
}
int Response::get_status_code()
{
    return status_code;
}
size_t Response::get_content_lenght()
{
    return content_lenght;
}
std::string Response::get_content_type()
{
    return content_type;
}
std::string Response::get_status()
{
    return status;
}
std::string Response::get_header()
{
    return header;
}
size_t Response::get_written()
{
    return written;
}
void Response::set_written(size_t written)
{
    this->written = written;
}
bool Response::isDir(std::string path)
{
    struct stat statbuf;
	if (path == "/")
		return true;
	std::string s = path;
	if (stat(s.c_str(), &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}
bool Response::get_is_complete()
{
    return is_complete;
}
void Response::set_is_complete(bool is_complete)
{
    this->is_complete = is_complete;
}
void Response::get_file(std::string file_name)
{
    std::ifstream f(file_name);
    if (f)
    {
        std::ostringstream ss;
        ss << f.rdbuf();
        body = ss.str();  // the contenant of the file in the body / check the contenant lenght and the extention
    }
    header = "HTTP/1.1 200 ok\r\n";
	header += "Content-Type: " + get_file_type(file_name) + "\r\n";
	header += "Content-Length: "+ std::to_string(body.size()) + "\r\n";
	header += "Server: mywebserv\r\n";
	header += "Date: " + formatted_time() + "\r\n";
	header += "\r\n";
    header += body;
}
int Response::search_for_default(server_config &s,std::string path)
{
    std::vector<location_config> locations = s.get_locations();
    size_t i ;
    std::string file_name;
    for (i = 0; i < locations.size(); i++)
    {
        if (locations[i].get_prefix() == path)
        {
            file_name = locations[i].get_root() + locations[i].get_default_file();
            if (!exists_test(file_name))
            {
                set_status_code(404);
                return 1;
            }
            get_file(file_name);
            return 1;
        }
    }
    return 0;
}

void Response::autoindex(std::string path, std::string prefix, std::string root)
{
    std::vector<Fileinfos> file = listofFiles(path);
    std::string	body = "<html lang='en'>\n<head>\n<title>Document</title>\n</head>\n<body>\n<h1>Index OF "+ path + " </h1>\n<br>\n<table width=\"100%\">\n<hr>";
    this->auto_index = get_file_name(path, root);
    for(size_t i =0; i < file.size(); i++)
    {
        std::string td = "<tr><td width=\"50%\"> <a href=\"" + prefix + this->auto_index + "/" + file[i].file_name +"\"> "+file[i].file_name + "</a></td>"  ;
		body += td;
		if (file[i].file_name == ".." || file[i].file_name == ".")
			continue;
		td  = "<td width=\"25%\">"+ file[i].date + "</td>"  ;
		body += td;
		if (isDir( path + "/"+ file[i].file_name))
			td = "<td width=\"25%\">"+ std::string("-") +"</td></tr>"  ;
		else
			td = "<td width=\"25%\">"+ (file[i].size)+"</td></tr>" ;
		body += td;
    }
    body +=  "</tbale></body></html>";
    header = "HTTP/1.1 200 Ok\r\n";
	header += "Content-Type: text/html\r\n";
	header += "Content-Length: "+ std::to_string(body.size()) + "\r\n";
	header += "Server: mywebserv\r\n";
	header += "Date: " + formatted_time() + "\r\n";
	header += "\r\n";
	header += body;
}


void Response::generate_headers()
{
    if (status_code == 400)
    {

        body = "<html>\n<head><title>400 Bad Request</title></head>\n<body bgcolor='white'>\n<center><h1>400 Bad Request</h1></center>\n</body>\n</html>";
        header = "HTTP/1.1 400 Bad Request\r\n";
        header += "Content-Type: text/html\r\n";
        header += "Content-Length: " + std::to_string(body.size()) + "\r\n";
        header += "Server: mywebserver\r\n";
        header += "Date: " + formatted_time() + "\r\n";
        header += "\r\n";
        header += body;
    }
    else if (status_code == 404)
    {
        body = "<html>\n<head><title>404 Not Found</title></head>\n<body bgcolor='white'>\n<center><h1>404 Not Found</h1></center>\n</body>\n</html>";
        header = "HTTP/1.1 404 Not Found\r\n";
	    header += "Content-Type: text/html\r\n";
	    header += "Content-Length: "+ std::to_string(body.size()) + "\r\n";
	    header += "Server: mywebserver\r\n";
	    header += "Date: " + formatted_time() + "\r\n";
	    header += "\r\n";
        header += body;
    }
    else if (status_code == 405)
    {
        body = "<html>\n<head><title>405 Method Not Allowed</title></head>\n<body bgcolor='white'>\n<center><h1>405 Method Not Allowed</h1></center>\n</body>\n</html>";
        header = "HTTP/1.1 405 Method Not Allowed\r\n";
	    header += "Content-Type: text/html\r\n";
	    header += "Content-Length: "+ std::to_string(body.size()) + "\r\n";
	    header += "Server: mywebserver\r\n";
	    header += "Date: " + formatted_time() + "\r\n";
	    header += "\r\n";
        header += body;
    }
    else if ( status_code == 403)
    {
        body = "<html>\n<head><title>403 Forbidden</title></head>\n<body bgcolor='white'>\n<center><h1>403 Forbidden</h1></center>\n</body>\n</html>";
        header = "HTTP/1.1 403 Forbidden\r\n";
	    header += "Content-Type: text/html\r\n";
	    header += "Content-Length: "+ std::to_string(body.size()) + "\r\n";
	    header += "Server: mywebserver\r\n";
	    header += "Date: " + formatted_time() + "\r\n";
	    header += "\r\n";
        header += body;
    }
    else if (status_code == 409)
    {
        body = "<html>\n<head><title>409 Conflict</title></head>\n<body bgcolor='white'>\n<center><h1>409 Conflict</h1></center>\n</body>\n</html>";
        header = "HTTP/1.1 409 Conflict\r\n";
	    header += "Content-Type: text/html\r\n";
	    header += "Content-Length: "+ std::to_string(body.size()) + "\r\n";
	    header += "Server: mywebserver\r\n";
	    header += "Date: " + formatted_time() + "\r\n";
	    header += "\r\n";
        header += body;
    }
    else if (status_code == 201)
    {
        body = "<html>\n<head><title>201 Created</title></head>\n<body bgcolor='white'>\n<center><h1>201 Created</h1></center>\n</body>\n</html>";
        header = "HTTP/1.1 201 Created\r\n";
	    header += "Content-Type: text/html\r\n";
	    header += "Content-Length: "+ std::to_string(body.size()) + "\r\n";
	    header += "Server: mywebserver\r\n";
	    header += "Date: " + formatted_time() + "\r\n";
	    header += "\r\n";
        header += body;
    }
    else if (status_code == 200)
    {
        body = "<html>\n<head><title>200 Ok</title></head>\n<body bgcolor='white'>\n<center><h1>200 Ok</h1></center>\n</body>\n</html>";
        header = "HTTP/1.1 200 Ok\r\n";
	    header += "Content-Type: text/html\r\n";
	    header += "Content-Length: "+ std::to_string(body.size()) + "\r\n";
	    header += "Server: mywebserver\r\n";
	    header += "Date: " + formatted_time() + "\r\n";
	    header += "\r\n";
        header += body;
    }
    else if (status_code == 413)
    {
        body = "<html>\n<head><title>413 Request Entity Too Large</title></head>\n<body bgcolor='white'>\n<center><h1>413 Request Entity Too Large</h1></center>\n</body>\n</html>";
        header = "HTTP/1.1 413 Request Entity Too Large\r\n";
	    header += "Content-Type: text/html\r\n";
	    header += "Content-Length: "+ std::to_string(body.size()) + "\r\n";
	    header += "Server: mywebserver\r\n";
	    header += "Date: " + formatted_time() + "\r\n";
	    header += "\r\n";
        header += body;
    }
}

void Response::get_headers(std::string file_name)
{
    header = "HTTP/1.1 200 ok\r\n";
	header += "Content-Type: " + get_file_type(file_name) + "\r\n";
	header += "Content-Length: "+ std::to_string(this->content_lenght) + "\r\n";
	header += "Server: mywebserv\r\n";
	header += "Date: " + formatted_time() + "\r\n";
	header += "\r\n";
}

void Response::delete_method(server_config &s, std::string path)
{
    std::string file_name;
    location_config loc = s.longest_prefix_match(path);
    path = get_file_name(path, loc.get_prefix());
    file_name = loc.get_root() + path;
    for (size_t i = 0 ; i < loc.get_methods().size();i++)
    {
        std::cout << loc.get_methods()[i] << std::endl;
    }
    if (!find_string(loc.get_methods(),"DELETE"))
    {
        set_status_code(405);
        return;
    }
    if (std::remove(file_name.c_str()))
        set_status_code(404);
    else
        set_status_code(200);
}

void Response::get_method(server_config &s,std::string path)
{
    if (search_for_default(s,path))
        return;
    location_config loc = s.longest_prefix_match(path);
    path = get_file_name(path, loc.get_prefix());
    std::string file_name = loc.get_root() + path;
    if (isDir(file_name) && loc.get_autoindex() == "on")
    {
        autoindex(file_name, loc.get_prefix(), loc.get_root());
        return;
    }
    if (!exists_test(file_name))
    {
        set_status_code(404);
        return;
    }
    if (!find_string(loc.get_methods(),"GET"))
    {
        set_status_code(405);
        return;
    }
    if (isDir(file_name) && loc.get_autoindex() == "off")
    {
        set_status_code(403);
        return;
    }
    this->body = file_name;
    this->content_lenght = fsize(file_name.c_str());
    get_headers(file_name);
    if (content_lenght + header.size() < SIZE_OF_BUFFER)
    {
        std::cout << "Number" << content_lenght - header.size() << std::endl;
        this->is_complete = true;
    }
}

void Response::post_method(server_config &s, request &req)
{
    location_config loc = s.longest_prefix_match(req.get_path());
    if (s.get_max_body_size() < size_t(fsize(req.get_body().c_str())))
    {
        remove(req.get_body().c_str());
        set_status_code(413);
        return ;
    }
    else if (!find_string(loc.get_methods(),"POST"))
    {
        set_status_code(405);
        return;
    }
    std::string new_file(strrchr(req.get_body().c_str(), '/') + get_file_ext(std::string(req.get_type())));
    std::string file_name = loc.get_upload_path() + "/" + new_file;
    std::ifstream in(req.get_body().c_str(), std::ios::in | std::ios::binary);
    std::ofstream out(file_name, std::ios::out | std::ios::binary);
    out << in.rdbuf();
    in.close();
    out.close();
    remove(req.get_body().c_str());
    set_status_code(201);
}

Response::Response(server_config server, request &req)
{
    if (req.get_error_flag() == 400)
    {
        set_status_code(400);
        generate_headers();
        return;
    }
    if (req.get_method() == "GET")
    {
        get_method(server, req.get_path());
        generate_headers();
    }
    else if (req.get_method() == "DELETE")
    {
        delete_method(server ,req.get_path());
        generate_headers();
    }
    else if (req.get_method() == "POST")
    {
        post_method(server, req);
        generate_headers();
    }
    else
    {
        set_status_code(400);
        generate_headers();
    }
}

