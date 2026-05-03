# Ultimata – Testing Guide

This document explains how to run the automated test suites for the Ultimata project across **Windows**, **macOS**, and **Linux**.

---

## Quick Start

From the repository root, run **all** test suites in one command:

**macOS / Linux**
```bash
./run-tests.sh
```

**Windows (PowerShell or Command Prompt)**
```bat
run-tests.bat
```

> Run a specific suite: `./run-tests.sh python`, `./run-tests.sh ts`, or `./run-tests.sh cpp`

---

## Test Suites at a Glance

| Suite | Language | Framework | Location |
|-------|----------|-----------|----------|
| Server unit tests | C++ | [Google Test](https://github.com/google/googletest) | `server/tests/` |
| Python server tests | Python | [pytest](https://pytest.org) | `python_server/tests/` |
| Node.js client tests | TypeScript | [Vitest](https://vitest.dev) | `nodejs-client/tests/` |

---

## 1. Python Tests (pytest)

### Prerequisites
- Python 3.10+
- pip

### Install dependencies
```bash
cd python_server
pip install -r requirements-dev.txt
```

### Run tests

**macOS / Linux**
```bash
cd python_server
python3 -m pytest -v
```

**Windows**
```bat
cd python_server
python -m pytest -v
```

### Options

| Command | Description |
|---------|-------------|
| `pytest -v` | Verbose output |
| `pytest -k "TestGameServerShutdown"` | Run a specific test class |
| `pytest -k "test_shutdown"` | Run tests matching a pattern |
| `pytest --tb=short` | Shorter traceback format |

### VS Code
Open the Command Palette (`Ctrl+Shift+P` / `Cmd+Shift+P`) → **Tasks: Run Task** → **Run Python Tests**.

To debug, use the **Debug Python Tests** launch configuration in `.vscode/launch.json`.

---

## 2. TypeScript Tests (Vitest)

### Prerequisites
- Node.js 25+
- npm

### Install dependencies
```bash
cd nodejs-client
npm install
```

### Run tests

```bash
cd nodejs-client
npm test                 # Run once
npm run test:watch       # Watch mode (re-run on file changes)
npm run test:coverage    # With code coverage report
```

### Options

| Command | Description |
|---------|-------------|
| `npx vitest run` | Single run (same as `npm test`) |
| `npx vitest` | Interactive watch mode |
| `npx vitest run --reporter=verbose` | Verbose output |
| `npx vitest run tests/messageProtocol.test.ts` | Run a single test file |

### VS Code
Open the Command Palette → **Tasks: Run Task** → **Run TypeScript Tests**.

Use **Debug TypeScript Tests (vitest)** to step through tests in the debugger.

---

## 3. C++ Server Tests (Google Test)

### Prerequisites

| Dependency | Notes |
|------------|-------|
| CMake ≥ 3.21 | `cmake --version` |
| vcpkg | Must have `VCPKG_ROOT` set |
| C++23 compiler | MSVC 2022 / GCC 13+ / Clang 17+ |

Install the required vcpkg packages (first time only):
```bash
cd server
vcpkg install   # reads vcpkg.json automatically
```

### Configure & build

**macOS / Linux**
```bash
cd server
export VCPKG_ROOT=/path/to/vcpkg   # if not already set

cmake --preset test -B build-tests -S . -DBUILD_TESTING=ON
cmake --build build-tests --target ultimata-server-tests -j$(nproc)
```

**Windows (PowerShell)**
```powershell
cd server
$env:VCPKG_ROOT = "C:\vcpkg"   # if not already set

cmake --preset test -B build-tests -S . -DBUILD_TESTING=ON
cmake --build build-tests --target ultimata-server-tests
```

### Run tests

**macOS / Linux**
```bash
ctest --test-dir build-tests --output-on-failure
```

**Windows**
```powershell
ctest --test-dir build-tests --output-on-failure
```

Or run the test binary directly for more control:
```bash
./build-tests/bin/ultimata-server-tests          # macOS / Linux
.\build-tests\bin\Debug\ultimata-server-tests.exe  # Windows
```

### Google Test filter examples
```bash
# Run only serializer tests
./build-tests/bin/ultimata-server-tests --gtest_filter="MessageSerializer.*"

# Run a single test
./build-tests/bin/ultimata-server-tests --gtest_filter="MessageSerializer.SerializeChatMessage_RoundTrip"
```

### VS Code
Use the **Run C++ Tests (build & run)** task.
For debugging, use the **Debug C++ Tests** launch configuration appropriate for your OS.

---

## Continuous Integration

The test runner scripts are designed for use in CI pipelines. Example GitHub Actions step:

```yaml
- name: Run Python tests
  run: cd python_server && pip install -r requirements-dev.txt && python3 -m pytest -v

- name: Run TypeScript tests
  run: cd nodejs-client && npm ci && npm test
```

---

## Writing New Tests

### Python
Add a new `test_*.py` file in `python_server/tests/` and follow the `unittest.TestCase` pattern used in the existing tests. pytest will discover it automatically.

### TypeScript
Add a new `*.test.ts` file in `nodejs-client/tests/`. Vitest discovers files matching the `tests/**/*.test.ts` glob configured in `vitest.config.ts`.

### C++
Add a new `test_*.cpp` file in `server/tests/`. Then add it to the `ultimata-server-tests` target in `server/CMakeLists.txt`:

```cmake
add_executable(ultimata-server-tests
    tests/test_message_serializer.cpp
    tests/test_message_protocol.cpp
    tests/test_my_new_file.cpp   # <-- add here
)
```
