@echo off
echo Starting Ultimata development environment...

REM Check if build directory exists and is configured
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

if not exist "build\CMakeCache.txt" (
    echo Configuring CMake for first time...
    REM Use the global vcpkg toolchain file
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/Users/nkans/git/microsoft/vcpkg/scripts/buildsystems/vcpkg.cmake
    if %ERRORLEVEL% NEQ 0 (
        echo CMake configuration failed!
        exit /b 1
    )
)

echo Building Ultimata...
cmake --build build --config Release
if %ERRORLEVEL% EQU 0 (
    echo Build successful, launching game...
    .\build\bin\Release\Ultimata.exe
) else (
    echo Build failed!
    exit /b 1
)