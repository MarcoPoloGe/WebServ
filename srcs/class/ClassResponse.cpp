#include "../../includes/Webserv_Includes.hpp"

Response::Response(Config config) : _config(config), _error_code(200)
{
}

Response::Response(Response &rhs)
{
	this->_config = rhs._config;
	this->_error_code = rhs._error_code;
	this->_content_type = rhs._content_type;
	this->_content = rhs._content;
}

Response::Response(int error_code, Config config) : _config(config)
{
    set_error_code(error_code);
}

Response::Response(std::string content, std::string content_ext, Config config) : _config(config)
{
    set_error_code(200);
    set_content(content, content_ext);
}

int Response::get_error_code() const
{
	return (this->_error_code);
}

std::string Response::get_content() const
{
	return (this->_content);
}

std::string Response::get_content_type() const
{
	return (this->_content_type);
}

void Response::set_error_code(int error_code)
{
    //todo check if error_code is valid when config file is working
	this->_error_code = error_code;
}

void Response::set_content(std::string content, std::string content_ext)
{
	this->set_content_body(content);
	this->set_content_ext(content_ext);
}

void Response::set_content_body(std::string body)
{
	this->_content = body;
}

void Response::set_content_type(std::string type)
{
	this->_content_type = type;
}

void Response::set_content_ext(std::string extension)
{
	this->_content_type = this->_config.getPathFormat(extension);
}

std::string Response::ft_error_def(int error_code)
{
	//todo when config file is working.
	(void) error_code;
	return ("OK");
}

std::string Response::ft_error_file(int error_code)
{
    //todo when config file is working.
    (void) error_code;
    return ("<h1>ERROR 404 file not found</h1>");
}

std::string Response::send(int client_socket)
{
	std::stringstream message;

	//building the message to send;
	message << HTTP_VERSION << " "
	<< this->get_error_code() << " "
	<< ft_error_def(this->get_error_code()) << std::endl;

	if(_error_code != 200)
	{
        set_content(ft_error_file(this->_error_code), "text/html");
	}
	else if(get_content().empty() || get_content_type().empty())
	{
		throw std::invalid_argument("can't send response because it is incomplete");
	}
	message << "Content-Length:" << get_content().length() << std::endl;
	message << "Content-Type:" << get_content_type() << std::endl;
	//todo maybe add more headers like date ect.
	message << std::endl;
	message << get_content();

	::send(client_socket, message.str().c_str(), message.str().length(),0);
	//returns the message for debug purposes.
	return (message.str());
}

Response Response::operator=(Response &rhs)
		{
    this->_error_code = rhs._error_code;
    this->_content_type = rhs._content_type;
    this->_content = rhs._content;
	return (*this);
}

std::ostream& operator<<(std::ostream& out, Response const& rhs)
{
	std::string short_content;

	out << "--Response_start--" << std::endl;

	out << "error_code : " << rhs.get_error_code() << std::endl;
	out << "content_type : " << rhs.get_content_type() << std::endl;

	short_content = rhs.get_content().substr(0,30);
	out << "content : " << short_content;
	if(rhs.get_content().length() > 30)
		out << "...";
	out << std::endl;

	out << "--Response_end--" << std::endl;
	return (out);
}
