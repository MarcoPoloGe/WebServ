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

#include "ClassRequest.hpp"
#include "Ft_error.hpp"
#include "Network.hpp"

#define MAX_CLIENTS 5
#define BUFFER_SIZE 10024

//NoClass Functions prototypes//
std::string	ft_read_file(std::string file_name);

#endif
