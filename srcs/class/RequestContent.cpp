
#include "../../includes/RequestContent.hpp"
#include "../../includes/Webserv_Includes.hpp"

RequestContent::RequestContent()
{
}

RequestContent::RequestContent(std::string raw_content)
{
	this->fill(raw_content);
}

void RequestContent::fill(std::string raw_content)
{
	std::string temp;
	std::istringstream raw(raw_content);
	this->headers_map = process_headers(raw);
	while (std::getline(raw >> std::ws, temp))
	{
		this->body += temp;
		if(!raw.eof())
			this->body += '\n';
	}
}

std::map<std::string, std::string> RequestContent::get_headers_map() const
{
	return (this->headers_map);
}

std::string RequestContent::get_body() const
{
	return (this->body);
}

std::string RequestContent::get_header(std::string header_name) const
{
	std::map<std::string,std::string>::const_iterator temp;
	temp = this->headers_map.find(header_name);
	if(temp == this->headers_map.end())
		return(std::string());
	else
		return (temp->second);
}

RequestContent &RequestContent::operator=(RequestContent const &rhs)
{
	this->headers_map = rhs.headers_map;
	this->body = rhs.body;
	return (*this);
}

void RequestContent::setHeadersMap(const std::map<std::string, std::string> &headersMap) {
	headers_map = headersMap;
}

void RequestContent::setBody(const std::string &body) {
	this->body = body;
}

std::ostream& operator<<(std::ostream& out, RequestContent const& rhs)
{
	std::map<std::string,std::string> headers = rhs.get_headers_map();
	std::string short_body;

	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		out << it->first << ": " << it->second <<  std::endl;
	}
	out << std::endl;
	short_body = rhs.get_body().substr(0,1000);
	out << short_body;
	if(rhs.get_body().length() > 1000)
		out << std::endl << "...";
	return (out);
}
