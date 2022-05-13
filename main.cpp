// #include "listening.hpp"
// #include "binding.hpp"
// #include <iostream>


// int main()
// {
//     std::cout << "Starting.." << std::endl;
//     // WS::Socket ss = WS::Socket(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY);
//     std::cout << "Binding socket.." << std::endl;
//     WS::Binding bs = WS::Binding(AF_INET, SOCK_STREAM, 0, 81, INADDR_ANY);
//     std::cout << "listening.." << std::endl;
//     WS::Listening ls = WS::Listening(AF_INET, SOCK_STREAM,0,80,INADDR_ANY,10);
//     std::cout << "success!" << std::endl;
   

// }
#include "tools.hpp"
#include "Request.hpp"

// int main()
// {
// std::string buff = "GET /favicon.ico HTTP/1.1\nHost: 127.0.0.1\nConnection: keep-alive\nsec-ch-ua-mobile: ?0\nsec-ch-ua-platform: macOS\nAccept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\nSec-Fetch-Site: same-origin\nSec-Fetch-Mode: no-cors\nSec-Fetch-Dest: image\n";
// WS::Request req;
// req.ParseHeader(buff);
// std::cout << req.get_method() << std::endl;
// std::cout << req.get_url() << std::endl;
// std::cout << req.get_version() << std::endl;
// }