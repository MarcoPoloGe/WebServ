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

#include "ClassResponse.hpp"
#include "ClassRequest.hpp"
#include "Ft_error.hpp"
#include "Network.hpp"


#define MAX_CLIENTS 5
#define BUFFER_SIZE 10024
#define HTTP_VERSION "HTTP/1.1"

//COLOR OUTPUT//
#define	B		"\033[1m\033[30m" 		// Bold Black
#define R		"\033[1m\033[31m"		// Bold Red
#define G		"\033[1m\033[32m"		// Bold Green
#define Y		"\033[1m\033[33m"		// Bold Yellow
#define W		"\033[1m\033[37m"		// Bold White
#define	RE   	"\033[0m"				// Reset

//NoClass Functions prototypes//
std::string	ft_read_file(std::string file_name);

#endif
