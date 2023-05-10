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
	request.erase(std::remove(request.begin(), request.end(), 6), request.end());
	std::istringstream file(request);
	std::string temp;
	std::string first;
	std::string second;
	
	try {

		if(std::getline(file, temp, ' '))
		{
			temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
			if(!temp.empty() && (temp == "GET" || temp == "POST" || temp == "DELETE"))
				type = temp;
			else
				throw std::invalid_argument("@fn Request::fill(std::string request)\nInvalid HTTP request type : " + temp);
		}
		if(std::getline(file, temp, ' '))
		{
			temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
			if(!temp.empty())
				URI = temp;
			else
				throw std::invalid_argument("@fn Request::fill(std::string request)\nInvalid HTTP request URI");
		}
		if(std::getline(file, temp))
		{
			temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
			if(!temp.empty() && temp == HTTP_VERSION)
				HTTP_version = temp;
			else
				throw std::invalid_argument("@fn Request::fill(std::string request)\nInvalid HTTP request HTTP version : " + temp);
		}
		while (std::getline(file, temp))
		{
			temp.erase(std::remove(temp.begin(), temp.end(), '\r'), temp.end());
			temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());
			if (temp.empty())
				break;
			std::istringstream stemp(temp);
			if(std::getline(stemp, first, ':'))
			{
				if(std::getline(stemp >> std::ws, second)) // >> std:ws skips white spaces before reading
				{
					if(first.empty() || second.empty())
						throw std::invalid_argument("@fn Request::fill(std::string request)\nInvalid HTTP request header");
					headers_map.insert(std::make_pair(first, second));
				}
			}
		}

//		/* Gather infos by Lowell */
//		for(std::map<std::string, std::string >::const_iterator it = headers_map.begin();
//			it != headers_map.end(); ++it)
//		{
//			std::cout <<R<< it->first <<Y<<" " << it->second <<RE<<"\n";
//		}

		if(this->type == "POST")
		{
			file >> std::ws;
			if(getline(file, temp, '\0'))// read everything left in the request;
			{
				if(!temp.empty())
					this->body = temp;
				else
					throw std::invalid_argument("@fn Request::fill(std::string request)\nInvalid HTTP request body");
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

std::string Request::get_body() const
{
	return (this->body);
}

std::string Request::get_header(std::string header_name) const
{
	std::map<std::string,std::string>::const_iterator temp;
	temp = this->headers_map.find(header_name);
	if(temp == this->headers_map.end())
		return(std::string());
	else
		return (temp->second);
}

Request &Request::operator=(Request const &rhs)
{
	this->type = rhs.type;
	this->URI = rhs.URI;
	this->HTTP_version = rhs.HTTP_version;
	this->headers_map = rhs.headers_map;
	this->body = rhs.body;
	return (*this);
}

void Request::setType(const std::string &type) {
	Request::type = type;
}

void Request::setUri(const std::string &uri) {
	URI = uri;
}

void Request::setHttpVersion(const std::string &httpVersion) {
	HTTP_version = httpVersion;
}

void Request::setHeadersMap(const std::map<std::string, std::string> &headersMap) {
	headers_map = headersMap;
}

void Request::setBody(const std::string &body) {
	Request::body = body;
}

std::ostream& operator<<(std::ostream& out, Request const& rhs)
{
	std::map<std::string,std::string> headers = rhs.get_headers_map();
	std::string short_body;

	out << "--Request_start--" << std::endl;

	out << "type : " << rhs.get_type() << std::endl;
	out << "URI : " << rhs.get_URI() << std::endl;
	out << "HTTP_version : " << rhs.get_HTTP_version() << std::endl;
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		out << it->first << ": " << it->second <<  std::endl;
	}
	out << std::endl;
	short_body = rhs.get_body().substr(0,1000);
	out << short_body;
	if(rhs.get_body().length() > 1000)
		out << std::endl << "...";
	out << std::endl;

	out << "--Request_end--" << std::endl;
	return (out);
}
