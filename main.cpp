#include "includes/Webserv_Includes.hpp"

void handle_request(int client_socket) {

	Request request;
	std::stringstream response;
	std::string html_content;

	// Read the incoming request
	char buffer[BUFFER_SIZE];
	int bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);

	if (bytes_read < 0)
	{
		std::cout << "Problem with recv :( \n";
		return ;
	}

	if (bytes_read > 0) {
		// Extract the request headers and body
		std::string request_string(buffer, bytes_read);
		request.fill(request_string);
		std::cout << "Parsed request:\n" << request << std::endl;

		/////////////////////AJOUT RENO 28.02.23//////////////////////
		//////////////////////////////////////////////////////////////

		std::string	root = "./website";  //a get dans le vrai config file
		std::string	host;
		std::string	referer;
		std::string	uri;
		std::string	path;
		std::map<std::string, std::string> mapa = request.get_headers_map();
		std::map<std::string, std::string>::iterator it = mapa.begin();
		int	rep_code = 0;
		std::pair<std::string, std::string> file_type = std::make_pair("type", "imgtype");
		
		uri = request.get_URI();

		while (it != mapa.end() )
		{
			/*std::cout << "in headers_map[" << it->first << "], there is : ["
				<< it->second << "]";*/		//DEBUG

			if (it->first == "Host")
				host = it->second;
			if (it->first == "Referer")
				referer = it->second;
			it++;
			std::cout << std::endl;
		}
		/*std::cout << "HOST = " << host << std::endl;
		std::cout << "REFERER = " << referer << std::endl;
		std::cout << "URI = " << uri << std::endl;*/	//DEBUG

		if ( !referer.empty() )
		{
			size_t index = referer.find(host);
			/*if (index == std::string::npos)
				std::cout << "APA TROUVE :(((\n";*/		//DEBUG
			path = root + referer.substr( (index + host.size()) );

			/*std::cout << "jai coupe a l'index = " << index + host.size() << "\n";
			std::cout << "car index = " << index 
				<< " et host.size = " << host.size() << "\n";*/		//DEBUG
		}
		else
			path = root + uri;
		
		std::cout << "***Try to access : {" << path << "}***\n";		//DEBUG

		if (path == root + "/")
		{
			html_content = ft_read_file(root + "/index.html"); //index aussi a chercher dans config
			file_type.first = "html"; file_type.second = "html";
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
				html_content = ft_read_file(path);
			}
			else
			{
				rep_code = 404;
				html_content = ft_read_file("./website/error-404.html");
				file_type.first = "html"; file_type.second = "html";
				goto fill_rep;
			}
			std::size_t last_point = path.rfind(".");
			if (last_point != std::string::npos)
			{
				file_type.second = path.substr(last_point + 1);
				if (file_type.second == "html")
					file_type.first = "html";
				else
					file_type.first = "image";	//pr l'instant on a que img et html donc a voir
			}
			else
			{
				file_type.first = "html"; file_type.second = "html";
			}
		}
fill_rep:
		std::cout << "@@@@file type@@@@\n" 
			<< "file type first = " << file_type.first << "\n"
			<< "file type second = "  << file_type.second << "\n"
			<< "@@@@@@@@@@@@@@@@@\n";

		/////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////
		
		// Send a response
		// Set the HTTP response headers
		if (rep_code == 200)
			response << "HTTP/1.1 200 OK\n";
		else
			response << "HTTP/1.1 404 Not Found\n";

		if (file_type.first == "html")
			response << "Content-Type: text/html\n";
		else if (file_type.first == "image")
		{
			response << "Content-Type: image/";
			response << file_type.second << "\n";
		}

		response << "Content-Length: " << html_content.length() << "\n";
		response << "\n";

		// Send the response headers and body

		std::string temp;
		temp = response.str();

		std::cout << "###### RESPONSE ######\n" << response.str() << std::endl;
		std::cout << "#########HTML#########\n" << html_content.c_str() << std::endl;
		std::cout << "######################\n";

		send(client_socket, temp.c_str(), temp.length(), 0);
		send(client_socket, html_content.c_str(), html_content.length(), 0);

	}
}
/*
int	main(int ac, char **av)
{
	(void)av;
	if (ac != 2)
		Ft_error err("Bad arguments");
	Network	serv(8080);
	serv.run();
	return (0);
}*/


int main(int ac, char **av) {
	if (ac != 2)
		Ft_error err("Bad arguments");

	(void)av;

	// Create a socket for incoming connections
	int	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
		Ft_error	err(NULL);

	// Bind the socket to a port and address
	sockaddr_in server_address = {0,0,0,{0},{0}}; //on doit intialiser a zero du au faite que c'est une struct C.
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY permet de bind à toutes les adresses qui s'adresse a au port 8080.
	server_address.sin_port = htons(8080);
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	// Listen for incoming connections
	listen(server_socket, MAX_CLIENTS);
	std::cout << "Server listening on port 8080..." << std::endl;

	// Accept incoming connections and handle requests
	while (true) {

		// Wait for a client to connect
		sockaddr_in client_address;
		socklen_t client_address_size = sizeof(client_address);
		int client_socket = accept(server_socket, (struct sockaddr*) &client_address,
				&client_address_size);
		std::cout << "Accepted connection from " << inet_ntoa(client_address.sin_addr)
			<< ":" << ntohs(client_address.sin_port) << std::endl;

		// Handle the request
		handle_request(client_socket);


		// Close the connection
		close(client_socket);
	}
}
