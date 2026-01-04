cmake_minimum_required(VERSION 3.21)
project(ultimata-server)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_net CONFIG REQUIRED)

# Create executable
add_executable(ultimata-server
    src/Main.cpp
)

# Link libraries
target_link_libraries(ultimata-server
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_net::SDL2_net>,SDL2_net::SDL2_net,SDL2_net::SDL2_net-static>
)

# Set output directory
set_target_properties(ultimata-server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)