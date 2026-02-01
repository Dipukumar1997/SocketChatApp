/**
 * @file ChatServer.cpp
 * @brief Multi-client TCP Chat Server (cross-platform)
 *
 * This program sets up a TCP server on port 12345 which allows multiple clients to connect,
 * send messages, and broadcast messages to all connected clients except the sender.
 *
 * @author
 * @version 1.0
 */

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>

using namespace std;

/* ================= PLATFORM ABSTRACTION ================= */

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")

    typedef SOCKET socket_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <cstring>
    #include <cerrno>

    typedef int socket_t;
    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR   (-1)
    #define closesocket close
#endif

/* ======================================================== */

std::mutex printMutex; // For thread-safe printing

/* ===================== FUNCTIONS ======================= */

bool InitializeSockets() {
#ifdef _WIN32
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2,2), &wsaData) == 0;
#else
    return true; // No init required on Linux
#endif
}

void CleanupSockets() {
#ifdef _WIN32
    WSACleanup();
#endif
}

void HandleClient(socket_t clientSocket, vector<socket_t>* clients) {
    char buffer[4096];
    string clientName = "Unknown";

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            lock_guard<mutex> lock(printMutex);
            cout << clientName << " disconnected." << endl;
            break;
        }

        buffer[bytesReceived] = '\0';
        string message(buffer);

        // Check connection message
        const string connectPrefix = "__CONNECT__";
        if (message.compare(0, connectPrefix.size(), connectPrefix) == 0) {
            clientName = message.substr(connectPrefix.size());
            string sysMsg = clientName + " connected.";
            lock_guard<mutex> lock(printMutex);
            cout << sysMsg << endl;

            for (socket_t otherClient : *clients) {
                if (otherClient != clientSocket) {
                    send(otherClient, sysMsg.c_str(), static_cast<int>(sysMsg.length()), 0);
                }
            }
            continue; // Skip broadcasting the raw connect message
        }

        // Broadcast chat message to other clients
        lock_guard<mutex> lock(printMutex);
        cout << clientName << ": " << message << endl;

        for (socket_t otherClient : *clients) {
            if (otherClient != clientSocket) {
                send(otherClient, message.c_str(), static_cast<int>(message.length()), 0);
            }
        }
    }

    // Remove client from vector
    auto it = find(clients->begin(), clients->end(), clientSocket);
    if (it != clients->end()) clients->erase(it);

    closesocket(clientSocket);
}

/* ======================== MAIN ========================= */

int main() {
    cout << "Starting TCP Chat Server..." << endl;

    if (!InitializeSockets()) {
        cerr << "Socket initialization failed." << endl;
        return 1;
    }

    // Create listening socket
    socket_t listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed." << endl;
        CleanupSockets();
        return 1;
    }

    // Bind socket
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Bind failed." << endl;
        closesocket(listenSocket);
        CleanupSockets();
        return 1;
    }

    // Listen
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Listen failed." << endl;
        closesocket(listenSocket);
        CleanupSockets();
        return 1;
    }

    cout << "Server listening on port 12345..." << endl;

    vector<socket_t> clients;

    while (true) {
        socket_t clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Accept failed." << endl;
            continue;
        }

        {
            lock_guard<mutex> lock(printMutex);
            cout << "New client connected. Socket: " << clientSocket << endl;
        }

        clients.push_back(clientSocket);
        thread clientThread(HandleClient, clientSocket, &clients);
        clientThread.detach();
    }

    closesocket(listenSocket);
    CleanupSockets();
    return 0;
}
