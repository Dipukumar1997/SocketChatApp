/**
 * @file ChatClient.cpp
 * @brief Cross-platform TCP Chat Client with multithreading and user-friendly prompts.
 *
 * This client connects to a TCP chat server and allows the user to send messages.
 * On startup, it announces the user's connection to the server.
 * Supports clean termination with "quit" or "exit".
 *
 * Usage:
 *  - Compile and run.
 *  - Enter your chat name.
 *  - Start typing messages; type "quit" or "exit" to disconnect.
 *
 * Cross-platform: Works on Windows (Winsock2) and Linux (POSIX sockets).
 * Uses threads and mutex for concurrent send/receive with safe console output.
 *
 * @author
 * @version 2.0
 */

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <limits>

using namespace std;

/* ================= PLATFORM ABSTRACTION ================= */

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET socket_t;
    #define GET_LAST_ERROR() WSAGetLastError()
    #define CLEANUP() WSACleanup()
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <cerrno>
    #include <cstring>
    typedef int socket_t;
    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR   (-1)
    #define closesocket close
    #define GET_LAST_ERROR() errno
    #define CLEANUP() // no-op on Linux
#endif

/* ======================================================== */

mutex printMutex;

/**
 * @brief Initialize networking (Winsock on Windows, noop on Linux)
 * @return true if successful
 */
bool InitializeNetworking() {
#ifdef _WIN32
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2,2), &wsaData) == 0;
#else
    return true;
#endif
}

/**
 * @brief Thread function to send messages
 */
void sendMesg(socket_t sock) {
    string name;
    do {
        lock_guard<mutex> lock(printMutex);
        cout << "Enter your chat name: ";
        getline(cin >> ws, name);
    } while (name.empty());

    string connectMsg = "__CONNECT__" + name;
    send(sock, connectMsg.c_str(), (int)connectMsg.length(), 0);

    string message;
    while (true) {
        {
            lock_guard<mutex> lock(printMutex);
            cout << "Send your message: ";
            cout.flush();
        }
        getline(cin, message);
        if (message.empty()) continue;

        string fullMsg = name + " : " + message;
        if (send(sock, fullMsg.c_str(), (int)fullMsg.length(), 0) == SOCKET_ERROR) {
            lock_guard<mutex> lock(printMutex);
            cerr << "\nError sending message. Err: " << GET_LAST_ERROR() << endl;
            break;
        }

        if (message == "quit" || message == "exit") {
            lock_guard<mutex> lock(printMutex);
            cout << "\nStopping the application." << endl;
            break;
        }
    }
    closesocket(sock);
    CLEANUP();
}

/**
 * @brief Thread function to receive messages
 */
void recvMesg(socket_t sock) {
    char buffer[4096];
    while (true) {
        int recvLen = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (recvLen <= 0) {
            lock_guard<mutex> lock(printMutex);
            cout << "\nDisconnected from server. Err: " << GET_LAST_ERROR() << endl;
            break;
        }
        buffer[recvLen] = '\0';
        {
            lock_guard<mutex> lock(printMutex);
            cout << "\n" << buffer << endl;
            cout << "Send your message: ";
            cout.flush();
        }
    }
    closesocket(sock);
    CLEANUP();
}

/**
 * @brief Main client entry point
 */
int main() {
    if (!InitializeNetworking()) {
        cerr << "Error initializing networking." << endl;
        return 1;
    }

    cout << "Client started" << endl;

    socket_t clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed. Err: " << GET_LAST_ERROR() << endl;
        CLEANUP();
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Unable to connect to server. Err: " << GET_LAST_ERROR() << endl;
        closesocket(clientSocket);
        CLEANUP();
        return 1;
    }

    cout << "Successfully connected to server" << endl;

    thread sender(sendMesg, clientSocket);
    thread receiver(recvMesg, clientSocket);

    sender.join();
    receiver.join();

    return 0;
}
