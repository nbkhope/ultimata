#pragma once

#include "NetworkManager.h"
#include "GameStateManager.h"
#include "NetworkTask.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <thread>
#include <concurrentqueue/moodycamel/blockingconcurrentqueue.h>

class Orchestrator {
private:
    boost::asio::io_context ioContext;
    boost::asio::signal_set signals;
    std::unique_ptr<NetworkManager> networkManager;
    std::unique_ptr<GameStateManager> gameStateManager;
    std::thread networkThread;
    std::thread gameStateThread;
    // bool shutdownRequested = false;
    moodycamel::BlockingConcurrentQueue<NetworkTask> inboundQueue;
    moodycamel::BlockingConcurrentQueue<NetworkTask> outboundQueue;

public:
    void waitForShutdownSignal();
    Orchestrator();
    ~Orchestrator();

    void run();
    void shutdown();
};
