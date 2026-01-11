#pragma once

#include "NetworkTask.h"
#include <boost/asio/io_context.hpp>
#include <concurrentqueue/moodycamel/blockingconcurrentqueue.h>

class GameStateManager {
private:
    boost::asio::io_context ioContext;
    bool running;
    moodycamel::BlockingConcurrentQueue<NetworkTask>& inboundQueue;
    moodycamel::BlockingConcurrentQueue<NetworkTask>& outboundQueue;
public:
    GameStateManager(
        moodycamel::BlockingConcurrentQueue<NetworkTask>& inboundQueue,
        moodycamel::BlockingConcurrentQueue<NetworkTask>& outboundQueue
    );
    void update();
    void runGameLoop();
    void shutdown();
};
