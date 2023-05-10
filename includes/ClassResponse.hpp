
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
	Response(Response const &rhs);
	Response(int error_code, Config config);
	Response(std::string path, Config config);

	int get_error_code() const;
	std::string get_content_type() const;
	std::string get_content_body() const;

	void set_path(std::string path);
	void set_error_code(int error_code);
	void set_content_body(std::string body);
	void set_content_extension(std::string extension); // setter content-type from extension ex : html > text/html txt > text/plain

	////TRICK TEST////
	void	set_manual_content_type(std::string content_type);
	void	set_manual_content(std::string content);
	////TRICK TEST/////

	std::string send(int client_socket);

	Response &operator=(Response const &rhs);

private:
	Config _config;
	int _error_code;
	std::string _content_type; //ex text/plain image/jpeg
	std::string _content;  //what will be put in the body of the response
	std::string _URIPathClean;
public:
	const Config &getConfig() const;

public:
	void setUriPathClean(const std::string &uriPathClean);

public:
	const std::string &getUriPathClean() const;

private:

	//these are helper functions;
		//returns the name of the error_code ex: 200 -> OK
		std::string ft_error_name(int error_code);
        //returns the content of the file to be displayed in correspondence to the error.
        std::string ft_error_page(int error_code);
};

std::ostream& operator<<(std::ostream& out, const Response& rhs);

#endif //WEBSERV_RESPONSE_HPP
