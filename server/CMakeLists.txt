cmake_minimum_required(VERSION 3.21)
project(ultimata-server)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to build tests (off by default so the main server build is unaffected)
option(BUILD_TESTING "Build the unit-test suite" OFF)

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
find_package(concurrentqueue CONFIG REQUIRED)

# Create executable
add_executable(ultimata-server
    src/Main.cpp
    src/Connection.cpp
    src/ConnectionManager.cpp
    src/NetworkManager.cpp
    src/Orchestrator.cpp
    src/GameStateManager.cpp
    src/MessageSerializer.cpp
)

# Link libraries
target_link_libraries(ultimata-server
    PRIVATE
    Boost::system
    spdlog::spdlog
    concurrentqueue::concurrentqueue
)

# Platform-specific library linking
if(WIN32)
    # Winsock provides htonl/ntohl and socket symbols on Windows.
    target_link_libraries(ultimata-server
        PRIVATE
        ws2_32
    )
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

# ---------------------------------------------------------------------------
# Unit tests (Google Test)
# Enable with:  cmake --preset default -DBUILD_TESTING=ON -B build-tests
# ---------------------------------------------------------------------------
if(BUILD_TESTING)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    include(GoogleTest)

    # Compile the serializer source as a library so tests can link against it
    # without pulling in server-specific dependencies (Boost, spdlog, etc.).
    add_library(ultimata-server-core OBJECT
        src/MessageSerializer.cpp
    )
    target_include_directories(ultimata-server-core PUBLIC src)

    # Test executable
    add_executable(ultimata-server-tests
        tests/test_message_serializer.cpp
        tests/test_message_protocol.cpp
    )

    target_link_libraries(ultimata-server-tests
        PRIVATE
        ultimata-server-core
        GTest::gtest
        GTest::gtest_main
    )

    if(WIN32)
        target_link_libraries(ultimata-server-tests
            PRIVATE
            ws2_32
        )
    endif()

    set_target_properties(ultimata-server-tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build-tests/bin
    )

    # Auto-discover tests with CTest
    gtest_discover_tests(ultimata-server-tests)
endif()