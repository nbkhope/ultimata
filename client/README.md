# Ultimata Client

A simple 2D RPG game built with SDL2.

## Modern Build System

This project now uses **vcpkg** for dependency management and **CMake** for building, providing a modern, cross-platform development experience.

## Prerequisites

- **CMake** 3.20 or higher
- **vcpkg** (C++ package manager)
- **Git**
- A C++17 compatible compiler (GCC, Clang, or MSVC)

## Setup Instructions

### 1. Install vcpkg

#### Windows:
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

#### macOS/Linux:
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

### 2. Install Dependencies

vcpkg will automatically install the dependencies listed in `vcpkg.json`:
- SDL2
- SDL2_image
- SDL2_ttf
- SDL2_mixer
- SDL2_net

### 3. Build the Project

#### Windows:
```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

#### macOS/Linux:
```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
make -j$(nproc)
```

### 4. Run the Game

The built executable will be in `build/bin/Ultimata` (or `build/bin/Ultimata.exe` on Windows).

## IDE Support

This project works with all modern IDEs:
- **Visual Studio**: Open the folder directly
- **VS Code**: Install CMake Tools extension
- **CLion**: Open CMakeLists.txt
- **Qt Creator**: Open CMakeLists.txt

## Migration from Makefile

The old makefile-based build system has been replaced with:
- `vcpkg.json` - Dependency manifest (replaces manual library linking)
- `CMakeLists.txt` - Build configuration (replaces makefile)
- `.gitignore` - Ignores build artifacts

## Development

### Debug Build:
```bash
cmake --build . --config Debug
```

### Release Build:
```bash
cmake --build . --config Release
```

### Clean Build:
```bash
rm -rf build  # or rmdir /s build on Windows
```

## Dependencies

All dependencies are managed through vcpkg:
- **SDL2**: Core graphics and input library
- **SDL2_image**: Image loading (PNG, JPEG, etc.)
- **SDL2_ttf**: TrueType font rendering
- **SDL2_mixer**: Audio mixing and playback
- **SDL2_net**: Network communication

## Cross-Platform Support

This build system works identically on:
- ✅ Windows (MSVC, MinGW)
- ✅ macOS (Clang)
- ✅ Linux (GCC, Clang)