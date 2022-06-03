#include "response.hpp"

Response::Response()
{
    this->status_code = 200;
    this->auto_index = "";
    this->content_lenght = 0;
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


void Response::set_status_code(int status)
{
    this->status_code = status;
}
void Response::set_content_lenght(int cl)
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
int Response::get_content_lenght()
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

void Response::get_file(std::string file_name)
{
    std::ifstream f(file_name);
     if(f)
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

void Response::generate_headers()
{
    if (status_code == 404)
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
}

void Response::delete_method(server_config &s, std::string path)
{
    std::string file_name;
    location_config loc = s.longest_prefix_match(path);
    path = get_file_name(path, loc.get_prefix());
    file_name = loc.get_root() + path;
    for (size_t i=0 ; i < loc.get_methods().size();i++)
    {
        std::cout << loc.get_methods()[i] << std::endl;
    }
    if (!find_string(loc.get_methods(),"DELETE"))
    {
        set_status_code(405);
        return;
    }
    // printf("file path ++++ %s\n",file_path.c_str());
    if (std::remove(file_name.c_str()))
        set_status_code(404);
    else
        set_status_code(200);
}

// function to create a directory


void Response::get_method(server_config &s,std::string path)
{
    std::cout << "path>> " << path << std::endl;
    s.print_server();
    if (search_for_default(s,path))
        return;
        std::cout << "!!!!!!!!!!!!!!!!!!!" << std::endl;
    location_config loc = s.longest_prefix_match(path);
    path = get_file_name(path, loc.get_prefix());
    std::string file_name = loc.get_root() + path;
    if (isDir(file_name) && loc.get_autoindex() == "on")
    {
        std::cout << "autoindex on" << std::endl;
        autoindex(file_name, loc.get_prefix(), loc.get_root());
        return;
    }
    if (!exists_test(file_name))
    {
        std::cout << "file not found" << std::endl;
        set_status_code(404);
        return;
    }
    if (!find_string(loc.get_methods(),"GET"))
    {
        std::cout << "method not allowed" << std::endl;
        set_status_code(405);
        return;
    }
    if (isDir(file_name) && loc.get_autoindex() == "off")
    {
        std::cout << "autoindex off" << std::endl;
        set_status_code(403);
        return;
    }
    // 413
    get_file(file_name);
}

// void Response::post_method(server_config &s, request &req)
// {
    // int index = search_for_path(s, req.get_path());
    // if (index != -1)
    //     set_status_code(409); // conflict
    // else
    // {
    //     std::ofstream outfile (req.get_path());
    //     outfile << req.get_body();
    //     outfile.close();
    //     set_status_code(201); //created

        // std::string content;
        // if (infile && outfile)
        // for(int i=0; !infile.eof(); i++)     // get content of infile
        // content += infile.get();
        // infile.close();
        // content.erase(content.end()-1);
        // outfile << content;
        // outfile.close();


    // }
// }

Response::Response(server_config server, request &req)
{
    // std::cout << server.get_locations()[0].get_root() << std::endl;
    if (req.get_method() == "GET")
    {
        std::cout << "GET" << std::endl;
        server.print_server();
        req.print_request();

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
        std::cout << "post method\n";
        // generate_headers();
    }
}
