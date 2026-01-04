@echo off
echo Starting Ultimata Server...
cd /d %~dp0
if exist build\ultimata-server.exe (
    build\ultimata-server.exe
) else if exist build\Release\ultimata-server.exe (
    build\Release\ultimata-server.exe
) else (
    echo Server executable not found! Run build.bat first.
    pause
)