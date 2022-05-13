

#include <unistd.h>
#include <iostream>
#include <map>
#include "tools.hpp"

namespace WS{
    class Request 
    {
        private:
        std::string   buff;
		std::string    method;
		std::string    url;
		float      version;
		std::map<std::string, std::string> headers;
		std::string         body;
        public:
        Request();
        ~Request();
        std::string get_method();
        std::string get_url();
        float get_version();
		// std::map<std::string, std::string>  get_headers();
        void ParseHeader(std:: string buff);
    };
}