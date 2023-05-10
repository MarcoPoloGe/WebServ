#include "../../includes/Webserv_Includes.hpp"



Network::Network(Config config, int portNo): _config(config)
{
	//std::cout << "Parametric constructor called\n";

	_port = _config.getPortServer()[portNo];
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
	listen(_sock, BACKLOG);

 	std::cout << "Server ready to listen on port [" << _port << "]\n";

	return ;
}

Network::Network(Network const &src)
{
	//std::cout << "Copy constructor called\n";
	
	*this = src;
	return ;
}

Network	&Network::operator=(Network const &rhs)
{
	//copy here the eventual values like :
	this->_sock = rhs._sock;
	this->_config = rhs._config;
	this->_port = rhs._port;
	this->_server_address = rhs._server_address;
	this->_reuse_addr = rhs._reuse_addr;

	//std::cout << "Copy assignment operator called\n";
	return (*this);
}

Network::~Network(void)
{
	//std::cout << "Destructor called\n";
	return ;
}

int	Network::getSock(void)
{
	return (_sock);
}

void Network::setnonblocking(int sock)
{
	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
		Ft_error	err("fcntl");
	return;
}

Request receive_request(int connection, fd_set &socks)
{
	char				buffer[BUFFER_SIZE + 1];
	int					bytes_read = 1;
	std::string 		request_string;
	Request				request;

	while (bytes_read > 0)
	{
		for (int i = 0; i < BUFFER_SIZE; i++)
			buffer[i] = 0;
		bytes_read = recv(connection, buffer, BUFFER_SIZE - 1, 0);
		if (bytes_read < 0)
		{
			if (FD_ISSET(connection, &socks))
				break ;
			throw std::runtime_error("@fn receive_request(int connection, fd_set &socks)\nrecv connection error");
		}
		request_string += std::string(buffer);
	}
	if (request_string.empty())
		throw std::runtime_error("@fn receive_request(int connection, fd_set &socks)\nrequest is empty");
	request.fill(request_string);
	return (request);
}


int	Network::deal_with_data(int connection, fd_set socks)
{
	std::cout << "⬇️ ⬇️ ⬇️ \n"<< std::endl;//DEBUG

	Request 			request;
	Response			response(_config);
	CGI 				cgi;

	try
	{
		request = receive_request(connection, socks);
	}
	catch(...)
	{
		std::cout << R << "Wrong HTTP REQUEST\n" << RE;
		Response error(404, _config);
		error.send(connection);
		return (1);
	}

	std::string	URI = request.get_URI();
	if (URI == "/")
		URI = "/index.html"; // todo remove and use instead default pages for each folder
	response = _config.IsLocation(URI, request.get_type()); // ./website

//	if (cgi.check(request))
//		cgi.execute(request, response, _config, _port);

/*	///////////////TRICK TEST/////////////////
	
	if ( no_extension() )
	{
		if (request.get_URI() == "/img") //coupe pour path

		std::string location = "/img";
		std::string bool_val = _config.getlocationvalue(location, "autoindex");
		if (bool_val == "true")
		{
			response.set_manual_content_type("text/html");
			response.set_manual_content( ft_generate_html_dir("website/img") );
			response.send(connection);
			return (0);
		}
		else
			path = _config.getlocationvalue(location, "default"); //a check pour plus tard
	}


	///////////////TRICK TEST//////////////// */

	std::cout <<B<< request << "\n" <<RE; //DEBUG

	if(request.get_type() == "GET")
	{


	}
	else if(request.get_type() == "POST")
	{
		std::cout << R << "POST request not implemented yet" << std::endl;
		response.set_path("./website/index.html");
	}
	else if(request.get_type() == "DELETE")
	{
		std::cout << R <<"DELETE request not implemented yet" << std::endl;
		response.set_path("./website/index.html");
	}
/*	// if resquest = cgi -- gooooooooo
	CGI cgi;
	cgi.execute(request, response, _config, _port);*/ //todo later

	std::cout <<Y<< response << "\n" <<RE; //DEBUG
	
	response.send(connection);

	std::cout << "⬆️ ⬆️ ⬆️\n"<< std::endl;//DEBUG
	return (0);
}
