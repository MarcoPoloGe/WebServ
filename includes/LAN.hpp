#ifndef LAN_HPP
# define LAN_HPP

#include "Webserv_Includes.hpp"

struct	s_CoServ {
	int								co;
	std::vector<Network>::iterator	net;
};

class LAN
{

public:
	//Constructor & Destructor//
	
	LAN(std::vector<Config> all_config);
	~LAN(void);

	
	//Public functions//
	
	void	runAll(void);	//todo

	
	//Setters & Getters//
	
	Network	getServer(int servNo);
private:
	//The nope zone //

	LAN(void);						//
	LAN(LAN const &src);			//
	LAN	&operator=(LAN const &rhs);	//nope


	//Private functions//

	void	setnonblocking(int sock);
	void	build_select_list(void);
	void	read_sockets(void);
	void	handle_new_connection(void);
	void	deal_with_data(int socket);


	//Variables//

	std::vector<Config>				_vec_config;
	unsigned int					_conf_amount;
	std::vector<Network>			_vec_network;
	unsigned int					_net_amount;
	int								_current_sock;
	struct timeval					_timeout;
	std::vector<Network>::iterator	_current_net;

	fd_set					_read_socks;
	fd_set					_wr_socks;
	fd_set					_err_socks;
	int						_highsock;
	struct s_CoServ			_connectlist[MAX_CLIENTS];

};

#endif
