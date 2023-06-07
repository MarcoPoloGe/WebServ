#ifndef WEBSERV_INCLUDES_HPP
# define WEBSERV_INCLUDES_HPP

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <algorithm>
#include <map>
#include <list>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <utility>
#include <signal.h>
#include <unistd.h>


#include "Response.hpp"
#include "Request.hpp"
#include "Ft_error.hpp"
#include "Config.hpp"
#include "Network.hpp"
#include "CGI.hpp"
#include "LAN.hpp"

#define BACKLOG 2
#define MAX_CLIENTS 5
#define HTTP_VERSION "HTTP/1.1"

//COLOR OUTPUT//
#define	B		"\033[1m\033[34m" 		// Bold Blue
#define R		"\033[1m\033[31m"		// Bold Red
#define G		"\033[1m\033[32m"		// Bold Green
#define Y		"\033[1m\033[33m"		// Bold Yellow
#define W		"\033[1m\033[37m"		// Bold White
#define C		"\033[1m\033[36m"		// Bold Cyan

#define	RE   	"\033[0m"				// Reset


//NoClass Functions prototypes//
std::string		ft_read_file(std::string file_name);
void			setnonblocking(int sock);
std::string		ft_get_extension(std::string str);
std::string		ft_remove_ack(std::string str);
std::string		ft_generate_html_dir(std::string dir_path, int final_slash);
std::string		ft_what_location(std::string URI);
std::string 	numberToString(unsigned long n);
std::string 	IsQuery(const std::string &URIraw);
std::string 	str_toupper(std::string &str);
char *const 	*maptoarray(std::map<std::string, std::string> &map);
std::string		ft_generate_error_html(int error, Config config);
std::string		ft_generate_error_html(int error, Config config, std::string more);
std::string		ft_generate_success_delete(Request request);
std::string		ft_generate_success_upload(Request request);
bool			isFile(const std::string &path);
bool			isDirectory(const std::string &path);

//RequestUtils//
void process_multipart_header(std::string &raw_header, std::string header_name, std::map<std::string,std::string> &headers_map);
std::map<std::string,std::string> process_headers(std::istringstream &file);
std::vector<RequestContent> process_contents(std::istringstream &file, std::map<std::string,std::string> &headers_map);
#endif
