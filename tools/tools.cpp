#include "tools.hpp"


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