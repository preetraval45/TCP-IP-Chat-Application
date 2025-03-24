#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")  // Link WinSock library

#define PORT 8080
#define SERVER_IP "127.0.0.1"  // Change if connecting to another machine
#define BUFFER_SIZE 1024

int main() {
    // Initialize WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return 1;
    }

    // Create client socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return 1;
    }

    // Server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);  // Use inet_addr() instead of inet_pton()

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection to server failed!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server!" << std::endl;

    // Chat loop
    char buffer[BUFFER_SIZE];
    while (true) {
        std::string message;
        std::cout << "You: ";
        std::getline(std::cin, message);

        send(clientSocket, message.c_str(), message.length(), 0);

        ZeroMemory(buffer, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived <= 0) {
            std::cerr << "Server disconnected!" << std::endl;
            break;
        }

        std::cout << "Server: " << buffer << std::endl;
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}