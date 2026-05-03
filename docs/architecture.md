# Ultimata – Architecture Overview

> This document provides a high-level overview of the Ultimata project for
> developers and AI assistants. Keep it up-to-date as the architecture evolves.

---

## Project Summary

**Ultimata** is a 2-D multiplayer role-playing game inspired by
[Tibia](https://www.tibia.com) and the [Final Fantasy](https://en.wikipedia.org/wiki/Final_Fantasy)
series. It is built with SDL 2 for the client and Boost.Asio for the server.

---

## Repository Layout

```
ultimata/
├── client/           # C++ SDL2 game client
│   ├── src/          # Game source files
│   ├── data/         # Tilesets, sounds, fonts
│   ├── CMakeLists.txt
│   └── vcpkg.json
│
├── server/           # C++ multiplayer game server
│   ├── src/          # Server source files
│   ├── tests/        # Google Test unit tests
│   ├── CMakeLists.txt
│   ├── CMakePresets.json
│   └── vcpkg.json
│
├── python_server/    # Python test/stub server
│   ├── simple_server.py
│   ├── tests/        # pytest unit tests
│   ├── pytest.ini
│   └── requirements-dev.txt
│
├── python_client/    # Python test client
│   └── test_client.py
│
├── nodejs-client/    # TypeScript/Node.js game client
│   ├── index.ts
│   ├── messageProtocol.ts
│   ├── vitest.config.ts
│   └── tests/        # Vitest unit tests
│
├── dummy_clients/    # Minimal test clients
├── tests/            # Integration / end-to-end test scripts
│   ├── test_multiplayer.sh
│   └── test_with_dummies.sh
│
├── docs/             # Project documentation
│   ├── architecture.md  ← you are here
│   ├── testing.md
│   └── Documentation.md
│
├── .vscode/          # VS Code workspace settings
│   ├── launch.json   # Debug configurations
│   ├── tasks.json    # Build & test tasks
│   └── settings.json
│
├── run-tests.sh      # One-command test runner (macOS/Linux)
└── run-tests.bat     # One-command test runner (Windows)
```

---

## C++ Server (`server/`)

### Technology Stack

| Component | Library / Tool |
|-----------|----------------|
| Language  | C++23 |
| Build system | CMake 3.21+ with vcpkg |
| Networking | Boost.Asio (async TCP) |
| Logging | spdlog |
| Message queues | moodycamel/ConcurrentQueue (lock-free) |
| Unit testing | Google Test (gtest) |

### Architecture

```
Main.cpp
  └── Orchestrator
        ├── NetworkManager          (Boost.Asio I/O thread)
        │     └── Connection × N   (one per TCP client)
        └── GameStateManager        (game loop thread)

Shared queues (lock-free):
  inboundQueue   NetworkManager → GameStateManager
  outboundQueue  GameStateManager → NetworkManager
```

### Key Source Files

| File | Responsibility |
|------|----------------|
| `Main.cpp` | Entry point, creates Orchestrator |
| `Orchestrator.cpp/.h` | Owns all subsystems, handles SIGINT/SIGTERM |
| `NetworkManager.cpp/.h` | Accepts TCP connections, reads/writes bytes |
| `Connection.cpp/.h` | Per-client connection, async read/write |
| `ConnectionManager.cpp/.h` | Tracks active connections, broadcasting |
| `GameStateManager.cpp/.h` | Game loop, processes inbound messages |
| `MessageSerializer.cpp/.h` | Serialize/deserialize game messages |
| `MessageTypes.h` | Struct definitions for all message types |
| `MessageProtocol.h` | Low-level binary framing helpers |
| `ThreadSafeQueue.h` | Lock-free queue wrapper |

### Message Protocol

Messages are little-endian binary packets with no length prefix (the type byte
determines the expected length):

```
[type: uint8] [payload: varies]
```

Message types (defined in `MessageTypes.h`):

| Value | Name | Direction | Payload |
|-------|------|-----------|---------|
| 1 | PlayerMove | Client → Server | playerId(i32) x(f32) y(f32) direction(f32) |
| 2 | ChatMessage | Client → Server | playerId(i32) text(str) |
| 10 | PlayerJoined | Server → Client | playerId(i32) playerName(str) x(f32) y(f32) |
| 11 | PlayerLeft | Server → Client | playerId(i32) |
| 12 | GameStateUpdate | Server → Client | *(future)* |
| 13 | ChatBroadcast | Server → Client | fromPlayerId(i32) playerName(str) text(str) |
| 20 | Ping | Bidirectional | *(none)* |
| 21 | Pong | Bidirectional | *(none)* |

Strings are length-prefixed: `length(int32) data(bytes)`.

---

## C++ Client (`client/`)

### Technology Stack

| Component | Library / Tool |
|-----------|----------------|
| Language | C++17 |
| Build system | CMake 3.20+ with vcpkg |
| Graphics | SDL2, SDL2_image, SDL2_ttf |
| Audio | SDL2_mixer |
| Networking | SDL2_net |

### Key Source Files

| File | Responsibility |
|------|----------------|
| `Main.cpp` / `Main.h` | SDL initialisation and main game loop |
| `GameMap.cpp/.h` | World map, tile rendering |
| `Creature.cpp/.h` | Player/NPC base class |
| `Monster.cpp/.h` | Monster behaviour |
| `Graphics.cpp/.h` | Rendering utilities |
| `Input.cpp/.h` | Keyboard/mouse handling |
| `Sound.cpp/.h` | Audio playback |
| `Camera.cpp/.h` | Viewport / scrolling |
| `Chat.h` / `ChatOverlay.cpp/.h` | In-game chat overlay |
| `EquipmentOverlay.cpp/.h` | Equipment/inventory UI |

---

## Python Server (`python_server/`)

A lightweight Python echo server used for manual testing of the C++ and
Node.js clients without needing to build the full C++ server.

- Listens on port **8099** (same as the C++ server)
- Sends a 4-byte big-endian welcome integer (`12345`) on connect
- Echoes all received data back to the sender

---

## Node.js Client (`nodejs-client/`)

A TypeScript client used for integration/compatibility testing. It connects
to the game server and exercises the message protocol.

Key file: `messageProtocol.ts` – implements `MessageBuilder`, `MessageReader`,
and `MessageParser` (streaming parser that handles partial receives).

---

## Testing Architecture

See [`docs/testing.md`](testing.md) for full instructions.

| Suite | Framework | What is tested |
|-------|-----------|----------------|
| `server/tests/` | Google Test | `MessageSerializer`, `MessageProtocol` |
| `python_server/tests/` | pytest | `GameServer` (init, shutdown, client handling) |
| `nodejs-client/tests/` | Vitest | `MessageBuilder`, `MessageReader`, `MessageParser` |

---

## Running the Project

### One-command setup (recommended)

After cloning the repository, a single script installs all prerequisites,
builds the chosen component, and starts it — no manual configuration needed.

**macOS / Linux**
```bash
./setup.sh [server|client|python-server|nodejs-client]
```

**Windows** (Command Prompt or PowerShell)
```bat
setup.bat [server|client|python-server|nodejs-client]
```
or directly in PowerShell:
```powershell
.\setup.ps1 [server|client|python-server|nodejs-client]
```

Omit the component name to see an interactive menu.

The scripts handle:
- Installing missing system packages (via Homebrew / apt / dnf / winget / Chocolatey)
- Cloning and bootstrapping vcpkg (first run only, placed in `~/.local/share/vcpkg`)
- Installing vcpkg and npm/pip dependencies
- Configuring and building CMake projects
- Starting the selected component

### Docker (server only)
```bash
cd server
docker compose up --build
```

### Manual build (advanced)

If you prefer full control, see [`docs/testing.md`](testing.md) for the
individual build commands for each language.

---

## Networking

The server binds to `0.0.0.0:8099` by default. Clients connect via TCP and
exchange binary messages using the protocol described above. The Python server
and Node.js client are alternative implementations of the same protocol.

---

## Future Work / TODOs

- Real player-name persistence (currently constructed as `Player{id}`)
- `GameStateUpdate` message implementation
- Authentication / session management
- Unit tests for `GameStateManager` and `ConnectionManager`
- Client-side unit tests for SDL rendering components
