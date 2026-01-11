#include "GameStateManager.h"
#include "MessageTypes.h"
#include "MessageSerializer.h"
#include <spdlog/spdlog.h>

GameStateManager::GameStateManager(
    moodycamel::BlockingConcurrentQueue<std::shared_ptr<Message>>& inboundQueue,
    moodycamel::BlockingConcurrentQueue<std::shared_ptr<Message>>& outboundQueue
)
    : ioContext(),
      inboundQueue(inboundQueue),
      outboundQueue(outboundQueue) {
}

void GameStateManager::update() {
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
void GameStateManager::runGameLoop() {
    running = true;
    spdlog::info("Game loop started");

    while (running) {
        // Process all incoming network messages
        processIncomingMessages();

        // Update game state
        update();

        // Sleep for ~60 FPS (16.67ms per frame)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    spdlog::info("Game loop ended");
}

void GameStateManager::shutdown() {
    running = false;
    spdlog::info("GameStateManager shutdown initiated");
}

void GameStateManager::processIncomingMessages() {
    std::shared_ptr<Message> msg;
    while (inboundQueue.try_dequeue(msg)) {
        processNetworkMessage(msg);
    }
}

void GameStateManager::processNetworkMessage(const std::shared_ptr<Message>& msg) {
    if (!msg) {
        spdlog::warn("Received null message");
        return;
    }

    try {
        switch (msg->type) {
            case MessageType::PlayerMove: {
                auto* moveMsg = dynamic_cast<PlayerMoveMessage*>(msg.get());
                if (moveMsg) {
                    spdlog::info("Player {} moved to ({}, {}), direction: {}",
                                 moveMsg->playerId, moveMsg->x, moveMsg->y, moveMsg->direction);

                    // TODO: Update game state with new player position
                    // gameWorld.updatePlayerPosition(moveMsg->playerId, moveMsg->x, moveMsg->y);
                }
                break;
            }

            case MessageType::ChatMessage: {
                auto* chatMsg = dynamic_cast<ChatMessageData*>(msg.get());
                if (chatMsg) {
                    spdlog::info("Chat from player {}: {}", chatMsg->playerId, chatMsg->text);

                    // Broadcast chat to all clients
                    auto broadcast = std::make_shared<ChatBroadcastMessage>();
                    broadcast->fromPlayerId = chatMsg->playerId;
                    broadcast->playerName = "Player" + std::to_string(chatMsg->playerId); // TODO: Get real name
                    broadcast->text = chatMsg->text;
                    broadcast->clientId = -1; // -1 means broadcast to all

                    outboundQueue.enqueue(broadcast);
                }
                break;
            }

            default:
                spdlog::warn("Unknown message type from client {}: {}",
                             msg->clientId, static_cast<int>(msg->type));
                break;
        }

    } catch (const std::exception& e) {
        spdlog::error("Error processing message from client {}: {}", msg->clientId, e.what());
    }
}
