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
	if(this->_config.getErrorNames(error_code).empty())
		throw std::invalid_argument("error_code does not exist\n");
	this->_error_code = error_code;
}

void Response::set_content(std::string content, std::string content_ext)
{
	this->set_content_body(content);
	this->set_content_extension(content_ext);
}

void Response::set_content_body(std::string body)
{
	this->_content = body;
}

void Response::set_content_type(std::string type)
{
	this->_content_type = type;
}

void Response::set_content_extension(std::string extension)
{
	this->_content_type = this->_config.getType(extension);
}

std::string Response::ft_error_name(int error_code)
{
	std::string name;

	name = _config.getErrorNames(error_code);
	if(name.empty())
		throw std::invalid_argument("error_name cannot be found\n");
	return (name);
}

std::string Response::ft_error_page(int error_code)
{
	std::string page_name;

	page_name = _config.getErrorPages(error_code);
	if(page_name.empty())
	{
		std::stringstream message;
		message << "error_page cannot be found :" << error_code << " please add it in config file." <<std::endl;
		throw std::invalid_argument(message.str());
	}
	return (page_name);
}

std::string Response::send(int client_socket)
{
	if ( _content_type.empty() || _content.empty() )
	{
//		std::cout << B << "Nothing send : it was empty\n" << RE; // DEBUG
		return ("");
	}

	std::stringstream message;

	//building the message to send;
	message << HTTP_VERSION << " "
	<< this->get_error_code() << " "
	<< ft_error_name(this->get_error_code()) << std::endl;

	if(_error_code != 200)
	{
		std::cout <<W<< "my errcode is :" << _error_code << std::endl <<RE; //DEBUG
        set_content(ft_error_page(this->_error_code), "text/html");
	}
	else if(get_content().empty() || get_content_type().empty())
	{
		throw std::invalid_argument("can't send response because it is incomplete");
	}
	message << "Content-Length:" << get_content().length() << std::endl;
	message << "Content-Type:" << get_content_type() << std::endl;
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

	short_content = rhs.get_content().substr(0,100);
	out << "content : " << short_content;
	if(rhs.get_content().length() > 100)
		out << "...";
	out << std::endl;

	out << "--Response_end--" << std::endl;
	return (out);
}
