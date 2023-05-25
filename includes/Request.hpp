
#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include <string>
#include <map>
#include <list>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "RequestContent.hpp"

class Request {

public:
	Request();
	Request(std::string request);

	void fill(std::string request);

	std::string get_type() const;
	std::string get_URI() const;
	std::string get_HTTP_version() const;

	//gets information on the first content only;
		std::string get_header(std::string header_name) const;
		std::map<std::string, std::string> get_headers_map() const;
		std::string get_body() const;

	std::vector<RequestContent> get_content_list() const;

	Request &operator=(Request const &rhs);

	void setType(const std::string &type);

	void setUri(const std::string &uri);

	void setHttpVersion(const std::string &httpVersion);

	void setHeadersMap(const std::map<std::string, std::string> &headersMap);

	void setBody(const std::string &body);

private:
	std::string type;
	std::string URI;
	std::string HTTP_version;
	std::map<std::string,std::string> headers_map;
	std::vector<RequestContent> content_list;
};

std::ostream& operator<<(std::ostream& out, const Request& rhs);

#endif //WEBSERV_REQUEST_HPP
