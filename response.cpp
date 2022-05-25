#include "response.hpp"


WS::Response::Response()
{

}
WS::Response::~Response()
{
    
}



void WS::Response::set_status_code(int status)
{
    this->status_code = status;
}
void WS::Response::set_content_lenght(int cl)
{
    this->content_lenght = cl;
}
void WS::Response::set_content_type(std::string ct)
{
    this->content_type = ct;
}
void WS::Response::set_status(std::string status)
{
    this->status = status;
}
int WS::Response::get_status_code()
{
    return status_code;
}
int WS::Response::get_content_lenght()
{
    return content_lenght;
}
std::string WS::Response::get_content_type()
{
    return content_type;
}
std::string WS::Response::get_status()
{
    return status;
}


bool WS::Response::isDir(std::string path)
{
    struct stat statbuf;
	if (path == "/")
		return true;
	std::string s = path; 
	if (stat(s.c_str(), &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}




int WS::Response::search_for_path(server_config &s,std::string path)
{
    int i ,j;
    // std::map<int, server_config> mapserver;
    

    // // std::vector<location_config> locations; = s->get_locations();
    // for(j=0;j<s.get_servers().size(); j++)
    //     for (i =0; i < s.get_servers()[j].get_locations().size() ; i++)
    //     {
    //          if (s.get_servers()[j].get_locations()[i].get_default_file() == path)
    //         {
    //              mapserver.insert(std::make_pair(i,s.get_servers()[j]));
    //              return mapserver;
    //         }
    //     }
    // mapserver.insert(std::make_pair(-1,s.get_servers()[0]));       
    // return mapserver;
    std::vector<location_config> locations = s.get_locations();
    
        for (i =0; i < locations.size() ; i++)
             if (locations[i].get_default_file() == path)
                return i;


           return -1;     

}


void WS::Response::autoindex(std::string path)
{
    std::string header;
    std::vector<Fileinfos> file = listofFiles(path);
    std::string	body = "<html lang='en'>\n<head>\n<title>Document</title>\n</head>\n<body>\n<h1>Index OF "+ path + " </h1>\n<br>\n<table width=\"100%\">\n<hr>";
	


    for(int i =0; i < file.size(); i++)
    {
        std::string td = "<tr><td width=\"50%\"> <a href=\"/"+ path + "/"+ file[i].file_name +"\"> "+file[i].file_name + "</a></td>"  ;
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


void WS::Response::get_file(std::string file_name)
{
    std::ifstream f(file_name);
     if(f)
     {
        std::ostringstream ss;
        ss << f.rdbuf();
        body = ss.str();  // the contenant of the file in the body / check the contenant lenght and the extention
    }
    // the header 
    header = "HTTP/1.1 200 ok\r\n";
	header += "Content-Type: " + get_file_type(file_name) + "\r\n";
	header += "Content-Length: "+ std::to_string(body.size()) + "\r\n";
	header += "Server: mywebserv\r\n";
	header += "Date: " + formatted_time() + "\r\n";
	header += "\r\n";
    header += body;
}
void WS::Response::generate_errors()
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
}

void WS::Response::delete_method(std::string filename)
{
    int status;
    std::remove(filename.c_str());
    if (errno == ENOENT)
        set_status_code(404);
    else
        set_status_code(200);
}

void WS::Response::get_method(server_config &s,std::string path)
{
    int index = search_for_path(s, path);
    std::vector<location_config> locations = s.get_locations();
    if (index == -1)
    { 
        set_status_code(404);
        return;
    }
    std::string file_name = locations[index].get_root() + "/" + path;
    std::vector<std::string>::iterator it;
    it = std::find(locations[index].get_methods().begin(),locations[index].get_methods().end(),"GET") ;
    if (it == locations[index].get_methods().end())
    {
        set_status_code(405);
        return;
    }
    if (isDir(file_name) && locations[index].get_autoindex() == "of")
    {
        set_status_code(403);
        return;
    }
    if (isDir(file_name) && locations[index].get_autoindex() == "on")
    {
        autoindex(file_name);
        return;
    }
    // 413
    get_file(file_name);
}

WS::Response::Response(server_config &server, request &req)
{
    if (req.get_method() == "GET")
    {
        get_method(server, req.get_path());
    }
    else if (req.get_method() == "DELETE")
    {
    }
    else if (req.get_method() == "POST")
    {
    }
}