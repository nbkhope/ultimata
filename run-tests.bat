@echo off
REM run-tests.bat – Run all automated tests on Windows.
REM
REM Usage:
REM   run-tests.bat         Run all test suites
REM   run-tests.bat python  Run only Python tests
REM   run-tests.bat ts      Run only TypeScript tests
REM   run-tests.bat cpp     Run only C++ tests (requires vcpkg + CMake)
REM
REM Requirements:
REM   - Python 3 (python3 or python in PATH)
REM   - Node.js + npm
REM   - CMake >= 3.21, vcpkg, and MSVC or MinGW (for C++ tests)

setlocal EnableDelayedExpansion

set "REPO_ROOT=%~dp0"
set "FILTER=%~1"
if "%FILTER%"=="" set "FILTER=all"

set OVERALL_PASS=1

echo.
echo ==========================================
echo    Ultimata – Test Runner (Windows)
echo ==========================================
echo.

REM ---------------------------------------------------------------------------
REM Python tests
REM ---------------------------------------------------------------------------
:run_python
if /i "%FILTER%"=="ts" goto run_ts
if /i "%FILTER%"=="cpp" goto run_cpp
if /i "%FILTER%"=="typescript" goto run_ts

echo [Python] Running pytest...
pushd "%REPO_ROOT%python_server"

python -m pytest --version >nul 2>&1
if errorlevel 1 (
    pip install -r requirements-dev.txt
)

python -m pytest -v
if errorlevel 1 (
    echo [FAIL] Python tests failed.
    set OVERALL_PASS=0
) else (
    echo [PASS] Python tests passed.
)
popd

if /i "%FILTER%"=="python" goto done

REM ---------------------------------------------------------------------------
REM TypeScript tests
REM ---------------------------------------------------------------------------
:run_ts
echo.
echo [TypeScript] Running vitest...
pushd "%REPO_ROOT%nodejs-client"

if not exist "node_modules" (
    npm install
)

npm test
if errorlevel 1 (
    echo [FAIL] TypeScript tests failed.
    set OVERALL_PASS=0
) else (
    echo [PASS] TypeScript tests passed.
)
popd

if /i "%FILTER%"=="ts" goto done
if /i "%FILTER%"=="typescript" goto done

REM ---------------------------------------------------------------------------
REM C++ tests
REM ---------------------------------------------------------------------------
:run_cpp
echo.
echo [C++] Running Google Test...
pushd "%REPO_ROOT%server"

if "%VCPKG_ROOT%"=="" (
    echo  WARNING: VCPKG_ROOT is not set - skipping C++ tests.
    echo           Set VCPKG_ROOT and re-run to include C++ tests.
    popd
    goto done
)

cmake --preset test -B build-tests -S . -DBUILD_TESTING=ON -Wno-dev
if errorlevel 1 (
    echo [FAIL] CMake configure failed.
    set OVERALL_PASS=0
    popd
    goto done
)

cmake --build build-tests --target ultimata-server-tests
if errorlevel 1 (
    echo [FAIL] C++ build failed.
    set OVERALL_PASS=0
    popd
    goto done
)

ctest --test-dir build-tests --output-on-failure
if errorlevel 1 (
    echo [FAIL] C++ tests failed.
    set OVERALL_PASS=0
) else (
    echo [PASS] C++ tests passed.
)
popd

:done
echo.
if "%OVERALL_PASS%"=="1" (
    echo All test suites passed!
    exit /b 0
) else (
    echo One or more test suites failed.
    exit /b 1
)
