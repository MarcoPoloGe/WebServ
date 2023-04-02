#include "../includes/Webserv_Includes.hpp"

Response::Response() : error_code(200)
{

}

Response::Response(Response &rhs)
{
	this->error_code = rhs.error_code;
	this->content_type = rhs.content_type;
	this->content = rhs.content;
}

Response::Response(int error_code)
{
    set_error_code(error_code);
}

Response::Response(std::string content, std::string content_type)
{
    set_error_code(200);
    set_content(content, content_type);
}

int Response::get_error_code() const
{
	return (this->error_code);
}

std::string Response::get_content() const
{
	return (this->content);
}

std::string Response::get_content_type() const
{
	return (this->content_type);
}

void Response::set_error_code(int error_code)
{
    //todo check if error_code is valid when config file is working
	this->error_code = error_code;
}

void Response::set_content(std::string content, std::string content_type)
{
	this->content = content;
	//todo check if content_extention is good when config file is working;
	this->content_type = content_type;
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

std::string Response::send() const
{
	std::stringstream message;

	message << HTTP_VERSION << " "
	<< this->get_error_code() << " "
	<< ft_error_def(this->get_error_code()) << std::endl;

	if(error_code != 200)
	{
        set_content(ft_error_file(this->error_code), "text/html");
	}
	else if(this->content.empty() || this->content_type.empty())
	{
		//todo error response is empty;
	}
	message << "Content-Length:" << this->content.length() << std::endl;
	message << "Content-Type:" << this->content_type << std::endl;
	//todo maybe add more headers like date ect.
	message << std::endl;
	message << this->content;
	return (message.str());
}

Response Response::operator=(Response &rhs)
        {
    this->error_code = rhs.error_code;
    this->content_type = rhs.content_type;
    this->content = rhs.content;
}

std::ostream& operator<<(std::ostream& out, Response const& rhs)
{
	std::string short_content;

	out << "--Response_start--" << std::endl;

	out << "error_code : " << rhs.get_error_code() << std::endl;
	out << "content_type : " << rhs.get_content_type() << std::endl;

	short_content = rhs.get_content().substr(0,20);
	out << "content : " << short_content;
	if(rhs.get_content().length() > 20)
		out << "...";
	out << std::endl;

	out << "--Response_end--" << std::endl;
	return (out);
}