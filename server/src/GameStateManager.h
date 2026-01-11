#pragma once

#include "MessageTypes.h"
#include <boost/asio/io_context.hpp>
#include <memory>
#include <concurrentqueue/moodycamel/blockingconcurrentqueue.h>

class GameStateManager {
private:
    boost::asio::io_context ioContext;
    bool running;
    moodycamel::BlockingConcurrentQueue<std::shared_ptr<Message>>& inboundQueue;
    moodycamel::BlockingConcurrentQueue<std::shared_ptr<Message>>& outboundQueue;

    void processIncomingMessages();
    void processNetworkMessage(const std::shared_ptr<Message>& msg);

public:
    GameStateManager(
        moodycamel::BlockingConcurrentQueue<std::shared_ptr<Message>>& inboundQueue,
        moodycamel::BlockingConcurrentQueue<std::shared_ptr<Message>>& outboundQueue
    );
    void update();
    void runGameLoop();
    void shutdown();
};
