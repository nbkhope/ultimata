#pragma once

#include "NetworkManager.h"
#include "GameStateManager.h"
#include "MessageTypes.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <thread>
#include <memory>
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
    moodycamel::BlockingConcurrentQueue<std::shared_ptr<Message>> inboundQueue;
    moodycamel::BlockingConcurrentQueue<std::shared_ptr<Message>> outboundQueue;

public:
    void waitForShutdownSignal();
    Orchestrator();
    ~Orchestrator();

    void run();
    void shutdown();
};
