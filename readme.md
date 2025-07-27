# Socket Chat App 💬

A simple yet efficient real-time chat application implemented in C++ using socket programming. This application consists of a server and client component that enables multiple users to communicate in real-time over a network.

## 🌟 Features

- **Real-time Communication**: Direct TCP socket communication between server and clients
- **Multi-client Support**: Server can handle multiple simultaneous client connections
- **Cross-platform Compatibility**: Works on Linux, Windows, and macOS
- **Lightweight**: Minimal resource usage with native C++ implementation
- **Console-based Interface**: Simple terminal-based chat interface
- **Thread-safe Operations**: Proper handling of concurrent client connections

## 🚀 Technologies Used

- **Language**: C++
- **Networking**: TCP Sockets (Berkeley sockets / Winsock)
- **Threading**: POSIX threads (pthread) / Windows threads
- **Build System**: Make / CMake (depending on your setup)

## 📋 Prerequisites

### Linux/macOS
- GCC compiler (g++) version 7.0 or higher
- POSIX-compliant system
- Make utility

### Windows
- MinGW-w64 or Visual Studio with C++ support
- Windows SDK (for Winsock)

## ⚡ Quick Start

### 1. Clone the Repository

```bash
git clone https://github.com/Dipukumar1997/SocketChatApp.git
cd SocketChatApp
```

### 2. Compile the Programs

#### Linux/macOS:
```bash
# Compile server
g++ -o server server.cpp -lpthread

# Compile client
g++ -o client client.cpp -lpthread
```

#### Windows (MinGW):
```bash
# Compile server
g++ -o server.exe server.cpp -lws2_32

# Compile client
g++ -o client.exe client.cpp -lws2_32
```

### 3. Run the Application

#### Start the Server:
```bash
./server
# or on Windows: server.exe
```

#### Connect Clients:
```bash
./client
# or on Windows: client.exe
```
<!-- 
## 🛠️ Compilation Instructions

### Using Make (if Makefile exists)

```bash
make all          # Compile both server and client
make server       # Compile only server
make client       # Compile only client
make clean        # Clean compiled files
```

### Manual Compilation

#### For Linux/Unix Systems:
```bash
# Server compilation
g++ -std=c++11 -Wall -Wextra -o server server.cpp -lpthread

# Client compilation
g++ -std=c++11 -Wall -Wextra -o client client.cpp -lpthread
```

#### For Windows:
```bash
# Server compilation
g++ -std=c++11 -Wall -Wextra -o server.exe server.cpp -lws2_32 -lwinmm

# Client compilation
g++ -std=c++11 -Wall -Wextra -o client.exe client.cpp -lws2_32 -lwinmm
``` -->

## 📁 Project Structure

```
SocketChatApp/
├── server.cpp            # Server implementation
├── client.cpp            # Client implementation
├── README.md             # Project documentation
└── LICENSE               # License file 
```

## 💻 Usage

### Starting the Chat System

1. **Start the Server First:**
   ```bash
   ./server
   ```
   The server will start listening on a default port (usually 8080 or as configured).

2. **Connect Multiple Clients:**
   ```bash
   ./client
   ```
   Each client will prompt for connection details and username.

3. **Start Chatting:**
   - Type messages in any client terminal
   - Messages will be broadcasted to all connected clients
   - Use special commands (if implemented) like `/quit` to exit

### Configuration

#### Server Configuration
The server typically uses these default settings:
- **Port**: 8080 (see the console for {PORT})
- **Max Connections**: 10 (or as configured)
- **Buffer Size**: 1024 bytes (or as defined)

#### Client Configuration
- **Server IP**: localhost/127.0.0.1 (default)
- **Server Port**: Must match server port
- **Username**: Prompted during connection

## 🔧 Customization

### Modifying Connection Parameters

#### In server.cpp:
```cpp
#define PORT 8080           // Change server port
#define MAX_CLIENTS 10      // Maximum concurrent clients
#define BUFFER_SIZE 1024    // Message buffer size
```

