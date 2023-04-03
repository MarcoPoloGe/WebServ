#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include <stdexcept>
#include "../includes/Webserv_Includes.hpp"

class Server
{

public:
//Constructors
	Server(int const port);
	Server(Server const &src);
	~Server(void);

//Accessors

//Operators
	Server	&operator=(Server const &rhs);

private:
//Constructors
	Server(void);

//Functions
	run(void);
	setnonblocking(int sock);
	
//Variables
	int					_sock;
	int					_connectlist[5];
	fd_set				_socks;
	int					_highsock;
	
	int					_port;
	struct sockaddr_in	_server_adress;
	int					_reuse_addr;
	struct timeval		_timeout;
	int					_readsocks;

};

#endif
