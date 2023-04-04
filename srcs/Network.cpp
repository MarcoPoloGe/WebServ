#include "../includes/Network.hpp"

Network::Network(void)
{
	//PRIVATE!!!
	//std::cout << "Default constructor called\n";
	return ;
}

Network::Network(int const port): _port(port)
{
	//std::cout << "Parametric constructor called\n";
	
	_reuse_addr = 1;

	_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock < 0)
		Ft_error	err("socket");
	
	setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &_reuse_addr,
		sizeof(_reuse_addr));

	setnonblocking(_sock);

	memset((char *) &_server_address, 0, sizeof(_server_address));
	_server_address.sin_family = AF_INET;
	_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_server_address.sin_port = htons(_port);

	if (bind(_sock, (struct sockaddr *) &_server_address, sizeof(_server_address)) < 0 )
	{
		close(_sock);
		Ft_error	err("bind");
	}

	listen(_sock, MAX_CLIENTS);
	
	_highsock = _sock;
	
	memset((char *) &_connectlist, 0, sizeof(_connectlist));

	std::cout << "Server ready to listen on port [" << _port << "]\n";

	return ;
}

Network::Network(Network const &src)
{
	*this = src;
	//std::cout << "Copy constructor called\n";
	return ;
}

Network::~Network(void)
{
	//std::cout << "Destructor called\n";
	return ;
}

void Network::setnonblocking(int sock)
{
	int opts;

	opts = fcntl(sock,F_GETFL);
	if (opts < 0)
		Ft_error	err("fcntl");

	opts = (opts | O_NONBLOCK);

	if (fcntl(sock,F_SETFL,opts) < 0)
		Ft_error	err("fcntl");

	return;
}

void	Network::build_select_list(void)
{
	int listnum;

	FD_ZERO(&_socks);
	FD_SET(_sock, &_socks);
	for (listnum = 0; listnum < 5; listnum++)
	{
		if (_connectlist[listnum] != 0)
		{
			FD_SET(_connectlist[listnum], &_socks);
			if (_connectlist[listnum] > _highsock)
				_highsock = _connectlist[listnum];
		}
	}
}

void	Network::handle_new_connection(void)
{
	int listnum;
	int connection;
	sockaddr_in client_address;
	socklen_t client_size = sizeof(client_address);

	connection = accept(_sock, (struct sockaddr*) &client_address, &client_size);
	if (connection < 0)
		Ft_error	err("accept");
	setnonblocking(connection);
	for (listnum = 0; (listnum < MAX_CLIENTS) && (connection != -1); listnum ++)
	{
		if (_connectlist[listnum] == 0)
		{
			std::cout << "\nConnection detected from " << inet_ntoa(client_address.sin_addr)
				<< ":" << ntohs(client_address.sin_port)
				<< " [Slot no " << listnum << "]\n";
			_connectlist[listnum] = connection;
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

void	Network::deal_with_data(int listnum)
{
	char	buffer[BUFFER_SIZE];
	char*	cur_char;

	if (recv(_connectlist[listnum], buffer, BUFFER_SIZE, 0) < 0)
	{
		std::cout << "\nConnection lost with FD = " << _connectlist[listnum]
			<< " & Slot = " << listnum << std::endl;
		close(_connectlist[listnum]);
		_connectlist[listnum] = 0;
	}
	else
	{
		std::cout << "\n#####We recieved : #####\n" << buffer
			<< std::endl << "########################\n";

		cur_char = buffer;
		while (cur_char[0] != 0)
		{
			cur_char[0] = toupper(cur_char[0]);
			cur_char++;
		}
		send(_connectlist[listnum],buffer, strlen(buffer), 0);
		send(_connectlist[listnum],"\n", 1, 0);

		std::cout << "\n#####We responded : #####\n" << buffer
			<< std::endl << "#########################\n";
		close(_connectlist[listnum]);
		_connectlist[listnum] = 0;
	}
}

void	Network::read_socks(void)
{
	int	listnum;

	if (FD_ISSET(_sock, &_socks))
		handle_new_connection();
	for (listnum = 0; listnum < MAX_CLIENTS; listnum++)
	{
		if (FD_ISSET(_connectlist[listnum], &_socks))
			deal_with_data(listnum);
	}
}

void	Network::run(void)
{
	std::string	dot[3] = {".  ", ".. ", "..."};
	int			n = 0;

	while (true)
	{
		build_select_list();
		_timeout.tv_sec = 1;
		_timeout.tv_usec = 0;

		_readsocks = select(_highsock + 1, &_socks, (fd_set *)0, (fd_set *)0, &_timeout);
		if (_readsocks < 0)
			Ft_error	Err("select");
		if (_readsocks == 0)
		{
			std::cout << "\rWaiting on a connection" << dot[n++] << std::flush;
			if (n == 3)
				n = 0;
		}
		else
			read_socks();
	}
}

Network	&Network::operator=(Network const &rhs)
{
	//copy here the eventual values like :
	this->_sock = rhs._sock;
	for (int i = 0; i < 5; i++)
		this->_connectlist[i] = rhs._connectlist[i];
	this->_socks = rhs._socks;
	this->_highsock = rhs._highsock;

	this->_port = rhs._port;
	this->_server_address = rhs._server_address;
	this->_reuse_addr = rhs._reuse_addr;
	this->_timeout = rhs._timeout;
	this->_readsocks = rhs._readsocks;

	//std::cout << "Copy assignment operator called\n";
	return (*this);
}
