#pragma once

#include <boost/asio/io_context.hpp>

class GameStateManager {
private:
    boost::asio::io_context ioContext;
    bool running;
public:
    GameStateManager();
    void update();
    void runGameLoop();
    void shutdown();
};
