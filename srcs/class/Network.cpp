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

bool no_final_slash(std::string URI)
{
	if (URI[URI.length() - 1] != '/')
		return (true);
	return (false);
}

int	add_slash(std::string URI, int connection, int port)
{
	std::string	URL = "http://localhost:" + std::to_string(port) + URI + "/";
   	std::string response =
		"HTTP/1.1 302 Found\r\n"
        "Location: " + URL + "\r\n"
        "\r\n";

	send(connection, response.c_str(), response.length(), 0);
	return (0);
}

Request Network::receive_request(int connection, fd_set &socks)
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
			throw std::runtime_error("@fn Network::receive_request(int connection, fd_set &socks)\nrecv connection error");
		}
		request_string += std::string(buffer);
	}
	if (request_string.empty())
		throw std::runtime_error("@fn Network::receive_request(int connection, fd_set &socks)\nrequest is empty");
	request.fill(request_string);
	return (request);
}

bool Network::CatchRequest(Request &request, int connection, fd_set socks)
{
	try
	{
		request = this->receive_request(connection, socks);
	}
	catch(...)
	{
		std::cout << R << "Wrong HTTP REQUEST\n" << RE;
		Response error(404, _config);
		error.send(connection, socks);
		return (false);
	}
	return (true);
}

int Network::SendResponse(int errorCode, Response &response, int connection)
{
	response.set_error_code(errorCode);
	response.send(connection);
	return (0);
}

int Network::SendResponseDefault(int errorCode, Response &response, int connection, std::string path, std::string URIraw)
{
	int final_slash = 0;
	if (URIraw[URIraw.length() - 1] == '/')
		final_slash = 1;

	response.set_error_code(errorCode);
	response.set_manual_content_type("text/html");

	response.set_manual_content( ft_generate_html_dir(path, final_slash) );
	response.send(connection);
	return (0);
}

int Network::SendCGIResponse(int errorCode, Response &response, Request &request, int connection, const std::string& path)
{
	CGI cgi;
	std::cout <<G<< "CGI Goooooooo" <<RE<< std::endl;
	response.set_error_code(errorCode);

	response.set_manual_content_type("text/html");
	response.set_manual_content(cgi.execute(request, _config, _port, path));
	response.send(connection);
	return (0);
}

int Network::Redirection(int connection, std::string redirect_URL)
{
	std::string	code = redirect_URL.substr(0, 3);
	std::string URL = redirect_URL.substr(4);
	std::string description = _config.getDefaultErrorDescription( std::atoi(code.c_str()) );
   	std::string response =
		"HTTP/1.1 " + code + " " + description  + "\r\n"
        "Location: " + URL + "\r\n"
        "\r\n";

	send(connection, response.c_str(), response.length(), 0);
	return (0);
}

int	Network::RequestToResponse(int connection, fd_set socks)
{
	std::cout << "⬇️ ⬇️ ⬇️ \n"<< std::endl;//DEBUG

	Request 			request;
	Response			response(_config);
	CGI 				cgi;

	//Catch Request, send Response error(404, _config) if Wrong HTTP Request
	if (!this->CatchRequest(request, connection, socks))
		return(SendResponse(404, response, connection));

	std::string	URIraw = request.get_URI();
//	std::cout <<Y<< "URIraw: {" << URIraw << "}" <<RE<< std::endl;

	std::cout <<R<< "REQUEST TYPE IIIIIIS : " << request.get_type() << "\n"<<RE;//DEBUG

	std::string PathToFile;

	if (URIraw.find("//") != std::string::npos)
		return(SendResponse(404, response, connection));
	std::cout << B << "######## URI IS : " << URIraw << " #########\n"<<RE;

	// get Folder from URIraw ("/img/kittycat.jpg" = ret(img) || "/index.html" = "/" || "/" = "/")
	std::string Folder = _config.getFolderLocationFromURI(URIraw);
//	std::string location = ft_what_location(URIraw);
	std::cout <<Y<< "Folder : " << Folder <<RE<< std::endl;
//	std::cout <<B<< "Loc what: " << location <<RE<< std::endl;

	// test if Folder is in locations ; return *getSingleMapLocation
	std::map<std::string, std::string> *singleLocationContent;
	if ((singleLocationContent = _config.getSingleMapLocation(Folder)) == nullptr){
		return (SendResponse(404, response, connection));
	}


	//////////////////////////////////////////////////////////////////////////////
	/////////////////////A IMPLEMENTER POUR LA REDI //////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	
	//if (Folder == "/Ronoo" || Folder == "/Samsaa" || Folder == "/Marcoo")

	std::string redi = _config.getReturn(*singleLocationContent); //A VERIFIER
	if (!redi.empty())
	{
		//std::string redi = "301,https://github.com/8L312";

		return ( Redirection(connection, redi) );
	}
	

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	_config.setPathToFile(URIraw, singleLocationContent);

	PathToFile = _config.getPath();
//	std::cout <<B<< "PathToFile : " << PathToFile <<RE<< std::endl;

	// test if IsCGI ; send reponse and return 0
	if (ft_get_extension(URIraw) == "py" ||( URIraw.find(".py?") != std::string::npos))
		return (SendCGIResponse(200, response, request, connection, PathToFile));


	// test if the file exist in location ; return path to the file or path to the folder in location
	PathToFile = _config.isPathToFile(PathToFile);
	if (PathToFile.empty())
		return (SendResponse(404, response, connection)); // file doesn't exist in folder from locations


//	std::cout <<B<< PathToFile <<RE<< std::endl;
	if ( ft_get_extension(PathToFile) == "" )
	{
		std::string autoindexValue = _config.getAutoindex(*singleLocationContent);
		std::string location = _config.getLocation(*singleLocationContent);
//		std::cout <<R<< "the location is :{" << location << "}\n" <<RE;//DEBUG

		if (autoindexValue == "true")
		{
//			std::cout <<Y<< "###AUTOINDEX IS TRUE###\n" <<RE;//DEBUG
			if ( no_final_slash(URIraw) )
				return ( add_slash(URIraw, connection, _port) );
			SendResponseDefault(200, response, connection, PathToFile, URIraw);

			return (0);
		}
		else
		{
//			std::cout <<Y<< "###AUTOINDEX IS FALSE###\n" <<RE;//DEBUG

			std::cout <<B<< PathToFile <<RE<< std::endl;
			PathToFile = PathToFile + _config.getDefault(*singleLocationContent);

//			response.set_error_code(errorCode);
//			response.set_manual_content_type("text/html");
//			SendResponseDefault(200, response, connection, (PathToFile + "index.html"));
		}
	}

	// test if Method is allowed in Location ; return bool
	if (!_config.IsMethodAllowed(request.get_type(), *(singleLocationContent)))
		return (SendResponse(404, response, connection));



	if(request.get_type() == "GET")
	{
		response.set_path(PathToFile);
		response.send(connection, socks);
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

	std::cout << "⬆️ ⬆️ ⬆️\n"<< std::endl;//DEBUG
	return (0);
}
