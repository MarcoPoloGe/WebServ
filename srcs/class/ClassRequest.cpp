#include "../../includes/Webserv_Includes.hpp"

Request::Request()
{
}

Request::Request(std::string request)
{
	this->fill(request);
}

bool Request::fill(std::string request)
{
	std::istringstream file(request);
	std::string line;
	std::string first;
	std::string second;

	try {

		if(std::getline(file, first, ' '))
		{
			first.erase(std::remove_if(first.begin(), first.end(), ::isspace), first.end());
			if(!first.empty() && (first == "GET" || first == "PUT" || first == "DELETE"))
				type = first;
			else
				throw std::invalid_argument("Invalid HTTP request type");
		}
		if(std::getline(file, first, ' '))
		{
			first.erase(std::remove_if(first.begin(), first.end(), ::isspace), first.end());
			if(!first.empty())
				URI = first;
			else
				throw std::invalid_argument("Invalid HTTP request URI");
		}
		if(std::getline(file, first))
		{
			first.erase(std::remove_if(first.begin(), first.end(), ::isspace), first.end());
			if(!first.empty() && first == HTTP_VERSION)
				HTTP_version = first;
			else
				throw std::invalid_argument("Invalid HTTP request HTTP version");
		}
		while (std::getline(file, line))
		{
			line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
			if (line.empty())
				break;
			std::istringstream sline(line);
			if(std::getline(sline, first, ':'))
			{
				if(std::getline(sline >> std::ws, second)) // >> std:ws skips white spaces before reading. It is used so that the string 'second' doesn't store the space right after the :
				{
					if(first.empty() || second.empty())
						throw std::invalid_argument("Invalid HTTP request header");
					headers_map.insert(std::make_pair(first, second));
				}
			}
		}
	}
	catch(std::invalid_argument &e)
	{
		std::cout << e.what() << std::endl;
		return (false);
	}
	return (true);
}

std::string Request::get_type() const
{
	return (this->type);
}
std::string Request::get_URI() const
{
	return (this->URI);
}
std::string Request::get_HTTP_version() const
{
	return (this->HTTP_version);
}

std::map<std::string, std::string> Request::get_headers_map() const
{
	return (this->headers_map);
}

//Returns string.empty if it did not find a corresponding header_name
std::string Request::get_header(std::string header_name) const
{
	return (this->headers_map.find(header_name)->second);
}

std::ostream& operator<<(std::ostream& out, Request const& rhs)
{
	std::map<std::string,std::string> headers = rhs.get_headers_map();

	out << "--Request_start--" << std::endl;
	out << "type : " << rhs.get_type() << std::endl;
	out << "URI : " << rhs.get_URI() << std::endl;
	out << "HTTP_version : " << rhs.get_HTTP_version() << std::endl;

	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		out << it->first << " : " << it->second << std::endl;
	}
	out << "--Request_end--" << std::endl;
	return (out);
}
