# Docker Setup for Ultimata Server

This guide shows how to run the Ultimata Server in Docker containers for Linux testing and development.

## Quick Start

### Production Testing
```bash
# Build and start the server
docker-compose up --build

# Or run in background
docker-compose up -d --build

# Check logs
docker-compose logs -f ultimata-server

# Stop the server
docker-compose down
```

### Development Setup
```bash
# Start development environment
docker-compose -f docker-compose.dev.yml up -d --build

# Enter the container for development
docker exec -it ultimata-server-dev bash

# Inside the container, build and run
cd /app
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
./ultimata-server
```

## Testing the Server

### Basic Connectivity Test
```bash
# Run the test script (from host)
chmod +x test-server.sh
./test-server.sh localhost 8099

# Or test manually
telnet localhost 8099
# or
nc localhost 8099
```

### Using the Test Client Container
```bash
# Start with test client
docker-compose --profile testing up -d

# Connect to test client
docker exec -it ultimata-test-client bash

# Inside test client, test the server
apt-get update && apt-get install -y telnet netcat-traditional
telnet ultimata-server 8099
```

## Container Configurations

### Production Container (`docker-compose.yml`)
- **Multi-stage build**: Smaller runtime image
- **Non-root user**: Enhanced security
- **Resource limits**: CPU and memory constraints
- **Health checks**: Automatic monitoring
- **Restart policy**: Automatic restart on failure

### Development Container (`docker-compose.dev.yml`)
- **Source mounting**: Live code editing
- **Debug tools**: gdb, valgrind, strace
- **Development libraries**: All build dependencies
- **No resource limits**: Full development power

## Environment Variables

| Variable | Default | Description |
|----------|---------|-------------|
| `SERVER_PORT` | 8099 | Port for game server |
| `MAX_PLAYERS` | 100 | Maximum concurrent players |
| `LOG_LEVEL` | INFO | Logging level (DEBUG/INFO/WARN/ERROR) |

## Docker Commands Reference

### Building
```bash
# Build production image
docker build -t ultimata-server .

# Build development image  
docker build -f Dockerfile.dev -t ultimata-server-dev .
```

### Running
```bash
# Run production container
docker run -d -p 8099:8099 --name ultimata ultimata-server

# Run development container with source mount
docker run -d -p 8099:8099 -v $(pwd)/src:/app/src ultimata-server-dev
```

### Management
```bash
# View logs
docker logs ultimata-server
docker-compose logs -f

# Enter running container
docker exec -it ultimata-server bash

# Stop and remove
docker stop ultimata-server
docker rm ultimata-server

# Or with compose
docker-compose down
```

### Monitoring
```bash
# Check container stats
docker stats ultimata-server

# Check health status
docker ps --format "table {{.Names}}\t{{.Status}}\t{{.Ports}}"

# View resource usage
docker system df
```

## Troubleshooting

### Common Issues

#### Port Already in Use
```bash
# Check what's using the port
netstat -tlnp | grep :8099
# or
lsof -i :8099

# Use different port
docker run -p 8100:8099 ultimata-server
```

#### Build Failures
```bash
# Clean build
docker system prune -a
docker-compose build --no-cache

# Check build logs
docker-compose build --progress=plain
```

#### Connection Issues
```bash
# Check if container is running
docker ps

# Check container logs
docker logs ultimata-server

# Test network connectivity
docker network ls
docker network inspect ultimata_ultimata-network
```

#### Permission Issues (Development)
```bash
# Fix file permissions
sudo chown -R $USER:$USER build/
sudo chown -R $USER:$USER logs/
```

## Performance Testing

### Load Testing with Multiple Containers
```bash
# Scale up for load testing
docker-compose up --scale ultimata-server=3

# Use nginx for load balancing
# (requires additional nginx container setup)
```

### Resource Monitoring
```bash
# Monitor resource usage
docker stats

# Check container resource limits
docker inspect ultimata-server | grep -A 10 "Resources"

# System resource usage
htop
iostat 1
```

## Networking

### Port Mapping
- **Host:Container** - `8099:8099`
- **Custom ports** - Modify in docker-compose.yml
- **Multiple instances** - Use different host ports

### Network Architecture
```
Host (Windows/macOS)
├── Port 8099 → Docker Container
│   ├── ultimata-server (Linux)
│   └── Network: ultimata-network (172.20.0.0/16)
└── Additional containers in same network
```

### Firewall Considerations
```bash
# Allow Docker ports (Linux host)
sudo ufw allow 8099
sudo iptables -I INPUT -p tcp --dport 8099 -j ACCEPT
```

## Integration with Development Workflow

### Code Changes
1. Edit source files on host
2. Container automatically sees changes (dev setup)
3. Rebuild inside container: `make -j$(nproc)`
4. Test changes immediately

### Debugging
```bash
# Run with debugger
docker exec -it ultimata-server-dev gdb ./ultimata-server

# Memory leak detection
docker exec -it ultimata-server-dev valgrind ./ultimata-server

# System call tracing
docker exec -it ultimata-server-dev strace ./ultimata-server
```

This Docker setup provides a complete Linux testing environment for your cross-platform server development!