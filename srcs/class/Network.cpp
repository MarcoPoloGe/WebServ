#include "../../includes/Webserv_Includes.hpp"



Network::Network(Config config, int portNo): _config(config)
{
	//std::cout << "Parametric constructor called\n";

	_max_body_size = _config.getBodyLength();//TO DO IN CONFIG
	//_max_body_size = 4096;
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
	this->_max_body_size = rhs._max_body_size;
	

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

int	go_default(std::string URI, int connection, int port)
{
	std::string	URL = "http://localhost:" + std::to_string(port) + URI;
   	std::string response =
		"HTTP/1.1 302 Found\r\n"
        "Location: " + URL + "\r\n"
        "\r\n";

	send(connection, response.c_str(), response.length(), 0);
	return (0);
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
			throw std::runtime_error("@fn Network::receive_request(int connection, fd_set &socks)\nrecv connection error");
			delete[] buffer;
		}
		total_bytes += bytes_read;
		request_string += std::string(buffer, bytes_read);
	}

	//DEBUG////////////////
	
	if ( request_string.substr(0, 6).find("POST") != std::string::npos)
	{
		std::cout <<Y<< "ITS POST AND I READ [" << total_bytes << "] bytes\n"<< RE;
		int j = -1;
		while (request_string.c_str()[++j] != '\0')
		{
			if (request_string.c_str()[j] == 6)
			{
				std::cout << R << "ATTENTION YA ACK >< \n" << RE;
				break;
			}
		}
		std::cout<<G<<"### REQUEST = ###\n"<<RE<<request_string/*.substr(0, 3000).c_str()*/
			<<G<< "\n\n### END OF REQUEST ####\n" <<RE;

		std::cout << "size of request_string = [" << request_string.length() << "]\n";

		request.upload_file(request_string); //TEST////////////////
	}
	//DEBUG/////////////////

	delete[] buffer;
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
	this->CatchRequest(request, connection, socks);

	std::cout << R << request << std::endl; //DEBUG

	std::string	URIraw = request.get_URI();
	std::string PathToFile;

	if (URIraw.find("//") != std::string::npos)
		return(SendResponse(404, response, connection));
	std::string Folder = _config.getFolderLocationFromURI(URIraw);
	// test if Folder is in locations ; return *getSingleMapLocation
	std::map<std::string, std::string> *singleLocationContent;
	if ((singleLocationContent = _config.getSingleMapLocation(Folder)) == nullptr){
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


		// test if the file exist in location ; return path to the file or path to the folder in location
		PathToFile = _config.isPathToFile(PathToFile);
		if (PathToFile.empty())
			return (SendResponse(404, response, connection)); // file doesn't exist in folder from locations

		if ( ft_get_extension(PathToFile) == "")
		{
			// ET SI ON A UN FICHIER QUI EXISTE MAIS EST SANS EXTENSION ?? //
		//	std::cout <<R<< "MY PATH TO FILE = " << PathToFile << "\n" RE;

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
				return (0);
			}

			//////////////////////////////////////// C FAIT /////////////////////

			std::string autoindexValue = _config.getAutoindex(*singleLocationContent);
			std::string location = _config.getLocation(*singleLocationContent);

			if (autoindexValue == "true")
			{
				if ( no_final_slash(URIraw) )
					return ( add_slash(URIraw, connection, _port) );
				SendResponseDefault(200, response, connection, PathToFile, URIraw);

				return (0);
			}
			else
			{
				std::map<std::string, std::string> *rootmap
					=_config.getSingleMapLocation("/");

				std::cout <<B<< "MY REFERER IS ["
					<< request.get_header("Referer") << "]\n" << RE; //DEBUG

				if ( request.get_header("Referer").find(URIraw) != std::string::npos
						&& URIraw != "/")
				{
					if ( no_final_slash(URIraw) )
					return ( add_slash(URIraw, connection, _port) );
					SendResponseDefault(200, response, connection, PathToFile, URIraw);

					return (0);
				}
				else
				{
					PathToFile = PathToFile
						+ _config.getDefault(*singleLocationContent);	
					PathToFile = PathToFile.substr( _config.getRoot(*rootmap).size() - 1 );
					
					std::cout <<R<< "MY PATH TO FILE = " << PathToFile << "\n" RE;//DEBUG

					return (go_default(PathToFile, connection, _port));
				}
			}
		}

		response.set_path(PathToFile);
		response.send(connection, socks);
	}
	else if(request.get_type() == "POST")
	{
		if(request.get_header("Content-Type") == "multipart/form-data")
		{
			if(request.get_content_header("Content-Disposition-name") == "file")
			{
				return (upload_file(request, response, connection));
			}
		}
		else
		{
			std::cout << R << "POST request not implemented" << std::endl;
			response.set_path("./website/index.html");
		}
	}
	else if(request.get_type() == "DELETE")
	{
		return ( delete_file(request, response, connection) );
	}

	// test if Method is allowed in Location ; return bool
	if (!_config.IsMethodAllowed(request.get_type(), *(singleLocationContent)))
		return (SendResponse(405, response, connection));

	std::cout << "⬆️ ⬆️ ⬆️\n"<< std::endl;//DEBUG
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
//		std::cout <<R<< "DELETE ISOK\n"<<RE; //DEBUG
		response.set_manual_content_type("text/html");
		response.set_manual_content( ft_generate_success_delete(request) );
		response.send(connection);
		return (0);
	}
	else
		return (SendResponse(500, response, connection));
}


int	Network::upload_file(Request request, Response response, int connection)
{
	std::string	file_path;
	file_path = _config.getUploadFolder();
	//file_path += request.get_content_header("Content-Disposition-filename");
	file_path += request.get_filename_post();

	// Create and open a text file
//	std::cout <<R<< "filepath usi " << file_path << "\n"<<RE;
	std::ofstream file(file_path);
	if (!file)
		return (SendResponse(500, response, connection));

	// Write to the file
	//file << request.get_content_body();
	file << request.get_file_post();

//	std::cout <<G<< "file = :\n" << request.get_content_body().substr(0, 400) << "\n" <<RE;//DEBUG

	// Close the file
	file.close();

	response.set_manual_content_type("text/html");
	response.set_manual_content( ft_generate_success_upload(request) );
	response.send(connection);
	return (0);
}
