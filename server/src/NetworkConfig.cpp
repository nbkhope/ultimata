#include "NetworkConfig.h"
#include "AsioNetworkManager.h"

INetworkManager* NetworkConfig::createAsioNetworkManager() {
    return new AsioNetworkManager();
}