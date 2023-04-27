#include "../../includes/Webserv_Includes.hpp"

LAN::LAN(void)
{
	//nope

	//std::cout << "Default constructor called\n";
	return ;
}

LAN	&LAN::operator=(LAN const &rhs)
{
	//nope

	//std::cout << "Copy assignment operator called\n";
	return (*this);
}

LAN::LAN(LAN const &src)
{
	//nope
	//std::cout << "Copy constructor called\n";
	return ;
}

LAN::~LAN(void)
{
	//std::cout << "Destructor called\n";
	return ;
}

LAN::LAN(std::vector<Config> all_config): _vec_config(all_config)
{
	//std::cout << "Parametric constructor called\n";
	
	_net_amount = 0;
	
	int	i;
	for (std::vector<Config>::iterator it = _vec_config.begin();
			it != _vec_config.end(); it++)
	{
		for (i = 0; i < (*it).getAmountPorts(); i++)
		Network	newNetwork(*it, i);
		_vec_network.push_back(newNetwork);
		_net_amount++;
	}
}
