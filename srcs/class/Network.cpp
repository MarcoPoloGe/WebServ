#include "../../includes/Webserv_Includes.hpp"



Network::Network(Config config, int portNo): _config(config), _default_conf(config)
{
	//std::cout << "Parametric constructor called\n";

	_max_body_size = _config.getBodyLength();
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

	_names.insert( std::pair<std::string, Config>(config.getNameServer(), config) );

	if (bind(_sock, (struct sockaddr *) &_server_address, sizeof(_server_address)) < 0 )
	{
		close(_sock);
		Ft_error	err("bind");
	}
	listen(_sock, BACKLOG);

 	std::cout <<W<< "Server <" << config.getNameServer() << "> ready to listen on port ["
		<< _port << "]\n\n" <<RE;

	return ;
}

void	Network::addName(Config config)
{
	if ( _names.count( config.getNameServer() ) == 0 )
		_names.insert( std::pair<std::string, Config>(config.getNameServer(), config) );
	else
		throw std::invalid_argument("Multiples serv on one port with same name");
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
	this->_max_body_size = rhs._max_body_size;
	this->_sock = rhs._sock;
	this->_config = rhs._config;
	this->_default_conf = rhs._default_conf;
	this->_port = rhs._port;
	this->_server_address = rhs._server_address;
	this->_reuse_addr = rhs._reuse_addr;
	this->_names = rhs._names;
	

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

int	Network::getPort(void)
{
	return (_port);
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

int	add_slash(std::string URI, int connection, Request request)
{
	std::string	URL = "http://" + request.get_header("Host") + URI + "/";
   	std::string response =
		"HTTP/1.1 302 Found\r\n"
        "Location: " + URL + "\r\n"
        "\r\n";

	send(connection, response.c_str(), response.length(), 0);
	return (1);
}

int	go_default(std::string URI, int connection, Request request)
{
	std::string	URL = "http://" + request.get_header("Host") + URI;
   	std::string response =
		"HTTP/1.1 302 Found\r\n"
        "Location: " + URL + "\r\n"
        "\r\n";

	int ret = send(connection, response.c_str(), response.length(), 0);
	if ( ret < 0)
		std::cerr <<R<< "Message not send to socket\n";
	return (1);
}

Request Network::receive_request(int connection, fd_set &socks)
{
	char 				*buffer = new char[sizeof(char) *_max_body_size + 1];
	int					bytes_read = 1;
	long int			total_bytes = 0;
	std::string 		request_string;
	Request				request;

	while (bytes_read > 0)
	{
		bzero(buffer, _max_body_size + 1);
		bytes_read = recv(connection, buffer, _max_body_size - 1, 0);
		if (bytes_read < 0)
		{
			if (FD_ISSET(connection, &socks))
				break ;
			delete[] buffer;
			throw std::runtime_error("@fn Network::receive_request(int connection, fd_set &socks)\nrecv connection error");
		}
		else if (bytes_read == 0)
			break ; 
		total_bytes += bytes_read;
		request_string += std::string(buffer, bytes_read);
	}
	if (total_bytes > _max_body_size)
		std::cout <<R<< "MAX BODY SIZE OF REQUEST EXCEEDED : THIS INCIDENT WILL BE REPORTED\n" <<
			"(just kidding, this is a local webserver but it has to be said)\n" <<RE;

	if ( request_string.substr(0, 6).find("POST /") != std::string::npos)
		request.upload_file(request_string);

	delete[] buffer;
	if (request_string.empty())
		throw std::runtime_error("@fn Network::receive_request(int connection, fd_set &socks)\nrequest is empty");
	request.fill(request_string);
	return (request);
}

bool Network::CatchRequest(Request &request, int connection, fd_set socks)
{
	_config = _default_conf;	//default config is always reset on config in 
								//case we had to change it for a special request
	try
	{
		request = this->receive_request(connection, socks);
	}
	catch(std::invalid_argument &e)
	{
		std::string err_string = e.what();
		int code = std::atoi( err_string.substr(1, 3).c_str() );
		Response response(_config);
		SendResponse(code, response, connection);
		return (false);
	}
	catch ( std::runtime_error &e)
	{
		return (false);
	}
	check_host(request);
	return (true);
}

void	Network::check_host(Request request)
{
	std::string hostname = request.get_header("Host");
	std::size_t pos = hostname.rfind(":");

	if (pos == std::string::npos)
		return ;
	hostname.erase(pos);

	if ( _names.find(hostname) != _names.end() )
		_config = _names.find(hostname)->second;
}

int Network::SendResponse(int errorCode, Response &response, int connection)
{
	response.set_error_code(errorCode);
	response.send(connection);
	return (1);
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
	return (1);
}

int Network::SendCGIResponse(int errorCode, Response &response, Request &request, int connection, const std::string& path)
{
	CGI cgi;
	response.set_error_code(errorCode);

	response.set_manual_content_type("text/html");
	response.set_manual_content(cgi.execute(request, _config, _port, path));
	response.send(connection);
	return (1);
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

	ssize_t ret = send(connection, response.c_str(), response.length(), 0);
	if ( ret < 0)
		std::cerr <<R<< "Message not send to socket\n";
	return (1);
}

int	Network::RequestToResponse(int connection, fd_set socks)
{
//	std::cout << "⬇️ ⬇️ ⬇️ \n"<< std::endl;//DEBUG

	Request 			request;
	Response			response(_config);
	CGI 				cgi;

	//Catch Request, send Response error(404, _config) if Wrong HTTP Request //
	if ( this->CatchRequest(request, connection, socks) == false)
		return (1);

	std::string	URIraw = request.get_URI();
	std::string PathToFile;

	if (URIraw.find("//") != std::string::npos)
		return(SendResponse(404, response, connection));
	std::string Folder = _config.getFolderLocationFromURI(URIraw);
	
	// test if Folder is in locations ; return *getSingleMapLocation
	std::map<std::string, std::string> *singleLocationContent;
	if ((singleLocationContent = _config.getSingleMapLocation(Folder)) == 0 ){
		return (SendResponse(404, response, connection));
	}

	std::string redi = _config.getReturn(*singleLocationContent);
	if (!redi.empty())
		return ( Redirection(connection, redi) );

	if(request.get_type() == "GET")
	{
		_config.setPathToFile(URIraw, singleLocationContent);

		PathToFile = _config.getPath();

		// test if IsCGI ; send reponse and return 0

		if (ft_get_extension(URIraw) == "py" ||( URIraw.find(".py?") != std::string::npos))
		{
			try {
			return (SendCGIResponse(200, response, request, connection, PathToFile));
			}
			catch(...)
			{
				std::invalid_argument("Python script not found");
			}
		}

		// test if the file exist in location
		// return path to the file or path to the folder in location
		PathToFile = _config.isPathToFile(PathToFile);
		if (PathToFile.empty())
			return (SendResponse(404, response, connection)); // file doesn't exist in folder
															  // from locations

		if ( ft_get_extension(PathToFile) == "")
		{
			// If teh ressource requested is actually a file but with no extension //
			// so it could be taken as a folder //

			bool doss;
			DIR	*dir = opendir( PathToFile.c_str() );
			if (dir == NULL)
				doss = false;
			else
				doss = true;
			if (isFile(PathToFile) && doss == false)
			{
				response.set_manual_content_type("application/octet-stream");
				response.set_manual_content( ft_read_file(PathToFile) );
				response.send(connection);
				return (1);
			}


			// Here is the handle of autoindex //
			std::string autoindexValue = _config.getAutoindex(*singleLocationContent);
			std::string location = _config.getLocation(*singleLocationContent);

			if (autoindexValue == "true")
			{
				if ( no_final_slash(URIraw) )
					return ( add_slash(URIraw, connection, request) );
				SendResponseDefault(200, response, connection, PathToFile, URIraw);

				return (0);
			}
			else
			{
				std::map<std::string, std::string> *rootmap
					=_config.getSingleMapLocation("/");

				// Even if autoindex is false, we need in this case to answer just the dir//
				if ( (request.get_header("Referer").find(URIraw) != std::string::npos
					&& URIraw != "/")
					&& request.get_header("User-Agent").find("curl") == std::string::npos)
				{
					if ( no_final_slash(URIraw) )
					return ( add_slash(URIraw, connection, request) );
					SendResponseDefault(200, response, connection, PathToFile, URIraw);
					return (1);
				}
				else // autoindex = false : we add the default path to URI //
				{
					PathToFile = PathToFile
						+ _config.getDefault(*singleLocationContent);

					if (request.get_header("User-Agent").find("curl") != std::string::npos
							|| request.get_header("User-Agent") == "")
					{
						response.set_path(PathToFile);
						response.send(connection);
						return (1);
					}
					PathToFile = PathToFile.substr( _config.getRoot(*rootmap).size() - 1 );
					return (go_default(PathToFile, connection, request));
				}
			}
		}
		response.set_path(PathToFile);
		response.send(connection, socks);
	}
	else if(request.get_type() == "POST")
	{
		// Mon trick qui est dans  receive request devrait etre ici //

		if(request.get_header("Content-Type") == "multipart/form-data")
		{
			if(request.get_content_header("Content-Disposition-name") == "file")
				return (upload_file(request, response, connection));
		}
		else
			SendResponse(422,response,connection);
	}
	else if(request.get_type() == "DELETE")
	{
		if (!_config.IsMethodAllowed(request.get_type(), *(singleLocationContent)))
			return (SendResponse(405, response, connection));
		return ( delete_file(request, response, connection) );
	}

	// test if Method is allowed in Location ; return bool
	if (!_config.IsMethodAllowed(request.get_type(), *(singleLocationContent)))
		return (SendResponse(405, response, connection));

//	std::cout << "⬆️ ⬆️ ⬆️\n"<< std::endl;//DEBUG
	return (0);
}

int	Network::delete_file(Request request, Response response, int connection)
{
	std::string	file_path = request.get_URI();
	std::map<std::string, std::string> *ptr = _config.getSingleMapLocation("/");
	std::string root = _config.getRoot(*ptr);

	root.erase(root.end() - 1);
	file_path = root + file_path;
	bool		file_delete = std::remove( file_path.c_str() ) == 0;

	if ( file_delete )
	{
		response.set_manual_code(204);					//J'ai mal a mon ptit coeur
		response.set_manual_content_type("text/html");
		response.set_manual_content( ft_generate_success_delete(request) );
		response.send(connection);
		return (1);
	}
	else
		return (SendResponse(500, response, connection));
}


int	Network::upload_file(Request request, Response response, int connection)
{
	std::string	file_path;
	file_path = _config.getUploadFolder();
	file_path += request.get_filename_post();

	// Create and open a text file
	std::ofstream file(file_path.c_str());
	if (!file)
		return (SendResponse(500, response, connection));

	// Write to the file
	file << request.get_file_post();

	// Close the file
	file.close();

	response.set_manual_code(201);
	response.set_manual_content_type("text/html");
	response.set_manual_content( ft_generate_success_upload(request) );
	response.send(connection);
	return (1);
}
