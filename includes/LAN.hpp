#ifndef LAN_HPP
# define LAN_HPP

#include "Webserv_Includes.hpp"

class LAN
{

public:
	LAN(std::vector<Config> all_config);
	~LAN(void);

	void	runAll(void);			//todo

private:
	LAN(void);						//
	LAN(LAN const &src);			//
	LAN	&operator=(LAN const &rhs);	//nope


	std::vector<Config>		_vec_config;
	unsigned int			_conf_amount;
	std::vector<Network>	_vec_network;
	unsigned int			_net_amount;
	int						_current_sock;
};

#endif
