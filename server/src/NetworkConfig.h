#include "INetworkManager.h"
#include <memory>

// Network configuration
enum class NetworkBackend {
    SDL_NET,
    BOOST_ASIO
};

class NetworkConfig {
public:
    static NetworkBackend getBackend() {
        // Could be read from config file, environment variable, etc.
        return NetworkBackend::SDL_NET;
    }
    
    static std::unique_ptr<INetworkManager> createNetworkManager() {
        switch (getBackend()) {
            case NetworkBackend::SDL_NET:
                return std::unique_ptr<INetworkManager>(createSDLNetManager());
            case NetworkBackend::BOOST_ASIO:
                // return std::unique_ptr<INetworkManager>(createBoostAsioManager());
                return std::unique_ptr<INetworkManager>(createSDLNetManager()); // Fallback
            default:
                return std::unique_ptr<INetworkManager>(createSDLNetManager());
        }
    }
};