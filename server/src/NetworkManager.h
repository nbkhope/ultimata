#pragma once

// Windows-specific networking definitions
#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601  // Windows 7 or later
#endif
#endif


#include "ConnectionManager.h"
#include "NetworkTask.h"
#include <boost/asio.hpp>
#include <thread>
#include <atomic>
#include <concurrentqueue/moodycamel/blockingconcurrentqueue.h>

using boost::asio::ip::tcp;

class NetworkManager {
private:
    boost::asio::io_context ioContext;
    std::unique_ptr<tcp::acceptor> acceptor;
    std::unique_ptr<ConnectionManager> connectionManager;
    std::atomic<bool> running;
    std::string lastError;
    moodycamel::BlockingConcurrentQueue<NetworkTask>& inboundQueue;
    moodycamel::BlockingConcurrentQueue<NetworkTask>& outboundQueue;

    void startAccept();
    void handleAccept(const std::shared_ptr<Connection>& newConnection,
                      const boost::system::error_code& error);

public:
    NetworkManager(
        moodycamel::BlockingConcurrentQueue<NetworkTask>& inboundQueue,
        moodycamel::BlockingConcurrentQueue<NetworkTask>& outboundQueue
    );
    ~NetworkManager();

    void runNetworkLoop();
    boost::asio::io_context& getIoContext();
    void shutdown();
    bool startServer(uint16_t port);
    void stopServer();
    int acceptConnection();
    void closeConnection(int clientId);
    bool isConnectionActive(int clientId);
    std::vector<int> getActiveConnections();
    bool sendData(int clientId, const void* data, size_t size);
    int receiveData(int clientId, void* buffer, size_t maxSize);
    bool broadcastData(const void* data, size_t size);
    void processAllMessages(const std::function<void(int clientId, const unsigned char* data, size_t size)>& callback);
    void processEvents();
    std::string getLastError();
};