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

int	Network::deal_with_data(int connection, fd_set socks)
{
	std::cout << "⬇️ ⬇️ ⬇️ \n"<< std::endl;//DEBUG

	char				buffer[BUFFER_SIZE];
	Request 			request;
	Response			response(_config);
	int					bytes_read = 1;
	std::string 		request_string;

	while (bytes_read > 0)
	{
		for (int i = 0; i < BUFFER_SIZE; i++)
			buffer[i] = 0;
		bytes_read = recv(connection, buffer, BUFFER_SIZE, 0);
		if (bytes_read < 0)
		{
			if (FD_ISSET(connection, &socks))
				break ;
			return (1);
		}
		request_string += buffer;
	}

	std::cout <<G<< "My request is :\n" << Y << request_string << std::endl << RE;//DEBUG
	
	std::string	root = "./website";//a changer 
//	std::string root;	//remplir plus tard selon la location
	std::string	path;		
	std::string	URI;
	std::string extension;
	int	rep_code = 0;

	std::pair<std::string, std::string> file_type = std::make_pair("type", "imgtype");

	if (request.fill(request_string) == false)
	{
		std::cout << R << "Wrong HTTP REQUEST\n" << RE;
		rep_code = 404;
		file_type.first = "text"; file_type.second = "html";
//		extension = ".html"; 										//pr marco (extension)
		goto fill_rep;
	}

	URI = request.get_URI();
	extension = ft_get_extension(URI);								//pr marco (extension)

	std::cout <<B<< "my extension for {" << URI << "} is {" << extension << "}\n"<<RE; //DEBUG
	
//	root = compare_to_locs_config(_config, URI);					//pr marco et Lowell(locations)

	path = root + URI; //il faudra enlever le doublon de '/' (par exemple : /images//kittycat.png)

//	if (extension == "")											//pr Lowell (index/autoindex)
//		check_index(_config);										//''

	std::cout << "***Try to access : {" << path << "}***\n";//DEBUG

//	if ( verify_method(request.get_type(), _config()) == false )	//pr marco et Lowell (method)
//		error_wrong_method();										//''

//	if ( CGI_extension(extension, _config) == true )				//pr Lowell (CGI)
//		exec_CGI();													//''

	// normalement obsolete ⬇️  //
	if (URI == "/")
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
			response.set_content_body(ft_read_file("./website/error_pages/error-404.html"));	//new

	// normalement obsolete ⬆️ //

		std::cout << G << "my response is \n" << W << response << std::endl << RE;
		response.send(connection);										//new

	std::cout << "⬆️ ⬆️ ⬆️\n"<< std::endl;//DEBUG
		return (0);
}
