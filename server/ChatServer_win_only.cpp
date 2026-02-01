/**
 * @file ChatServer.cpp
 * @brief Multi-client TCP Chat Server using Winsock
 *
 * This program sets up a TCP server on port 12345 which allows multiple clients to connect,
 * send messages, and broadcast messages to all connected clients except the sender.
 *
 * @author
 * @version 1.0
 */

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <vector>
#include <thread>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib") // Link the Winsock library

using namespace std;

/**
 * @brief Initializes Winsock (version 2.2).
 * @return true if successful, false otherwise.
 */
bool InitializeWinsock() {
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

/**
 * @brief Handles interaction with a connected client.
 *
 * This function runs in a separate thread per client. It listens for messages from
 * the connected client and broadcasts them to all other clients.
 *
 * @param clientSocket The socket connected to the client.
 * @param clients Pointer to the vector of all connected client sockets.
 */
void HandleClient(SOCKET clientSocket, vector<SOCKET>* clients) {
    char buffer[4096] = { 0 };
    string clientName = "Unknown";

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            cout << clientName << " disconnected." << endl;
            break;
        }

        buffer[bytesReceived] = '\0';
        string message(buffer);

        // Check if this is a connection message
        const string connectPrefix = "__CONNECT__";
        if (message.compare(0, connectPrefix.length(), connectPrefix) == 0) {
            clientName = message.substr(connectPrefix.length()); // extract username
            string sysMsg = clientName + " connected.";
            cout << sysMsg << endl;

            // Broadcast system message to others
            for (SOCKET otherClient : *clients) {
                if (otherClient != clientSocket) {
                    send(otherClient, sysMsg.c_str(), static_cast<int>(sysMsg.length()), 0);
                }
            }
            continue; // don't broadcast raw connection message
        }

        // Regular chat message: broadcast to others
        cout << "Message from " << clientName << ": " << message << endl;
        for (SOCKET otherClient : *clients) {
            if (otherClient != clientSocket) {
                send(otherClient, message.c_str(), static_cast<int>(message.length()), 0);
            }
        }
    }

    // Remove client from list & cleanup
    auto it = find(clients->begin(), clients->end(), clientSocket);
    if (it != clients->end()) {
        clients->erase(it);
    }
    closesocket(clientSocket);
}


/**
 * @brief Entry point for the chat server.
 * @return int Exit status code.
 */
int main() {
    cout << "Starting TCP Chat Server..." << endl;

    // Step 1: Initialize Winsock
    if (!InitializeWinsock()) {
        cerr << "Winsock initialization failed. Error: " << WSAGetLastError() << endl;
        return EXIT_FAILURE;
    }
    cout << "Winsock initialized successfully." << endl;

    // Step 2: Create a listening socket
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed. Error: " << WSAGetLastError() << endl;
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Step 3: Bind the socket to an IP and port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);            // Host to network short
    serverAddr.sin_addr.s_addr = INADDR_ANY;       // Accept connections from any IP

    if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Socket binding failed. Error: " << WSAGetLastError() << endl;
        closesocket(listenSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Step 4: Set socket to listen for incoming connections
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Listen failed. Error: " << WSAGetLastError() << endl;
        closesocket(listenSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    cout << "Server is listening on port 12345..." << endl;

    // Step 5: Accept clients and handle them using threads
    vector<SOCKET> clients;

    while (true) {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);

        if (clientSocket == INVALID_SOCKET) {
            cerr << "Accept failed. Error: " << WSAGetLastError() << endl;
            continue; // Continue to accept other clients
        }

        cout << "New client connected. Socket: " << clientSocket << endl;

        // Store client and spawn thread
        clients.push_back(clientSocket);
        thread clientThread(HandleClient, clientSocket, &clients);
        clientThread.detach(); // Let the thread run independently
    }

    // Step 6: Cleanup (this is unreachable in current setup)
    closesocket(listenSocket);
    WSACleanup();

    return EXIT_SUCCESS;
}
