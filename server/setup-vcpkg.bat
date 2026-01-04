@echo off
echo Installing vcpkg dependencies for server...
if not exist "%VCPKG_ROOT%" (
    echo Error: VCPKG_ROOT environment variable not set
    echo Please set VCPKG_ROOT to point to your vcpkg installation
    pause
    exit /b 1
)

echo Installing dependencies...
%VCPKG_ROOT%\vcpkg.exe install --triplet=x64-windows

echo Configuring CMake...
cmake --preset=default -B build

echo Building server...
cmake --build build --config Release

echo Build complete! Server executable is in build/ultimata-server.exe
pause