#### In client.cpp:
```cpp
#define SERVER_IP "127.0.0.1"  // Server IP address
#define PORT 8080              // Server port (must match server)
#define BUFFER_SIZE 1024       // Message buffer size
```

## 🧪 Testing

### Basic Functionality Test

1. **Single Client Test:**
   ```bash
   # Terminal 1
   ./server
   
   # Terminal 2
   ./client
   ```

2. **Multiple Clients Test:**
   ```bash
   # Terminal 1: Server
   ./server
   
   # Terminal 2: Client 1
   ./client
   
   # Terminal 3: Client 2
   ./client
   
   # Terminal 4: Client 3
   ./client
   ```

3. **Network Test:**
   - Run server on one machine
   - Connect clients from different machines on the same network
   - Update client.cpp with server's actual IP address

## 🐛 Troubleshooting

### Common Issues

**Connection Refused:**
- Ensure server is running before starting clients
- Check if the port is already in use
- Verify firewall settings

**Compilation Errors:**
```bash
# Linux: Install build essentials
sudo apt-get install build-essential

# Missing pthread library
sudo apt-get install libc6-dev
```

**Windows Socket Errors:**
- Ensure Winsock2 is properly linked (-lws2_32)
- Check Windows Defender firewall settings

### Debug Mode

Compile with debug flags for troubleshooting:
```bash
g++ -g -DDEBUG -o server server.cpp -lpthread
g++ -g -DDEBUG -o client client.cpp -lpthread
```

## 🚀 Deployment

### Local Network Deployment

1. **Configure Server IP:**
   - Find server machine's IP address: `ifconfig` (Linux/Mac) or `ipconfig` (Windows)
   - Update client.cpp with the server's IP address

2. **Firewall Configuration:**
   - Open the port used by the server (default 8080)
   - Allow incoming connections for the server application

3. **Run Across Network:**
   ```bash
   # On server machine
   ./server
   
   # On client machines
   ./client
   ```

## 📚 Code Architecture

### Server Architecture
- **Main Thread**: Accepts incoming connections
- **Client Threads**: Handle individual client communication
- **Broadcast Function**: Sends messages to all connected clients
- **Connection Management**: Maintains list of active clients

### Client Architecture
- **Main Thread**: Handles user input
- **Receive Thread**: Listens for incoming messages from server
- **Send Function**: Sends user messages to server

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. **Fork the repository**
2. **Create a feature branch:**
   ```bash
   git checkout -b feature/new-feature
   ```
3. **Make your changes:**
   - Follow C++ best practices
   - Add comments for complex logic
   - Test thoroughly on different platforms
4. **Commit your changes:**
   ```bash
   git commit -m "Add new feature: description"
   ```
5. **Push and create a Pull Request**

### Coding Standards
- Use consistent indentation (4 spaces or tabs)
- Follow C++ naming conventions
- Add error handling for socket operations
- Include comments for complex algorithms
- Test on both Linux and Windows if possible

## 🔮 Future Enhancements

- [ ] GUI interface using Qt or GTK
- [ ] Private messaging between specific users
- [ ] File transfer capabilities
- [ ] Message encryption (SSL/TLS)
- [ ] User authentication system
- [ ] Chat rooms/channels
- [ ] Message history logging
- [ ] Configuration file support
- [ ] IPv6 support
- [ ] Reconnection handling

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Dipu Kumar**
- GitHub: [@Dipukumar1997](https://github.com/Dipukumar1997)
<!-- - LinkedIn: [Your LinkedIn Profile] -->
- Email: [dk95074450@gmail.com]

## 🙏 Acknowledgments

- C++ Socket Programming tutorials and documentation
- POSIX threading documentation
- Winsock API documentation
- Open source community for inspiration and best practices

## 📞 Support

If you encounter issues:

1. Check the [Issues](https://github.com/Dipukumar1997/SocketChatApp/issues) section
2. Create a new issue with:
   - Operating system and version
   - Compiler version
   - Error messages or unexpected behavior
   - Steps to reproduce the problem

---

⭐ **Star this repository if you found it helpful!**
