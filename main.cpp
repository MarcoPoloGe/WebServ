#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <sstream>
//#include <stdlib.h>


using namespace std;

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

void handle_request(int client_socket) {

{
	std::stringstream response;
	std::string html_content;

	// Read the incoming request
	char buffer[BUFFER_SIZE];
	int bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);

	if (bytes_read > 0) {
		// Extract the request headers and body
		string request_string(buffer, bytes_read);
		cout << "Received request:\n" << request_string << endl;

		// Send a response
		// Set the HTTP response headers
		response << "HTTP/1.1 200 OK\r\n";
		response << "Content-Type: text/html\r\n";
		html_content = ft_read_file("./website/index.html");
		response << "Content-Length: " << html_content.length() << "\r\n";
		response << "\r\n";

	// Send the response headers and body

	string temp;
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
		string request_string(buffer, bytes_read);
		cout << "Received request:\n" << request_string << endl;

		// Send a response
		// Set the HTTP response headers
		response << "HTTP/1.1 200 OK\r\n";
		response << "Content-Type: image/jpeg\r\n";
		html_content = ft_read_file("./website/jesus.jpeg");
		response << "Content-Length: " << html_content.length() << "\r\n";
		response << "\r\n";

	// Send the response headers and body

	string temp;
	temp = response.str();

	send(client_socket, temp.c_str(), temp.length(), 0);
	send(client_socket, html_content.c_str(), html_content.length(), 0);

	}
}*/

}

int main() {
	// Create a socket for incoming connections
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// Bind the socket to a port and address
	sockaddr_in server_address = {0,0,0,{0},{0}}; //on doit intialiser a zero du au faite que c'est une struct C.
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY permet de bind à toutes les adresses qui s'adresse a au port 443.
	server_address.sin_port = htons(443);
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	// Listen for incoming connections
	listen(server_socket, MAX_CLIENTS);
	cout << "Server listening on port 443..." << endl;

	// Accept incoming connections and handle requests
	while (true) {
		// Wait for a client to connect
		sockaddr_in client_address;
		socklen_t client_address_size = sizeof(client_address);
		int client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_address_size);
		cout << "Accepted connection from " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << endl;

		// Handle the request
		handle_request(client_socket);

		// Close the connection
		close(client_socket);
	}
}

