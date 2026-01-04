#include "NetworkConfig.h"
#include "AsioNetworkManager.h"

// Forward declaration for SDL manager
extern "C" INetworkManager* createSDLNetworkManager();

INetworkManager* NetworkConfig::createSDLNetworkManager() {
    return ::createSDLNetworkManager();
}

INetworkManager* NetworkConfig::createAsioNetworkManager() {
    return new AsioNetworkManager();
}