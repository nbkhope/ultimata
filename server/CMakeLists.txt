cmake_minimum_required(VERSION 3.21)
project(ultimata-server)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
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

find_package(Boost CONFIG REQUIRED COMPONENTS system)
find_package(spdlog CONFIG REQUIRED)

# Create executable
add_executable(ultimata-server
    src/Main.cpp
    src/Connection.cpp
    src/ConnectionManager.cpp
    src/NetworkManager.cpp
    src/Orchestrator.cpp
    src/GameState.cpp
)

# Link libraries
target_link_libraries(ultimata-server
    PRIVATE
    Boost::system
    spdlog::spdlog
)

# Platform-specific library linking
if(WIN32)
    # Windows doesn't need additional libraries for this server
elseif(UNIX)
    target_link_libraries(ultimata-server
        PRIVATE
        Threads::Threads
    )
endif()

# Set output directory
set_target_properties(ultimata-server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)