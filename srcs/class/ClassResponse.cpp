#include "../../includes/Webserv_Includes.hpp"

Response::Response(Config config) : _config(config), _error_code(200)
{
}

Response::Response(Response const &rhs)
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

Response::Response(std::string path, Config config) : _config(config), _error_code(200)
{
	set_path(path);
}

int Response::get_error_code() const
{
	return (this->_error_code);
}

std::string Response::get_content_body() const
{
	return (this->_content);
}

std::string Response::get_content_type() const
{
	return (this->_content_type);
}

/////////////////TRICK TEST //////////////////////

void Response::set_manual_content_type(std::string content_type)
{
	this->_content_type = content_type;
}

void Response::set_manual_content(std::string content)
{
	this->_content = content;
}
/////////////////TRICK TEST //////////////////////

void Response::set_path(std::string path)
{
	if(path.empty())
		throw std::invalid_argument("@fn void Response::set_path(std::string path)\npath is empty");
	set_error_code(200);
	std::string ext;

	ext = ft_get_extension(path);
	if(ext.empty())
		throw std::invalid_argument("@fn void Response::set_path(std::string path)\npath : "+ path +" has no exception");
	set_content_body(ft_read_file(path));
	set_content_extension(ext);

}

void Response::set_error_code(int error_code)
{
	if(this->_config.getErrorNames(error_code).empty())
		throw std::invalid_argument("@fn void Response::set_error_code(int error_code)\nerror_code does not exist\n");
	this->_error_code = error_code;
}

void Response::set_content_body(std::string body)
{
	this->_content = body;
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
		throw std::invalid_argument("@fn std::string Response::ft_error_name(int error_code)\nerror_name cannot be found\n");
	return (name);
}

std::string Response::ft_error_page(int error_code)
{
	std::string page_name;

	page_name = _config.getErrorPages(error_code);
	if(page_name.empty())
	{
		std::stringstream message;
		message << "@fn std::string Response::ft_error_page(int error_code)\nerror_page cannot be found :" << error_code << " please add it in config file." <<std::endl;
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
	std::cout <<W<< "@fn Response::send(int client_socket)" <<RE<< std::endl;
	message << HTTP_VERSION << " "
	<< this->get_error_code() << " "
	<< ft_error_name(this->get_error_code()) << std::endl;

	if(_error_code != 200)
	{
		std::cout <<W<< "@fn Response::send(int client_socket)" <<RE<< std::endl;
		std::cout <<B<< "my errcode is :" << _error_code << std::endl <<RE; //DEBUG
        set_content_body(ft_error_page(this->_error_code));
		set_content_extension("html");
	}
	else if(get_content_body().empty() || get_content_type().empty())
	{
		throw std::invalid_argument("@fn Response::send(int client_socket)\nCan't send response because it is incomplete");
	}
	message << "Content-Length:" << get_content_body().length() << std::endl;
	message << "Content-Type:" << get_content_type() << std::endl;
	message << std::endl;
	message << get_content_body();

	std::cout <<Y<< "My reponse is : \n" << message.str() <<RE<<std::endl;//DEBUG
	::send(client_socket, message.str().c_str(), message.str().length(),0);
	//returns the message for debug purposes.
	return (message.str());
}

Response &Response::operator=(const Response &rhs)
		{
			this->_error_code = rhs._error_code;
			this->_content_type = rhs._content_type;
			this->_content = rhs._content;
			return (*this);
		}

std::ostream& operator<<(std::ostream& out, Response const& rhs)
{
	std::string short_content;

//	out <<W<< "@fn operator<<(std::ostream& out, Response const& rhs)" <<RE<< std::endl;
	out << "--Response_start--" << std::endl;

	out << "error_code : " << rhs.get_error_code() << std::endl;
	out << "content_type : " << rhs.get_content_type() << std::endl;

	short_content = rhs.get_content_body().substr(0,100);
	out << "content : " << short_content;
	if(rhs.get_content_body().length() > 100)
		out << "...";
	out << std::endl;

	out << "--Response_end--" << std::endl;
	return (out);
}
