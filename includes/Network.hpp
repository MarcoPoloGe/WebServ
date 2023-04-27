#ifndef NETWORK_HPP
# define NETWORK_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include <stdexcept>
# include "Config.hpp"
#include "Webserv_Includes.hpp"

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 5
#endif

class Network
{

public:
//Constructors
	Network(Config config, int portNo);
	Network(Network const &src);
	~Network(void);

//Accessors

//Operators
	Network	&operator=(Network const &rhs);
	
//Public Functions
	void	run(void);


private:
//Constructors
	Network(void);

//Private Functions
	void	setnonblocking(int sock);
	void	build_select_list(void);
	void	read_socks(void);
	void	handle_new_connection(void);
	void	deal_with_data(int listnum);
	
//Variables
	int					_sock;
	int					_sock2; //multiport
	int					_connectlist[MAX_CLIENTS];
	fd_set				_socks;
	int					_highsock;
	int					_current_sock; //multiport

	Config				_config;
	int					_port;
	struct sockaddr_in	_server_address;
	struct sockaddr_in	_server_address2; //multiport
	int					_reuse_addr;
	struct timeval		_timeout;
	int					_readsocks;

};

#endif
