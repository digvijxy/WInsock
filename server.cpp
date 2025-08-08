// server.cpp - Simple Winsock TCP Server
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // Link with Winsock Library

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in server, client;
    int c;
    char* message;
      char buffer[1024];

    // Initialize Winsock
     //Windows-specific API to communicate over the network(TCP)
    std::cout << "Initializing Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Failed. Error Code: " << WSAGetLastError();
        return 1;
    }

    std::cout << "Winsock Initialized.\n";

    // Create a socket for IPV4
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Could not create socket: " << WSAGetLastError();
        return 1;
    }

    std::cout << "Socket created.\n";

    // Prepare the sockaddr_in struct 
//     struct sockaddr_in {
//     short sin_family;       // Address family (AF_INET for IPv4)
//     u_short sin_port;       // Port number (must be in network byte order)
//     struct in_addr sin_addr;// IP address (also in network byte order)
//     char sin_zero[8];       // Padding, not used (just to match size of sockaddr)
// };

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind
    if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError();
        return 1;
    }

    std::cout << "Bind done.\n";

    // Listen
    listen(serverSocket, 3);

    // Accept incoming connections
    std::cout << "Waiting for incoming connections\n";
    c = sizeof(struct sockaddr_in);
    clientSocket = accept(serverSocket, (struct sockaddr*)&client, &c);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError();
        return 1;
    }

    std::cout << "Connection accepted.\n";


    int bytesReceived = recv(clientSocket, buffer, 1024 - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received string
        std::cout << "Client says: " << buffer << std::endl;

        // Send response back to client
        const char* response = "Hi Client, message received!";
        send(clientSocket, response, strlen(response), 0);
    } else {
        std::cout << "Failed to receive message from client." << std::endl;
    }

    // Send a welcome message to the client
    
    message = "Hello Client, connection established.\n";
    send(clientSocket, message, strlen(message), 0);

    // Close sockets

    closesocket(serverSocket);
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
