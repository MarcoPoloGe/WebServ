
#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP
#include <string>
#include <map>
#include <list>
#include <iomanip>
#include <iostream>
#include <sstream>

class Response {

public:
	Response();
	Response(Response&);
	Response(int error_code);
	Response(std::string content, std::string content_type);

	int get_error_code() const;
	std::string get_content_type() const;
	std::string get_content() const;

	void set_error_code(int error_code);
	void set_content(std::string content, std::string content_type);

	std::string send() const;

private:
	int error_code;
	std::string content_type; //ex text/plain image/jpeg
	std::string content;  //what will be put in the body of the response
	Response operator=(Response&);

	//these are helper functions for send();
		//returns the name of the error_code ex: 200 -> OK
		static std::string ft_error_def(int error_code);
        //returns the string to be displayed in corespondance to the error code.
        static std::string ft_error_file(int error_code);
};

std::ostream& operator<<(std::ostream& out, const Response& rhs);

#endif //WEBSERV_RESPONSE_HPP
