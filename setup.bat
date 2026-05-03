@echo off
REM setup.bat — Windows launcher for setup.ps1
REM
REM Usage:
REM   setup.bat                  (interactive menu)
REM   setup.bat server
REM   setup.bat client
REM   setup.bat python-server
REM   setup.bat nodejs-client
REM
REM This file simply invokes setup.ps1 via PowerShell with the correct
REM execution policy so that users do not need to configure anything first.

setlocal

set "SCRIPT_DIR=%~dp0"
set "PS_SCRIPT=%SCRIPT_DIR%setup.ps1"
set "COMPONENT=%~1"

where powershell >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: PowerShell is not available. Please install it from:
    echo   https://aka.ms/powershell
    pause
    exit /b 1
)

if "%COMPONENT%"=="" (
    powershell -NoProfile -ExecutionPolicy Bypass -File "%PS_SCRIPT%"
) else (
    powershell -NoProfile -ExecutionPolicy Bypass -File "%PS_SCRIPT%" -Component "%COMPONENT%"
)

exit /b %ERRORLEVEL%
