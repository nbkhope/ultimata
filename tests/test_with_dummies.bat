@echo off
echo === Ultimata Multiplayer Test with Dummy Clients ===
echo.

REM Build dummy client first
echo Building dummy client...
g++ -IC:\Users\nkans\include -LC:\Users\nkans\lib -o dummy_client.exe dummy_client.cpp -lSDL2main -lSDL2 -lSDL2_net

if %ERRORLEVEL% neq 0 (
    echo Failed to build dummy client!
    pause
    exit /b 1
)

echo ✓ Dummy client built successfully
echo.

REM Start server in background
echo Starting server...
cd server
start "Ultimata Server" server.exe
echo ✓ Server started
cd ..

REM Wait for server to start
echo Waiting for server to initialize...
timeout /t 3 /nobreak > nul

REM Start dummy clients
echo Starting dummy clients...

start "Alice" dummy_client.exe Alice 150 100
echo ✓ Started Alice at (150,100)

start "Bob" dummy_client.exe Bob 250 200
echo ✓ Started Bob at (250,200)

start "Carol" dummy_client.exe Carol 350 150
echo ✓ Started Carol at (350,150)

echo.
echo === Test Setup Complete ===
echo Server and 3 dummy clients are running in separate windows
echo.
echo Now start a real client with: .\build\Ultimata.exe
echo You should see Alice, Bob, and Carol moving around as blue rectangles!
echo.
echo To stop everything: Close all the terminal windows that opened
echo.
pause