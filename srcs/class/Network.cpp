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

	try
	{
		request = receive_request(connection, socks);
	}
	catch(...)
	{
		std::cout << R << "Wrong HTTP REQUEST\n" << RE;
		Response error(404, _config);
		error.send(connection);
		return ;
	}

	_config.getInLocationValue("/", "root");
	std::string	root = _config.getValueTemp();// root = "./website"

	std::string	path;
	std::string	URI;
	std::string extension;
	int	rep_code = 0;
	std::string slash_str;

	std::pair<std::string, std::string> file_type = std::make_pair("type", "imgtype");

	URI = ft_remove_nonprintable( request.get_URI() );
	extension = ft_get_extension(URI);//check size max extension?	//pr marco (extension)

//	std::cout <<B<< "my extension for {" << URI << "} is {" << extension << "}\n"<<RE; //DEBUG
//	path = root + URI; //il faudra enlever le doublon de '/' (par exemple : /images//kittycat.png)

	rep_code = _config.IsLocation(URI, request.get_type());
	if (rep_code != 200) {
//		std::cout << "The type is : {" << request.get_type() << "}\n";
		file_type.first = "text"; file_type.second = "html";
		goto fill_rep;
	}

	if ((path = _config.getPath_of_URI(URI)).empty())
		path = "";

	if (path == "./website/")
		path += "index.html";

	//	if (extension == "")											//pr Lowell (index/autoindex)
//		check_index(_config);										//''

//	if ( CGI_extension(extension, _config) == true )				//pr Lowell (CGI)
//		exec_CGI();													//''

	// normalement obsolete ⬇️  //

//	if (URI.compare("/") == 0)
//	{
//		file_type.first = "html"; file_type.second = "html";
//		//path += root;
//		rep_code = 200;
////		std::cout <<R<< "\n\nURI IS '/' : SO MY PATH IS {" << path << "}\n" << RE;//DEBUG
//	}
//	else
//	{
//		std::ifstream	infile;
//
//		infile.open(path.c_str(), std::ios::in | std::ios::out | std::ios::binary);
//		if (infile.is_open())
//		{
////			std::cout <<R<< "YES I OPENED [" << path << "]\n" << RE;//DEBUG
//			infile.close();
//			rep_code = 200;
//		}
//		else
//		{
////			std::cout <<R<< "NOPE I DID NOT OPEN [" << path << "]\n" << RE;//DEBUG
//			rep_code = 404;
//			file_type.first = "html"; file_type.second = "html";
//			goto fill_rep;
//		}
//
//		std::size_t last_point = path.rfind(".");
//		if (last_point == 0)
//		{
//			std::cout << G << "YA QUUN POINT AU DEBUT\n" << RE;	//DEBUG
//
//			file_type.first = "html"; file_type.second = "html";
//			goto fill_rep;
//		}
//		if (last_point != std::string::npos)
//		{
//			file_type.second = path.substr(last_point + 1);
//			if (file_type.second == "html")
//				file_type.first = "html";
//			else
//				file_type.first = "image";
//		}
//		else
//		{
//			file_type.first = "html";
//			file_type.second = "html";
//		}
//	}

fill_rep:
		/*std::cout << G << "@@@@file type@@@@\n"					//
			<< "file type first = " << file_type.first << "\n"		//
			<< "file type second = "  << file_type.second << "\n"	//
			<< "@@@@@@@@@@@@@@@@@\n" << RE;*/						// DEBUG

		response.set_error_code(rep_code);											//new
//		std::cout <<W<< "@fn Network::deal_with_data(int connection, fd_set socks)" <<RE<< std::endl;
//		std::cout <<R<< "The request body is : ######\n" <<RE<<  request.get_body()
//			<<R<< "\n#####\n"<<RE;

//		std::cout <<R<<  "MY REP CODE IS {" << rep_code << "}\n" << RE;//DEBUG
		if (file_type.second == "html"){
			response.set_content_extension("html");
		}
		else if (file_type.first == "image")
			response.set_content_type(file_type.first + "/" + file_type.second);

		if (response.get_error_code() == 200)
			response.set_content_body(ft_read_file(path));
		else if (response.get_error_code() == 404)
			response.set_content_body(ft_read_file("./website/error_pages/error-404.html"));	//new
		else
			response.set_content_body(ft_read_file("./website/error_pages/error-405.html")); //new

			// normalement obsolete ⬆️ //
		std::cout <<W<< "@fn Network::deal_with_data(int connection, fd_set socks)" <<RE<< std::endl;
		std::cout << G << "my response is \n" << RE << response << std::endl;
		response.send(connection);										//new

	std::cout << "⬆️ ⬆️ ⬆️\n"<< std::endl;//DEBUG
		return (0);
}
