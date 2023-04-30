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
#include "Config.hpp"
#include "Network.hpp"
#include "CGI.hpp"


#define MAX_CLIENTS 5
#define BUFFER_SIZE 1
#define HTTP_VERSION "HTTP/1.1"

// Bytes for endian conversion
# define BYTE_0 0xff000000
# define BYTE_1 0x00ff0000
# define BYTE_2 0x0000ff00
# define BYTE_3 0x000000ff

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
unsigned short	ft_bswap16(unsigned short x);
unsigned short	ft_htons(unsigned short s);
unsigned int	ft_bswap32(unsigned int x);
unsigned int	ft_htonl(unsigned int l);

#endif
