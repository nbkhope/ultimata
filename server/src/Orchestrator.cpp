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
    networkManager = std::make_unique<NetworkManager>();

    spdlog::info("Network initialized successfully.");

    // Start server
    if (!networkManager->startServer(8099)) {
        throw std::runtime_error("Failed to start server: " + networkManager->getLastError());
    }

    // Start network thread
    networkThread = std::thread([this] {
        networkManager->runNetworkLoop();
    });

    spdlog::info("Network thread started");

    // Create and start game state in separate thread
    gameStateManager = std::make_unique<GameStateManager>();
    gameStateThread = std::thread([this] {
        gameStateManager->runGameLoop();
    });

    spdlog::info("Game state thread started");

    waitForShutdownSignal();
}

void Orchestrator::shutdown() {
    // if (shutdownRequested) {
    //     return;  // Already shut down
    // }

    spdlog::info("Orchestrator stopping...");

    gameStateManager->shutdown();
    // Stop the game state thread
    if (gameStateThread.joinable()) {
        gameStateThread.join();
    }

    // Explicitly shut down network manager before cleanup
    networkManager.reset();

    // Wait for network thread to finish
    if (networkThread.joinable()) {
        networkThread.join();
    }

    spdlog::info("Orchestrator stopped");
}
