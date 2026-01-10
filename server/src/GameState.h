#pragma once

#include <boost/asio/io_context.hpp>

class GameState {
private:
    boost::asio::io_context ioContext;
    bool running;
public:
    GameState();
    void update();
    void runGameLoop();
    void shutdown();
};
