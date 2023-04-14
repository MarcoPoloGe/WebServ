#include "../../includes/Webserv_Includes.hpp"

Network::Network(void)
{
	//PRIVATE!!!
	//std::cout << "Default constructor called\n";
	return ;
}

Network::Network(int const port, Config config): _config(config), _port(port)
{
	//std::cout << "Parametric constructor called\n";

	//std::cout << B << "AF_INET is equal to [" << AF_INET << "]\n" << RE; //to check macros

	_req_handled = 0; // DEBUG

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
//	_server_address.sin_addr.s_addr = inet_addr("127.0.0.5");
	_server_address.sin_port = htons(_port);

	if (bind(_sock, (struct sockaddr *) &_server_address, sizeof(_server_address)) < 0 )
	{
		close(_sock);
		Ft_error	err("bind");
	}

	/*struct sockaddr_in remoteaddr;
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	remoteaddr.sin_port = htons(8081);
	int ret = connect(_sock, (struct sockaddr *)&remoteaddr, sizeof(remoteaddr));
	if (ret < 0)
	{
		std::cout << R << "Remoteaddr cannot connect to sock\n" << RE;
	}*/

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
	char				buffer[BUFFER_SIZE];
	Request 			request;
	Response			response;
	int					bytes_read;
	std::string 		request_string;
	int					recv_it = 0;	// TEST MULTI RECV
	// need to boucler les recv pr si buffer size trop petit ? //

	_total_bytes_read = 0;
	_req_handled++;
	std::cout << Y << "\nvvv CECI EST LA " << _req_handled << "eme REQUEST HANDLED vvv\n" << RE;

//recv:	// TEST MULTI RECV
	for (int i = 0; i < BUFFER_SIZE; i++)
		buffer[i] = 0;

	bytes_read = recv(_connectlist[listnum], buffer, BUFFER_SIZE - 1, 0);

	_total_bytes_read += bytes_read;
	recv_it++;

	//std::cout << G << "It's the " << recv_it << "eme try of recv\n" << RE; // TEST MULTI RECV

	if (bytes_read < 0)
	{
		std::cout << "\nConnection lost with FD = " << _connectlist[listnum]
			<< " & Slot = " << listnum << std::endl;
		close(_connectlist[listnum]);
		_connectlist[listnum] = 0;
	}
	else
	{
		request_string += buffer;

		if (buffer[bytes_read - 1] == '\0')
			std::cout << B << "YA EOF A buffer[bytes_read] !!!\n" << RE;

	//	if (request_string.find("\r\n\r\n") == std::string::npos)
	//		goto recv; // TEST MUTLI RECV

		std::string	root = "./website";  //a get dans le vrai config file
		std::string	uri;
		std::string	path;
		int	rep_code = 0;
		std::pair<std::string, std::string> file_type = std::make_pair("type", "imgtype");

		std::cout << R << "@@@RAW REQUEST IS :@@@\n" << RE << request_string << std::endl
			<< R << "@@@@@ =" << _total_bytes_read << " bytes read and "
			<< recv_it << " recv @@@@@\n" << RE;								//DEBUG


		if (request.fill(request_string) == false)
		{
			std::cout << "Wrong HTTP REQUEST\n";
			rep_code = 404;
			file_type.first = "html"; file_type.second = "html";
			goto fill_rep;
		}

//		std::cout << "Parsed request:\n" << request << std::endl; //DEBUG

		uri = request.get_URI();
		path = root + uri;

		std::cout << "***Try to access : {" << path << "}***\n";		//DEBUG

		if (uri == "/")
		{
			file_type.first = "html"; file_type.second = "html";
			path += "index.html";
			rep_code = 200;
		}
		else
		{
			std::ifstream	infile;

			infile.open(path.c_str(), std::ios::in);
			if (infile.is_open())
			{
				infile.close();
				rep_code = 200;
			}
			else
			{
				rep_code = 404;
				file_type.first = "html"; file_type.second = "html";
				goto fill_rep;
			}

			std::size_t last_point = path.rfind(".");
			if (last_point == 0)
			{
				std::cout << G << "YA QUUN POINT AU DEBUT\n" << RE;	//DEBUG

				file_type.first = "html"; file_type.second = "html";
				goto fill_rep;
			}
			if (last_point != std::string::npos)
			{
				file_type.second = path.substr(last_point + 1);
				if (file_type.second == "html")
					file_type.first = "html";
				else
					file_type.first = "image";
			}
			else
			{
				file_type.first = "html";
				file_type.second = "html";
			}
		}

fill_rep:
		/*std::cout << G << "@@@@file type@@@@\n"					//
			<< "file type first = " << file_type.first << "\n"		//
			<< "file type second = "  << file_type.second << "\n"	//
			<< "@@@@@@@@@@@@@@@@@\n" << RE;*/						// DEBUG

		response.set_error_code(rep_code);											//new


		if (file_type.first == "html")												//a changer
			response.set_content_type("text/html");									//quand on
		else if (file_type.first == "image")										//aura les
			response.set_content_type(file_type.first + "/" + file_type.second);	//mimes_type

		if (response.get_error_code() == 200)										//
			response.set_content_body(ft_read_file(path));							//
		else																		//
			response.set_content_body(ft_read_file("./website/error-404.html"));	//new

		response.send(_connectlist[listnum]);										//new

		close(_connectlist[listnum]);
		_connectlist[listnum] = 0;

		std::cout << Y << "^^^ FIN DE LA " << _req_handled << "eme REQUEST ^^^\n" << RE;//DEBUG
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
	std::string	dot[12] = {"🕛","🕐","🕑","🕒","🕓","🕔","🕕","🕖","🕗","🕘","🕙","🕚"};
	int			n = 0;

	while (true)
	{
		build_select_list();
		_timeout.tv_sec = 0;
		_timeout.tv_usec = 500000;

		_readsocks = select(_highsock + 1, &_socks, (fd_set *)0, (fd_set *)0, &_timeout);
		if (_readsocks < 0)
			Ft_error	Err("select");
		if (_readsocks == 0)
		{
			std::cout << "\rWaiting on a connection " << dot[n++] << std::flush;
			if (n == 12)
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
