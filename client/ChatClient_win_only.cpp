/**
 * @file ChatClient.cpp
 * @brief TCP Chat Client using Winsock with user-friendly prompts and connection announcement.
 *
 * This client connects to a TCP chat server and allows the user to send messages.
 * On startup, it sends a connection notification message to the server.
 * The client reads full-line input messages, sends them prefixed with the username,
 * and supports clean termination with "quit" or "exit" commands.
 *
 * Usage:
 *  - Compile and run.
 *  - Enter your chat name.
 *  - Start typing messages; type "quit" or "exit" to disconnect.
 *
 * @author
 * @version 1.0
 */

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include<thread>
#include <limits>
#include <mutex>

std::mutex printMutex;


#pragma comment(lib, "ws2_32.lib") // Link Winsock library

using namespace std;

/**
 * @brief Initialize Winsock library (version 2.2).
 * @return true if initialization is successful, false otherwise.
 */
bool InitializeWinsock() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

/**
 * @brief Handles sending messages to the server from the client.
 *
 * First sends a connection message announcing the user's chat name.
 * Then enters a loop to read user input and send messages prefixed with the chat name.
 * Exits cleanly when user types "quit" or "exit".
 *
 * @param clientSocket Connected socket to the chat server.
 */

/*

void sendMesg(SOCKET s) {
    string name;
    // Prompt user repeatedly until a valid non-empty chat name is entered
    do {
        cout << "Enter your chat name: ";
        getline(cin >> ws, name);  // Read full line, ignore leading whitespace
    } while (name.empty());

    // Send connection message with special prefix to the server
    string connectMsg = "__CONNECT__" + name;
    send(s, connectMsg.c_str(), static_cast<int>(connectMsg.length()), 0);

    string message;
    while (true) {
        cout << "Send your message: ";
        getline(cin, message);

        if (message.empty()) {
            // Ignore empty messages and keep prompting
            continue;
        }

        string fullMsg = name + " : " + message;
        int byteSent = send(s, fullMsg.c_str(), static_cast<int>(fullMsg.length()), 0);

        if (byteSent == SOCKET_ERROR) {
            cerr << "Error sending message." << endl;
            break;
        }

        if (message == "quit" || message == "exit") {
            cout << "Stopping the application." << endl;
            break;
        }
    }
    closesocket(s);
    WSACleanup();
}
*/

void sendMesg(SOCKET s) {
    string name;
    do {
        std::lock_guard<std::mutex> lock(printMutex);
        cout << "Enter your chat name: ";
        getline(cin >> ws, name);
    } while (name.empty());

    string connectMsg = "__CONNECT__" + name;
    send(s, connectMsg.c_str(), (int)connectMsg.length(), 0);

    string message;
    while (true) {
        {
            std::lock_guard<std::mutex> lock(printMutex);
            cout << "Send your message: ";
            cout.flush();
        }
        getline(cin, message);
        if (message.empty()) continue;

        string fullMsg = name + " : " + message;
        int byteSent = send(s, fullMsg.c_str(), (int)fullMsg.length(), 0);

        if (byteSent == SOCKET_ERROR) {
            std::lock_guard<std::mutex> lock(printMutex);
            cerr << "\nError sending message." << endl;
            break;
        }
        if (message == "quit" || message == "exit") {
            std::lock_guard<std::mutex> lock(printMutex);
            cout << "\nStopping the application." << endl;
            break;
        }
    }
    closesocket(s);
    WSACleanup();
}




/**
 * @brief Handles receiving messages from the server.
 *
 * Continuously listens for messages from the server and prints them to console.
 * Terminates when the connection is closed or an error occurs.
 *
 * @param clientSocket Connected socket to the chat server.
 */

/*
void recvMesg(SOCKET clientSocket) {
    char buffer[4096];
    while (true) {
        int recvLen = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (recvLen <= 0) {
            cout << "\nDisconnected from server." << endl;
            break;
        }

        buffer[recvLen] = '\0'; // Null-terminate the received data
        cout << "\n" << buffer << endl;
        cout << "Send your message: ";
        cout.flush();
    }

    closesocket(clientSocket);
    WSACleanup();
}

*/

void recvMesg(SOCKET s) {
    char buffer[4096];
    while (true) {
        int recvLen = recv(s, buffer, sizeof(buffer) - 1, 0);
        if (recvLen <= 0) {
            std::lock_guard<std::mutex> lock(printMutex);
            cout << "\nDisconnected from server." << endl;
            break;
        }
        buffer[recvLen] = '\0'; // Null terminate safely

        {
            std::lock_guard<std::mutex> lock(printMutex);
            cout << "\n" << buffer << endl;  // Print incoming message on new line
            cout << "Send your message: ";   // Re-print prompt for user input
            cout.flush();
        }
    }
    closesocket(s);
    WSACleanup();
}

/**
 * @brief Main entry point for the chat client.
 *
 * Initializes Winsock, creates socket, connects to server,
 * then starts send and receive threads.
 *
 * @return int Exit status code.
 */
int main() {
    if (!InitializeWinsock()) {
        cerr << "Error initializing Winsock." << endl;
        return 1;
    }

    cout << "Client started" << endl;

    // Create TCP socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // Server address setup
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // Connect to server
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Unable to connect to server. Error: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Successfully connected to server" << endl;

    // Launch sender and receiver threads
    thread sender(sendMesg, clientSocket);
    thread receiver(recvMesg, clientSocket);

    sender.join();
    receiver.join();

    return 0;
}
