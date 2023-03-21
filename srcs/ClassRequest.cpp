
#include "ClassRequest.hpp"

Request::Request()
{
}

Request::Request(std::string request)
{
	this->fill(request);
}

void Request::fill(std::string request)
{
	size_t next;
	std::string types[3] = {"GET ","PUT ","DELETE "};

	for (int i = 0; i < 3; ++i)
	{
		if(request.compare(0,3,types[i]))
		{
			this->type = types[i];
			request.erase(0,types->length());
			break;
		}
	}
	if(this->type.empty())
	{
		//todo error request cannot be handled;
	}
	next = request.find(' ');
	URI = request.substr(0, next);
	request.erase(0, next);

	next = request.find('\n');
	HTTP_version = request.substr(0, next);
	request.erase(0, next);

	std::istringstream file(request);
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream sline(line);
		std::string first;
		std::string second;
		if(std::getline(sline, first, ':'))
		{
			std::getline(sline, second);
			this->headers_map.insert(std::make_pair(first, second));
		}
	}
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
