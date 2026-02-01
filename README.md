# Socket Chat Application (C++)

A real-time, console-based multi-client chat application developed in C++ using **TCP/IP socket programming**. The application follows a **client–server architecture**, where the server manages multiple concurrent client connections using **POSIX threads (pthreads)** and ensures **thread-safe message broadcasting** over TCP sockets.

## Build and Run

### Linux / macOS
```bash
g++ server.cpp -o server -lpthread
g++ client.cpp -o client -lpthread

./server
./client
