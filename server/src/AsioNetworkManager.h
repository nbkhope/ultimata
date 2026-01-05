#pragma once

// Windows-specific networking definitions
#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601  // Windows 7 or later
#endif
#endif

#include "INetworkManager.h"
#include "AsioConnectionManager.h"
#include <boost/asio.hpp>
#include <thread>
#include <atomic>

using boost::asio::ip::tcp;

class AsioNetworkManager : public INetworkManager {
private:
    boost::asio::io_context ioContext;
    std::unique_ptr<tcp::acceptor> acceptor;
    std::unique_ptr<AsioConnectionManager> connectionManager;
    std::thread networkThread;
    std::atomic<bool> running;
    std::string lastError;
    
    void runNetworkThread();
    void startAccept();
    void handleAccept(std::shared_ptr<AsioConnection> newConnection, 
                      const boost::system::error_code& error);

public:
    AsioNetworkManager();
    ~AsioNetworkManager();
    
    // INetworkManager interface
    bool initialize() override;
    void shutdown() override;
    
    bool startServer(uint16_t port) override;
    void stopServer() override;
    
    int acceptConnection() override;
    void closeConnection(int clientId) override;
    bool isConnectionActive(int clientId) override;
    std::vector<int> getActiveConnections() override;
    
    bool sendData(int clientId, const void* data, size_t size) override;
    int receiveData(int clientId, void* buffer, size_t maxSize) override;
    bool broadcastData(const void* data, size_t size) override;
    
    // Process all received messages in arrival order with a callback
    void processAllMessages(std::function<void(int clientId, const unsigned char* data, size_t size)> callback);
    
    void processEvents() override;
    int getMaxClients() const override;
    std::string getLastError() override;
};