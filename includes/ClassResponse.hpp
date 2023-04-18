
#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP
#include <string>
#include <map>
#include <list>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Config.hpp"

class Response {

public:
	Response(Config config);
	Response(Response&);
	Response(int error_code, Config config);
	Response(std::string content, std::string content_type, Config config);

	int get_error_code() const;
	std::string get_content_type() const;
	std::string get_content() const;

	void set_error_code(int error_code);
	void set_content(std::string content, std::string content_type);
	void set_content_body(std::string body);
	void set_content_type(std::string type); // setters content-type ex : text/html text/plain
	void set_content_ext(std::string extension); // setter content-type from extension ex : html > text/html txt > text/plain

	std::string send(int client_socket);

private:
	Config _config;
	int _error_code;
	std::string _content_type; //ex text/plain image/jpeg
	std::string _content;  //what will be put in the body of the response
	Response operator=(Response&);

	//these are helper functions;
		//returns the name of the error_code ex: 200 -> OK
		static std::string ft_error_def(int error_code);
        //returns the content of the file to be displayed in correspondence to the error.
        static std::string ft_error_file(int error_code);
};

std::ostream& operator<<(std::ostream& out, const Response& rhs);

#endif //WEBSERV_RESPONSE_HPP
