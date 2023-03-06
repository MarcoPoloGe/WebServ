#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int MAX_CLIENTS = 5;
const int BUFFER_SIZE = 10024;

void handle_request(int client_socket) {
	// Read the incoming request
	char buffer[BUFFER_SIZE];
	int bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);

	if (bytes_read > 0) {
		// Extract the request headers and body
		string request_string(buffer, bytes_read);
		cout << "Received request:\n" << request_string << endl;

		// Send a response
		string response_string = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
		send(client_socket, response_string.c_str(), response_string.size(), 0);
	}
}

int main() {
	// Create a socket for incoming connections
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// Bind the socket to a port and address
	sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
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