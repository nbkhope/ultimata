#include "Orchestrator.h"
#include <spdlog/spdlog.h>

Orchestrator::Orchestrator()
    : ioContext(),
      signals(ioContext, SIGINT, SIGTERM) {
}

Orchestrator::~Orchestrator() {
    shutdown();
}

void Orchestrator::waitForShutdownSignal() {
    signals.async_wait([&](const boost::system::error_code &ec, int signo) {
        if (ec) {
            spdlog::error("Signal wait error: {}", ec.message());
        } else {
            spdlog::info("Shutdown signal received: {}", signo);
        }
        // shutdownRequested = true;
        ioContext.stop();
    });

    ioContext.run();
}

void Orchestrator::run() {  // Renamed from start()
    spdlog::info("Orchestrator running...");

    // Create network manager
    g_network = std::make_unique<NetworkManager>();

    spdlog::info("Network initialized successfully.");

    // Start server (on a separate thread)
    // TODO: move thread logic here
    if (!g_network->startServer(8099)) {
        throw std::runtime_error("Failed to start server: " + g_network->getLastError());
    }

    // Create and start game state in separate thread
    gameState = std::make_unique<GameState>();
    gameStateThread = std::thread([this] {
        gameState->runGameLoop();
    });

    spdlog::info("Game state thread started");

    waitForShutdownSignal();
}

void Orchestrator::shutdown() {
    // if (shutdownRequested) {
    //     return;  // Already shut down
    // }

    spdlog::info("Orchestrator stopping...");

    gameState->shutdown();
    // Stop the game state thread
    if (gameStateThread.joinable()) {
        gameStateThread.join();
    }

    // Explicitly shut down network manager before cleanup
    g_network.reset();

    spdlog::info("Orchestrator stopped");
}
