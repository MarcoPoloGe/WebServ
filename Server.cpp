#include "Server.hpp"

Server::Server(void)
{
	//PRIVATE!!!
	//std::cout << "Default constructor called\n";
	return ;
}

Server::Server(): _port(int const port)
{
	//std::cout << "Parametric constructor called\n";
	
	_reuse_address = 1;

	_sock = socket(AF_IFNET, SOCK_STREAM, 0);
	if (sock < 0)
		Ft_error	err(NULL);
	
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &_reuse_addr,
		sizeof(_reuse_addr));

	setnonblocking(sock);

	memset((char *) &_server_address, 0, sizeof(_server_address));
	_server_address.sin_family = AF_INET;
	_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_server_address.sin_port = htons(_port);

	if (bind(_sock, (struct sockaddr *) &_server_address, sizeof(_server_address)) < 0 )
	{
		close(_sock);
		Ft_error	err(NULL);
	}

	listen(_sock, MAX_CLIENT);
	
	_highsock = _sock;
	
	memset((char *) &_connectlist, 0, sizeof(_connectlist));

	std::cout << "Server ready to listen on port [" << _port << "]\n";

	return ;
}

Server::Server(Server const &src)
{
	*this = src;
	//std::cout << "Copy constructor called\n";
	return ;
}

Server::~Server(void)
{
	//std::cout << "Destructor called\n";
	return ;
}

void Server::setnonblocking(int sock)
{
	int opts;

	opts = fcntl(sock,F_GETFL);
	if (opts < 0)
		Ft_error	err(NULL);

	opts = (opts | O_NONBLOCK);

	if (fcntl(sock,F_SETFL,opts) < 0)
		Ft_error	err(NULL);

	return;
}

void	Server::run(void)
{
	//AAAAAAAAAAAAAAAAAAAAAAAAAAAA;  SELON MAIN DE SELECT2.CPP
}

Server	&Server::operator=(Server const &rhs)
{
	//copy here the eventual values like :
	this->_sock = rhs._sock;
	this->_connectlist = rhs._connectlist;
	this->_socks = rhs._socks;
	this->_hisghsock = rhs._highsock;

	this->_port = rhs._port;
	this->_server_address = rhs._server_address;
	this->_reuse_addr = rhs._reuse_addr;
	this->_timeout = rhs._timeout;
	this->_readsocks = rhs._readsocks;

	//std::cout << "Copy assignment operator called\n";
	return (*this);
}
