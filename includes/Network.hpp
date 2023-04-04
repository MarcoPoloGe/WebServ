#ifndef NETWORK_HPP
# define NETWORK_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include <stdexcept>
# include "Webserv_Includes.hpp"

class Network
{

public:
//Constructors
	Network(int const port);
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
	int					_connectlist[5];
	fd_set				_socks;
	int					_highsock;
	
	int					_port;
	struct sockaddr_in	_server_address;
	int					_reuse_addr;
	struct timeval		_timeout;
	int					_readsocks;

};

#endif
