@echo off
echo Building dummy client...
g++ -IC:\Users\nkans\include -LC:\Users\nkans\lib -o dummy_client.exe dummy_client.cpp -lSDL2main -lSDL2 -lSDL2_net

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b 1
)

echo Build successful!
echo.
echo Usage examples:
echo   dummy_client.exe                              ^(default: DummyPlayer at 100,100^)
echo   dummy_client.exe Alice                        ^(Alice at 100,100^)
echo   dummy_client.exe Bob 200 150                  ^(Bob at 200,150^)
echo   dummy_client.exe Carol 300 250 1000          ^(Carol at 300,250, moves every 1000ms^)
echo.
echo You can run multiple instances with different names and positions:
echo   start dummy_client.exe Alice 100 100
echo   start dummy_client.exe Bob 200 200  
echo   start dummy_client.exe Carol 300 100
echo.
pause