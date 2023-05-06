#include "../../includes/Webserv_Includes.hpp"

LAN::~LAN(void)
{
	//std::cout << "Destructor called\n";
	return ;
}

Network	LAN::getServer(int servNo)
{
	return (_vec_network[servNo]);
}

LAN::LAN(std::vector<Config> all_config): _vec_config(all_config)
{
	//std::cout << "Parametric constructor called\n";
	
	_net_amount = 0;
	_timeout.tv_sec = 1;
	_timeout.tv_usec = 0;
	
	int	i;
	for (std::vector<Config>::iterator it = _vec_config.begin();
			it != _vec_config.end(); it++)
	{
		for (i = 0; i < (*it).getAmountPortServer(); i++)
		{
			Network	newNetwork(*it, i);

			_vec_network.push_back(newNetwork);
			_net_amount++;
		}
	}
	for (i = 0; i < MAX_CLIENTS; i++)
		_connectlist[i].co = 0;
}

void	LAN::setnonblocking(int sock)
{
	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
		Ft_error Err("fcntl");
	return ;
}

void	LAN::runAll(void)
{
	std::string	dot[12] = {"🕛","🕐","🕑","🕒","🕓","🕔","🕕","🕖","🕗","🕘","🕙","🕚"};
	int			n = 0;
	int			ready_socks;

	while (true)
	{
		build_select_list();
		ready_socks = select(_highsock + 1, &_read_socks, &_wr_socks, &_err_socks, &_timeout);
		if (ready_socks < 0)
			Ft_error	Err("select");
		if (ready_socks == 0)
		{
			std::cout << "\rWaiting on a connection " << dot[n++] << std::flush;
			if (n == 12)
				n = 0;
		}
		else
			read_sockets();
	}
}

void	LAN::build_select_list(void)
{
	int	listnum;
	int	high = -1;

	FD_ZERO(&_read_socks);
	FD_ZERO(&_wr_socks);
	FD_ZERO(&_err_socks);
	for (std::vector<Network>::iterator it = _vec_network.begin(); it != _vec_network.end(); it++)
	{
		FD_SET( (*it).getSock(), &_read_socks );
		if ( (*it).getSock() > high )
			high = (*it).getSock();
	}
	_highsock = high;

	for(listnum = 0; listnum < MAX_CLIENTS; listnum++)
	{
		if (_connectlist[listnum].co != 0)
		{
			FD_SET(_connectlist[listnum].co, &_read_socks);
			if (_connectlist[listnum].co > _highsock)
				_highsock = _connectlist[listnum].co;
		}
	}
}

void	LAN::read_sockets(void)
{
	int	listnum;

	for (std::vector<Network>::iterator it = _vec_network.begin(); it != _vec_network.end(); it++)
	{
		if (FD_ISSET((*it).getSock(), &_read_socks) )
			handle_new_connection();
	}

	for (listnum = 0; listnum < MAX_CLIENTS; listnum++)
	{
		if (FD_ISSET(_connectlist[listnum].co, &_read_socks))
			deal_with_data(listnum);
	}
}

void	LAN::handle_new_connection(void)
{
	int			listnum;
	int			connection;
	sockaddr_in	client_address;
	socklen_t	client_size = sizeof(client_address);

	for (std::vector<Network>::iterator it = _vec_network.begin(); it != _vec_network.end(); it++)
	{
		if ( FD_ISSET((*it).getSock(), &_read_socks) )
		{
			_current_sock = (*it).getSock();
			_current_net = it;
			break ;
		}
	}
	connection = accept(_current_sock, (struct sockaddr*) &client_address, &client_size);
	if (connection < 0)
		Ft_error Err("accept");
	setnonblocking(connection);
	for (listnum = 0; (listnum < MAX_CLIENTS) && (connection != -1); listnum ++)
	{
		if (_connectlist[listnum].co == 0)
		{
			std::cout << "\nConnection detected from " << inet_ntoa(client_address.sin_addr)
				<< ":" << ntohs(client_address.sin_port)
				<< " [Slot no " << listnum << "]\n";
			_connectlist[listnum].co = connection;
			_connectlist[listnum].net = _current_net;
			connection = -1;
		}
	}
	if (connection != -1)
	{
		std::cout << "\nNo more room left for the client\n";
		send(connection,"Sorry, this server is too busy. Try again later!\r\n",50, 0);
		close(connection);
	}
}

void	LAN::deal_with_data(int listnum)
{
	if ((*_connectlist[listnum].net).deal_with_data(_connectlist[listnum].co, _read_socks))
	{
		std::cout << "\nConnection lost with FD = " << _connectlist[listnum].co
				<< " & Slot = " << listnum << std::endl;
	}
	close(_connectlist[listnum].co);
	_connectlist[listnum].co = 0;
	_connectlist[listnum].net = _vec_network.end();
}
