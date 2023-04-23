
#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP
#include <string>
#include <map>
#include <list>
#include <iomanip>
#include <iostream>
#include <sstream>

class Request {

public:
	Request();
	Request(std::string request);
	bool fill(std::string request);  //modifie val de retour de void a bool (reno 04.04.23)

	std::string get_type() const;
	std::string get_URI() const;
	std::string get_HTTP_version() const;
	std::string get_header(std::string header_name) const;
	std::map<std::string, std::string> get_headers_map() const;
	std::string get_body() const;

	Request &operator=(Request const &rhs);

private:
	std::string type;
	std::string URI; // le lien donné apres la requete. GET -> /lelien.html <- HTML/1.1
	std::string HTTP_version;
	std::map<std::string,std::string> headers_map;
	std::string body;
};

std::ostream& operator<<(std::ostream& out, const Request& rhs);

#endif //WEBSERV_REQUEST_HPP
