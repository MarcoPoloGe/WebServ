#ifndef NETWORK_HPP
# define NETWORK_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include <stdexcept>
# include "Config.hpp"
#include "Webserv_Includes.hpp"

#ifndef MAX_CLIENTS
#define MAX_CLIENTS 5
#endif

class Network
{

public:
	//Constructors
	
	Network(Config config, int portNo);
	Network(Network const &src);

	Network(const int port, Config config);

	~Network(void);


	//Accessors

	int		getSock(void);
	int		getPort(void);  /////////todo
	

	//Public Functions
	
	void		deal_with_data			(int connection, fd_set socks);
	int			RequestToResponse		(int connection, fd_set socks);

	bool 		CatchRequest			(Request &request, int connection, fd_set socks);

	int 		SendResponse			(int errorCode, Response &response, int connection);
	int			SendCGIResponse			(int errorCode, Response &response, Request &request, int connection, const std::string& path);
	int			SendResponseDefault		(int errorCode, Response &response, int connection, std::string path, std::string URIraw);

	Request		receive_request			(int connection, fd_set &socks);

	int			Redirection				(int connection, std::string redirect_URL);
	int			delete_file				(Request request, Response response, int connection);
	int			upload_file				(Request request, Response response, int connection);
	void		addName					(Config config); ///todo
														//do getPort();
private:
	//Nope nope nope

	Network(void);
	Network	&operator=(Network const &rhs);

	//Private Functions
	
	void	setnonblocking(int sock);
	void	check_host(Request request);
	

	//Variables
	
	int											_max_body_size;
	int											_sock;
	Config										_config;
	Config										_default_conf;
	int											_port;
	struct sockaddr_in							_server_address;
	int											_reuse_addr;
	std::map<std::string, Config>				_names;
};

#endif
