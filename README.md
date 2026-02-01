# Socket Chat Application (C++)

A real-time, console-based TCP chat application implemented in C++.  
The project follows a **client–server architecture** and demonstrates **low-level socket programming**, **multithreaded I/O**, and **thread-safe console handling**.

The client is implemented using **Winsock (Windows)** and communicates with the server over **TCP/IP** for reliable message delivery.

## Key Technical Details
- Uses **TCP sockets** (AF_INET, SOCK_STREAM) for reliable communication
- Client implemented using **Winsock 2.2**
- **Multithreaded design**:
  - One thread for sending user input
  - One thread for receiving server messages
- **Thread-safe console output** using `std::mutex`
- Graceful connection handling and clean shutdown (`quit` / `exit`)
- Full-line input handling with username-prefixed messages

## Build and Run

### Windows (MinGW) and Linux
```bash
g++ ChatClient.cpp -o client.exe -lws2_32
client.exe

g++ client.cpp -o client -lpthread
./client

