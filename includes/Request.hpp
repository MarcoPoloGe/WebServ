
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
	std::string get_header(std::string header_name) const;
	std::map<std::string, std::string> get_headers_map() const;

	//gets information on the first content only;
		std::string get_content_header(std::string header_name) const;
		std::string get_content_body() const;

	//gets information on the specified content;
		std::string get_content_header(size_t  content_nb, std::string header_name) const;
		std::string get_content_body(size_t  content_nb) const;
	//getter form test Reno
		std::string get_file_post(void) const;
		std::string get_filename_post(void) const;
		std::string	get_raw_string(void) const;

	std::vector<RequestContent> get_content_list() const;

	Request &operator=(Request const &rhs);

	void setType(const std::string &type);

	void setUri(const std::string &uri);

	void setHttpVersion(const std::string &httpVersion);

	void setHeadersMap(const std::map<std::string, std::string> &headersMap);

	void setBody(const std::string &body);

	void upload_file(std::string full_request);

private:
	std::string type;
	std::string URI;
	std::string HTTP_version;
	std::map<std::string,std::string> headers_map;
	std::vector<RequestContent> content_list;

	////////////TEST/////////////
	
	std::string _upload_file;
	std::string _filename;
	std::string _raw_string;

};

std::ostream& operator<<(std::ostream& out, const Request& rhs);

#endif //WEBSERV_REQUEST_HPP
