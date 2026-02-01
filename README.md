# Socket Chat Application (C++)

A real-time multi-client chat application implemented using C++ and TCP socket programming.

## Build and Run

### Linux / macOS
```bash
g++ server.cpp -o server -lpthread
g++ client.cpp -o client -lpthread

./server
./client
