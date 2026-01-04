# Multi-stage Dockerfile for Ultimata Server
# Stage 1: Build environment
FROM ubuntu:22.04 AS builder

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install basic build tools and vcpkg prerequisites
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    ninja-build \
    python3 \
    python3-pip \
    autoconf \
    autoconf-archive \
    automake \
    libtool \
    && rm -rf /var/lib/apt/lists/*

# Install vcpkg
RUN git clone https://github.com/Microsoft/vcpkg.git /opt/vcpkg && \
    cd /opt/vcpkg && \
    ./bootstrap-vcpkg.sh && \
    ln -s /opt/vcpkg/vcpkg /usr/local/bin/vcpkg

# Set vcpkg environment
ENV VCPKG_ROOT=/opt/vcpkg
ENV PATH="/opt/vcpkg:${PATH}"

# Create working directory
WORKDIR /app

# Copy vcpkg configuration first (for better Docker layer caching)
COPY vcpkg.json ./

# Install dependencies using vcpkg
RUN vcpkg install --triplet=x64-linux

# Copy source files
COPY src/ ./src/
COPY CMakeLists.txt ./

# Build the server using CMake with vcpkg toolchain
RUN mkdir -p build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release \
             -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake \
             -DVCPKG_TARGET_TRIPLET=x64-linux && \
    make -j$(nproc)

# Stage 2: Runtime environment
FROM ubuntu:22.04 AS runtime

# Install minimal runtime dependencies (only essential system libraries)
RUN apt-get update && apt-get install -y \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user for security
RUN useradd -m -u 1001 gameserver

# Create working directory and set ownership
WORKDIR /app
RUN chown gameserver:gameserver /app

# Copy built executable from builder stage
COPY --from=builder --chown=gameserver:gameserver /app/build/ultimata-server /app/

# Copy vcpkg shared libraries that the executable needs
COPY --from=builder --chown=gameserver:gameserver /opt/vcpkg/installed/x64-linux/lib/*.so* /usr/local/lib/
COPY --from=builder --chown=gameserver:gameserver /opt/vcpkg/installed/x64-linux/bin/* /usr/local/bin/ 2>/dev/null || true

# Update library path
RUN ldconfig

# Copy startup script
COPY --chown=gameserver:gameserver start-server.sh /app/
RUN chmod +x /app/start-server.sh

# Switch to non-root user
USER gameserver

# Expose the default game port
EXPOSE 8099

# Health check to verify server is running
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD netstat -an | grep -q :8099 || exit 1

# Set the default command
CMD ["./ultimata-server"]