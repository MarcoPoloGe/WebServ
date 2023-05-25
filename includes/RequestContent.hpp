
#ifndef WEBSERV_REQUEST_CONTENT_HPP
#define WEBSERV_REQUEST_CONTENT_HPP

#include <string>
#include <map>
#include <list>
#include <iomanip>
#include <iostream>
#include <sstream>

class RequestContent {

public:
	RequestContent();
	RequestContent(std::string raw_content);

	void fill(std::string raw_content);

	std::string get_header(std::string header_name) const;
	std::map<std::string, std::string> get_headers_map() const;
	std::string get_body() const;

	RequestContent &operator=(RequestContent const &rhs);

	void setHeadersMap(const std::map<std::string, std::string> &headersMap);

	void setBody(const std::string &body);

private:
	std::map<std::string,std::string> headers_map;
	std::string body;
};

std::ostream& operator<<(std::ostream& out, const RequestContent& rhs);

#endif
