#pragma once

#include "NetworkManager.h"
#include "GameState.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <thread>

class Orchestrator {
private:
    boost::asio::io_context ioContext;
    boost::asio::signal_set signals;
    std::unique_ptr<NetworkManager> g_network;
    std::unique_ptr<GameState> gameState;
    std::thread gameStateThread;
    // bool shutdownRequested = false;

public:
    void waitForShutdownSignal();
    Orchestrator();
    ~Orchestrator();

    void run();
    void shutdown();
};
