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
#include <utility>


#include "Response.hpp"
#include "Request.hpp"
#include "RequestContent.hpp"
#include "Ft_error.hpp"
#include "Config.hpp"
#include "Network.hpp"
#include "CGI.hpp"
#include "LAN.hpp"

#define BACKLOG 2
#define MAX_CLIENTS 5
//#define BUFFER_SIZE 100
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
//unsigned short	ft_bswap16(unsigned short x);
//unsigned short	ft_htons(unsigned short s);
//unsigned int	ft_bswap32(unsigned int x);
//unsigned int	ft_htonl(unsigned int l);
void			setnonblocking(int sock);
std::string		ft_get_extension(std::string str);
std::string		ft_remove_nonprintable(std::string str);
std::string		ft_generate_html_dir(std::string dir_path, int final_slash);
std::string		ft_what_location(std::string URI);
std::string 	numberToString(unsigned long n);
std::string 	IsQuery(const std::string &URIraw);
std::string 	str_toupper(std::string &str);
char *const 	*maptoarray(std::map<std::string, std::string> &map);
std::string		ft_generate_error_html(int error, Config config);
std::string		ft_generate_success_delete(Request request);
std::string		ft_generate_success_upload(Request request);

//RequestUtils//
void process_multipart_header(std::string &raw_header, std::string header_name, std::map<std::string,std::string> &headers_map);
std::map<std::string,std::string> process_headers(std::istringstream &file);
std::vector<RequestContent> process_contents(std::istringstream &file, std::map<std::string,std::string> &headers_map);
#endif
