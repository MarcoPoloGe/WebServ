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

void	setnonblocking(int sock);
void	handle_request(int client_socket);

#endif
