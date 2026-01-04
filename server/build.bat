@echo off
echo Configuring CMake...
cmake --preset=default -B build

echo Building server...
cmake --build build --config Release

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo Build complete! Server executable is in build/ultimata-server.exe
