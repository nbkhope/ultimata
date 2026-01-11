#pragma once

#include "NetworkManager.h"
#include "GameStateManager.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <thread>
#include <concurrentqueue/blockingconcurrentqueue.h>

class Orchestrator {
private:
    boost::asio::io_context ioContext;
    boost::asio::signal_set signals;
    std::unique_ptr<NetworkManager> networkManager;
    std::unique_ptr<GameStateManager> gameStateManager;
    std::thread networkThread;
    std::thread gameStateThread;
    // bool shutdownRequested = false;
    moodycamel::BlockingConcurrentQueue<NetworkTask> inbound;
    moodycamel::BlockingConcurrentQueue<NetworkTask> outbound;

public:
    void waitForShutdownSignal();
    Orchestrator();
    ~Orchestrator();

    void run();
    void shutdown();
};
