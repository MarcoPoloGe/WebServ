#include "../../includes/Webserv_Includes.hpp"

Request::Request()
{
}

Request::Request(std::string request)
{
	this->fill(request);
}

void Request::fill(std::string request)
{
	_raw_string = request;
	request.erase(std::remove(request.begin(), request.end(), 6), request.end());
	request.erase(std::remove(request.begin(), request.end(), '\r'), request.end());
	std::istringstream file(request);
	std::string temp;
	std::string first;
	std::string second;
	
	if(std::getline(file >> std::ws, temp, ' '))
	{
		temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
		if(!temp.empty() && (temp == "GET" || temp == "POST" || temp == "DELETE"))
			type = temp;
		else
			throw std::invalid_argument("@fn Request::fill(std::string request)\nInvalid HTTP request type : " + temp);
	}
	if(std::getline(file >> std::ws, temp, ' '))
	{
		temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
		if(!temp.empty())
			URI = temp;
		else
			throw std::invalid_argument("@fn Request::fill(std::string request)\nInvalid HTTP request URI");
	}
	if(std::getline(file >> std::ws, temp))
	{
		temp.erase(std::remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
		if(!temp.empty() && temp == HTTP_VERSION)
			HTTP_version = temp;
		else
			throw std::invalid_argument("@fn Request::fill(std::string request)\nInvalid HTTP request HTTP version : " + temp);
	}
	this->headers_map = process_headers(file);
	this->content_list = process_contents(file, this->headers_map);
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

std::string Request::get_header(std::string header_name) const
{
	std::map<std::string,std::string>::const_iterator temp;
	temp = this->headers_map.find(header_name);
	if(temp == this->headers_map.end())
		return(std::string());
	else
		return (temp->second);
}

std::string Request::get_content_header(std::string header_name) const
{
	if(!this->content_list.empty())
		return (content_list.at(0).get_header(header_name));
	return (std::string());
}

std::string Request::get_content_header(size_t content_nb, std::string header_name) const
{
	if(this->content_list.size() > content_nb)
		return (content_list.at(content_nb).get_header(header_name));
	return (std::string());
}

std::string Request::get_content_body() const
{
	if(!this->content_list.empty())
		return (content_list.at(0).get_body());
	return (std::string());
}

std::string Request::get_content_body(size_t  content_nb) const
{
	if(this->content_list.size() > content_nb)
		return (content_list.at(content_nb).get_body());
	return (std::string());
}

std::vector<RequestContent> Request::get_content_list() const
{
	return (this->content_list);
}

Request &Request::operator=(Request const &rhs)
{
	this->type = rhs.type;
	this->URI = rhs.URI;
	this->HTTP_version = rhs.HTTP_version;
	this->headers_map = rhs.headers_map;
	this->content_list = rhs.content_list;
	this->_upload_file = rhs._upload_file;
	this->_filename = rhs._filename;
	this->_raw_string = rhs._raw_string;
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

std::string Request::get_file_post(void) const
{
	return (_upload_file);
}

std::string Request::get_filename_post(void) const
{
	return (_filename);
}

std::string Request::get_raw_string(void) const
{
	return (_raw_string);
}

void Request::upload_file(std::string full_request)
{
	std::string boundary;
	std::string file_content;
	std::string filename;

	int bound_begin = full_request.find("boundary=");
	int bound_end = full_request.find("\r\n", bound_begin);

	boundary = full_request.substr(bound_begin + 9, bound_end - bound_begin - 9);

//	std::cout <<R<< "&&&&&BOUNDAAAAARY = [" << boundary << "]\n" <<RE;

	int start_file = full_request.find( "\r\n\r\n", full_request.find("--"+boundary) );
	int end_file = full_request.find("--"+boundary+"--");

	file_content = full_request.substr(start_file + 4, end_file - (start_file + 4) - 1);

	//std::cout <<R<< "FIIIIIIIIIIILE is {\n" << file_content << "\n}\n"<<RE;

	int filename_begin = full_request.find("filename=\"", full_request.find("--"+boundary) );
	int	filename_end = full_request.find("\"\r\n", filename_begin);

	filename = full_request.substr(filename_begin + 10, filename_end - filename_begin - 10);

	//std::cout <<R<< "FIIIILENAAAAME is {\n" << filename << "\n}\n"<<RE;

	_upload_file = file_content;
	_filename = filename;

}

std::ostream& operator<<(std::ostream& out, Request const& rhs)
{
	std::map<std::string,std::string> headers = rhs.get_headers_map();
	std::vector<RequestContent> content = rhs.get_content_list();
	std::string short_body;

	out << "--Request_start--" << std::endl;

	out << "type: " << rhs.get_type() << std::endl;
	out << "URI: " << rhs.get_URI() << std::endl;
	out << "HTTP_version: " << rhs.get_HTTP_version() << std::endl;
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		out << it->first << ": " << it->second <<  std::endl;
	}
	out << std::endl;
	for (std::vector<RequestContent>::iterator it = content.begin(); it != content.end(); it++)
	{
		out << "-content_start- nb "<< it - content.begin() << ":" << std::endl;
		out << *it;
		out << "-content_end- "<< it - content.begin() << ";" << std::endl;
	}
	out << std::endl;
	out << "--Request_end--" << std::endl;
	return (out);
}
