# Socket Chat Application (C++)

A real-time, console-based TCP chat application implemented in C++.  
The project uses a **client–server architecture** and demonstrates **low-level socket programming**, **multithreaded I/O**, and **thread-safe console handling**.

## Project Structure

- **Client**
  - `ChatClient_cross_platform.cpp` — Cross-platform client (Windows & Linux)
  - `ChatClient_win_only.cpp` — Windows-only client using Winsock

- **Server**
  - `ChatServer_cross_platform.cpp` — Cross-platform server (Windows & Linux)
  - `ChatServer_win_only.cpp` — Windows-only server using Winsock

## Key Technical Details
- Uses **TCP sockets** (`AF_INET`, `SOCK_STREAM`) for reliable communication
- Multithreaded design with separate send/receive threads
- Thread-safe console output using `std::mutex`
- Graceful connection handling and clean shutdown (`quit` / `exit`)
- Full-line input handling with username-prefixed messages

## Build and Run

### Server (Run the server first // then run the client similarly)

#### Cross-platform (Linux & Windows)
```bash
# Linux
g++ server/ChatServer_cross_platform.cpp -o server -lpthread
./server

# Windows (MinGW)
g++ server/ChatServer_cross_platform.cpp -o server.exe -lws2_32
server.exe
