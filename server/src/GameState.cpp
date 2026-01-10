#include "GameState.h"
#include <spdlog/spdlog.h>

GameState::GameState()
    : ioContext() {
}

void GameState::update() {
    // Game state update logic here
    // This runs at ~60 FPS in the game thread

    // Example: Update monster positions, process game events, etc.
    // For now, just a placeholder
    // static int frameCount = 0;
    // frameCount++;

    // You can add actual game logic here later
    // processMonsters();
    // updatePhysics();
    // handleGameEvents();
}

// TODO: do we have to use async io here?
void GameState::runGameLoop() {
    running = true;
    spdlog::info("Game loop started");

    while (running) {
        spdlog::debug("Game loop tick");
        // if (gameState) {
        //     gameState->update();
        // }

        // Sleep for ~60 FPS (16.67ms per frame)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    spdlog::info("Game loop ended");
}

void GameState::shutdown() {
    running = false;
    spdlog::info("GameState shutdown initiated");
}
