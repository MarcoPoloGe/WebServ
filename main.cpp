#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <sstream>
#include <fcntl.h>

#include "includes/ClassRequest.hpp"
#include "includes/Ft_error.hpp"
//#include <stdlib.h>



const int MAX_CLIENTS = 5;
const int BUFFER_SIZE = 10024;

std::string ft_read_file(std::string file_name)
{
	std::ifstream html_file(file_name.c_str());
	std::string content;
	std::string line;

	if(html_file.is_open())
	{
		std::cout << "yes \n\n\n";
	} else
	{
		std::cout << "fuck\n\n\n";
	}
	while (std::getline(html_file, line))
	{
		content += line + '\n';
	}
	return (content);
}

void custom_request(int client_socket);

int custom_msg(Request request, int client_socket)
{
	if (request.get_URI() == "/marco")
	{
	//	custom_request(client_socket); //test
	//	return (1);

		std::string rep = "POLO !!!!!!!!!!\n";
		send(client_socket, rep.c_str(), rep.length(), 0);
		return (1);
	}
	return (0);
}

void custom_request(int client_socket) {
	std::stringstream response;
	std::string html_content;

	std::cout << "We had a custom request\n";
	
//	send(client_socket, "AHAHAH", 4, 0);//test cpt
//	return ;

	// Read the incoming request
	char buffer2[BUFFER_SIZE];
	int bytes_read = recv(client_socket, buffer2, BUFFER_SIZE, 0);
	if (bytes_read < 1)
	{
		std::cout << "Error on recv\n";
		std::exit(1);
	}

	std::cout << "we recieved " << bytes_read << " bytes to read\n";
	if (bytes_read > 0) {
		std::cout << "reading startd\n";

		// Extract the request headers and body
		std::string request_string(buffer2, bytes_read);
		std::cout << "Received request:\n" << request_string << std::endl;

		// Send a response
		// Set the HTTP response headers
		response << "HTTP/1.1 200 OK\r\n";
		response << "Content-Type: text/html\r\n";
		
		html_content = ft_read_file("./website/test1.html");
		response << "Content-Length: " << html_content.length() << "\r\n";
		response << "\r\n";

		// Send the response headers and body

		std::string temp;
		temp = response.str();

		send(client_socket, temp.c_str(), temp.length(), 0);
		send(client_socket, html_content.c_str(), html_content.length(), 0);
	}
	std::cout << "The custom request ended\n";
}

void handle_request(int client_socket) {

{
	Request request;
	std::stringstream response;
	std::string html_content;

	// Read the incoming request
	char buffer[BUFFER_SIZE];
	int bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);

	if (bytes_read > 0) {
		// Extract the request headers and body
		std::string request_string(buffer, bytes_read);
		request.fill(request_string);
		std::cout << "Parsed request:\n" << request << std::endl;
		///////////////////////////////////////////////////
		//////////////////TELLNET//////////////////////////
		///////////////////////////////////////////////////
		if (custom_msg(request, client_socket) == 1)
			return ;
		//pour tester ca, installe telnet avec brew
		//depuis le terminal: `telnet localhost 443`
		//si tu dis le mot magique tu auras une surprise
		///////////////////////////////////////////////////

		// Send a response
		// Set the HTTP response headers
		response << "HTTP/1.1 200 OK\r\n";
		response << "Content-Type: text/html\r\n";
		html_content = ft_read_file("./website/index.html");
		response << "Content-Length: " << html_content.length() << "\r\n";
		response << "\r\n";

		// Send the response headers and body

		std::string temp;
		temp = response.str();

		send(client_socket, temp.c_str(), temp.length(), 0);
		send(client_socket, html_content.c_str(), html_content.length(), 0);
	}
}

/*{	
	//// TEST WITH IMAGE ////
		
	std::stringstream response;
	std::string html_content;

	// Read the incoming request
	char buffer[BUFFER_SIZE];
	int bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);

	if (bytes_read > 0) {
		// Extract the request headers and body
		std::string request_string(buffer, bytes_read);
		std::cout << "Received request:\n" << request_string << std::endl;

		// Send a response
		// Set the HTTP response headers
		response << "HTTP/1.1 200 OK\r\n";
		response << "Content-Type: image/jpeg\r\n";
		html_content = ft_read_file("./website/jesus.jpeg");
		response << "Content-Length: " << html_content.length() << "\r\n";
		response << "\r\n";

		// Send the response headers and body

		std::string temp;
		temp = response.str();

		send(client_socket, temp.c_str(), temp.length(), 0);
		send(client_socket, html_content.c_str(), html_content.length(), 0);
	}
}*/

}

int main(int ac, char **av) {

	if (ac != 2)
		Ft_error err("Bad arguments");

	(void)av;

	// Create a socket for incoming connections
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
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
		int client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_address_size);
		std::cout << "Accepted connection from " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;

		// Handle the request
		handle_request(client_socket);

		// Close the connection
		close(client_socket);
	}
}

