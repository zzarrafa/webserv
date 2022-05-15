#ifndef PARSEFILE_HPP
#define PARSEFILE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "server.hpp"
#include "../tools.hpp"

class parsefile{
	public:
	std::vector<server_config> servers;
	
};

#endif