#pragma once

#include <vector>

// Task structure for communication between network and game state threads
struct NetworkTask {
    int clientId;
    std::vector<unsigned char> data;
};
