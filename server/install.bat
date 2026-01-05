@echo off
setlocal enabledelayedexpansion

echo Installing vcpkg dependencies for server...
if not exist "%VCPKG_ROOT%" (
    echo Error: VCPKG_ROOT environment variable not set
    echo Please set VCPKG_ROOT to point to your vcpkg installation
    exit /b 1
)

echo Installing dependencies...
%VCPKG_ROOT%\vcpkg.exe install --triplet=x64-windows
if !ERRORLEVEL! NEQ 0 exit /b 1
