cmake_minimum_required(VERSION 3.21)
project(ultimata-server)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific configurations
if(WIN32)
    # Windows-specific settings
    add_definitions(-D_WIN32_WINNT=0x0601)  # Windows 7 support
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(UNIX AND NOT APPLE)
    # Linux-specific settings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    find_package(Threads REQUIRED)
elseif(APPLE)
    # macOS-specific settings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    find_package(Threads REQUIRED)
endif()

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_net CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

# Create executable
add_executable(ultimata-server
    src/Main.cpp
    src/SDLNetManager.cpp
    src/Connection.cpp
    src/ConnectionManager.cpp
    src/AsioConnection.cpp
    src/AsioConnectionManager.cpp
    src/AsioNetworkManager.cpp
    src/NetworkConfig.cpp
)

# Link libraries
target_link_libraries(ultimata-server
    PRIVATE
    Boost::system
)

# Platform-specific library linking
if(WIN32)
    target_link_libraries(ultimata-server
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_net::SDL2_net>,SDL2_net::SDL2_net,SDL2_net::SDL2_net-static>
    )
elseif(UNIX)
    target_link_libraries(ultimata-server
        PRIVATE
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_net::SDL2_net>,SDL2_net::SDL2_net,SDL2_net::SDL2_net-static>
        Threads::Threads
    )
endif()

# Set output directory
set_target_properties(ultimata-server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)