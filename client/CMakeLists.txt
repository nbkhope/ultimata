cmake_minimum_required(VERSION 3.20)
project(ultimata-client)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages using vcpkg
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_net CONFIG REQUIRED)

# Collect all source files
file(GLOB_RECURSE SOURCES 
    src/*.cpp
    src/*.h
)

# Create executable
add_executable(Ultimata ${SOURCES})

# Link SDL2 libraries
target_link_libraries(Ultimata 
    PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2-static>
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
    SDL2_net::SDL2_net
)

# Set output directory
set_target_properties(Ultimata PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy data directory to build output
add_custom_command(TARGET Ultimata POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data
    ${CMAKE_BINARY_DIR}/bin/data
    COMMENT "Copying data directory to build output"
)

# On Windows, copy SDL2 DLLs to output directory
if(WIN32)
    add_custom_command(TARGET Ultimata POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:Ultimata>
        COMMENT "Copying SDL2.dll to output directory"
    )
endif()

# Enable debug symbols in debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        target_compile_options(Ultimata PRIVATE /Zi)
    else()
        target_compile_options(Ultimata PRIVATE -g)
    endif()
endif()

# Set warning levels
if(MSVC)
    target_compile_options(Ultimata PRIVATE /W4)
else()
    target_compile_options(Ultimata PRIVATE -Wall -Wextra -Wpedantic)
endif()