# Ultimata Server - Cross-Platform Build Guide

The Ultimata Server now supports both Windows and Linux/macOS platforms using Boost ASIO for high-performance networking.

## Quick Start

### Windows (Recommended)
```bash
# 1. Install dependencies with vcpkg
install.bat

# 2. Build the server
build.bat

# 3. Start the server
startit.bat
```

### Linux/macOS

#### Option 1: Using CMake (Recommended)
```bash
# 1. Install dependencies
# Ubuntu/Debian:
sudo apt-get install -y build-essential cmake libboost-all-dev libsdl2-dev libsdl2-net-dev

# macOS:
brew install boost sdl2 sdl2_net cmake

# 2. Build the server
mkdir -p build && cd build
cmake ..
make -j$(nproc)

# 3. Start the server
../start-server.sh
```

#### Option 2: Using Makefile
```bash
# 1. Install dependencies (same as above)

# 2. Build using the cross-platform Makefile
cp Makefile_cross_platform Makefile
make

# 3. Start the server
./start-server.sh
```

## Architecture

### Networking Backend
- **Primary**: Boost ASIO (high-performance, async I/O)

### Cross-Platform Features

#### Conditional Compilation
The code uses conditional compilation for platform-specific features:

```cpp
// Windows-specific networking definitions
#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601  // Windows 7 or later
#endif
#endif
```

#### Threading
- **Windows**: Uses native Windows threading via Boost
- **Linux/macOS**: Uses POSIX threads (`-pthread`)

#### Timing
- Cross-platform timing using `std::chrono::steady_clock`
- Consistent millisecond precision on all platforms

## Performance Benefits of ASIO

### vs SDL_net
| Feature | SDL_net | Boost ASIO |
|---------|---------|------------|
| **Max Connections** | ~100 | 1000+ |
| **I/O Model** | Blocking | Async/Non-blocking |
| **CPU Usage** | High (polling) | Low (event-driven) |
| **Scalability** | Limited | Excellent |
| **Error Handling** | Basic | Comprehensive |

### Connection Management
- **Asynchronous Accept**: New connections don't block the server
- **Automatic Cleanup**: Connections auto-close on errors
- **Connection Pooling**: Efficient ID reuse and memory management
- **Timeout Handling**: Configurable connection timeouts

## Development

### Project Structure
```
server/
├── src/
│   ├── AsioConnection.h/cpp      # Individual ASIO connections
│   ├── AsioConnectionManager.h/cpp # Connection pool management
│   ├── AsioNetworkManager.h/cpp   # Main ASIO network interface
│   ├── INetworkManager.h          # Abstract network interface
│   └── Main.cpp                   # Server entry point
├── CMakeLists.txt                 # Cross-platform CMake build
├── Makefile_cross_platform       # Alternative Linux/macOS build
├── start-server.bat              # Windows startup script
├── start-server.sh               # Linux/macOS startup script
└── vcpkg.json                    # Windows dependencies
```

### Adding Features
1. **New Packet Types**: Add to your protocol definition
2. **Connection Events**: Use the callback system in `AsioConnection`
3. **Custom Logic**: Implement in your game logic layer, not the network layer

### Debugging
- **Debug Builds**: Use `make debug` or CMake Debug configuration
- **Connection Logs**: Built-in logging shows connection events
- **Error Handling**: ASIO provides detailed error information

## Configuration

### Timeout Settings
```cpp
// Configure in AsioConnectionManager constructor
AsioConnectionManager(16, 30000);  // 16 max connections, 30s timeout
```

### Port Configuration
```cpp
// In your main server startup
networkManager->startServer(8099);  // Default port
```

## Troubleshooting

### Common Issues

#### "Boost not found" (Linux)
```bash
# Ubuntu/Debian
sudo apt-get install libboost-all-dev

# CentOS/RHEL
sudo yum install boost-devel
```

#### "SDL2 not found" (macOS)
```bash
# Install with Homebrew
brew install sdl2 sdl2_net

# Or MacPorts
sudo port install libsdl2 libsdl2_net
```

#### "Cannot bind to port" 
- Port 8099 might be in use
- Try a different port in `Main.cpp`
- Check firewall settings

### Performance Tuning

#### For High Load (1000+ players)
1. Increase connection limits in OS settings
2. Use multiple server instances with load balancing
3. Consider Redis for shared state
4. Profile with tools like `perf` (Linux) or Performance Toolkit (Windows)

## Future Enhancements

- **SSL/TLS Support**: Boost ASIO has built-in SSL support
- **IPv6 Support**: Already available in ASIO
- **WebSocket Support**: For web clients
- **Clustering**: Multiple server instances