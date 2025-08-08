// client.cpp - Simple Winsock TCP Client
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // Link against the Winsock library

int main() {
    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in server;
    char buffer[1024];

    // Step 1: Initialize Winsock
    std::cout << "Initializing Winsock...\n";
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Failed. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    std::cout << "Winsock initialized.\n";

    // Step 2: Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Could not create socket: " << WSAGetLastError() << std::endl;
        return 1;
    }

    std::cout << "Socket created.\n";

    // Step 3: Define server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Connect to localhost

    // Step 4: Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server.\n";

    const char* message = "Hello Server!";
    send(clientSocket, message, strlen(message), 0);

    // Step 5: Receive message from server
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
    } else {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        std::cout << "Server response: " << buffer << std::endl;
    }

    // Step 6: Clean up
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
