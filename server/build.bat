@echo off

echo Configuring CMake...
cmake --preset=default -B build
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake configuration failed
    exit /b 1
)

echo Building server...
cmake --build build --config Release
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Build failed
    exit /b 1
)

echo Build complete! Server executable is in build/ultimata-server.exe
