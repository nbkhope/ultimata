#!/usr/bin/env bash
# =============================================================================
# setup.sh — One-command setup & run for the Ultimata project.
#
# Clones, installs dependencies, builds, and starts a component.
# Run this script immediately after cloning the repository.
#
# Usage:
#   ./setup.sh                  # interactive: asks which component to run
#   ./setup.sh server           # C++ multiplayer server
#   ./setup.sh client           # C++ SDL2 game client
#   ./setup.sh python-server    # Python stub/test server
#   ./setup.sh nodejs-client    # TypeScript/Node.js client
#
# Requirements installed automatically when missing:
#   macOS  : Homebrew, cmake, git, node, python3
#   Linux  : apt / dnf / pacman + cmake, git, nodejs, python3
#   C++ targets also need vcpkg (bootstrapped automatically).
# =============================================================================
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
COMPONENT="${1:-}"

# ---------------------------------------------------------------------------
# Colour helpers
# ---------------------------------------------------------------------------
RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
CYAN='\033[0;36m'; BOLD='\033[1m'; NC='\033[0m'

info()    { echo -e "${CYAN}▶ $*${NC}"; }
success() { echo -e "${GREEN}✓ $*${NC}"; }
warn()    { echo -e "${YELLOW}⚠  $*${NC}"; }
error()   { echo -e "${RED}✗ $*${NC}" >&2; exit 1; }
header()  { echo -e "\n${BOLD}═══ $* ═══${NC}\n"; }

# ---------------------------------------------------------------------------
# Platform detection
# ---------------------------------------------------------------------------
detect_os() {
    case "$(uname -s)" in
        Darwin) echo "macos" ;;
        Linux)  echo "linux" ;;
        CYGWIN*|MINGW*|MSYS*) echo "windows" ;;
        *) error "Unsupported platform: $(uname -s)" ;;
    esac
}
OS=$(detect_os)

detect_pkg_manager() {
    if   command -v apt-get &>/dev/null; then echo "apt"
    elif command -v dnf     &>/dev/null; then echo "dnf"
    elif command -v pacman  &>/dev/null; then echo "pacman"
    elif command -v zypper  &>/dev/null; then echo "zypper"
    else echo "unknown"
    fi
}

# ---------------------------------------------------------------------------
# Privilege helper: run a command with sudo (prompts once if needed)
# ---------------------------------------------------------------------------
need_sudo() {
    if [[ $EUID -eq 0 ]]; then
        "$@"
    else
        warn "Elevated privileges required. You may be prompted for your password."
        sudo "$@"
    fi
}

# ---------------------------------------------------------------------------
# Install a system package if a command is not found
# ---------------------------------------------------------------------------
ensure_cmd() {
    local cmd="$1"; shift           # command to test
    local pkg="${1:-$cmd}"; shift || true   # package name (default = cmd)
    if command -v "$cmd" &>/dev/null; then
        return 0
    fi
    info "Installing $pkg…"
    if [[ "$OS" == "macos" ]]; then
        ensure_homebrew
        brew install "$pkg"
    else
        local pm; pm=$(detect_pkg_manager)
        case "$pm" in
            apt)    need_sudo apt-get install -y "$pkg" ;;
            dnf)    need_sudo dnf install -y "$pkg" ;;
            pacman) need_sudo pacman -Sy --noconfirm "$pkg" ;;
            zypper) need_sudo zypper install -y "$pkg" ;;
            *)      error "Could not auto-install $pkg. Please install it manually." ;;
        esac
    fi
}

# ---------------------------------------------------------------------------
# Homebrew (macOS only)
# ---------------------------------------------------------------------------
ensure_homebrew() {
    if command -v brew &>/dev/null; then return 0; fi
    info "Installing Homebrew (macOS package manager)…"
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    # Add Homebrew to PATH for Apple Silicon
    if [[ -x /opt/homebrew/bin/brew ]]; then
        eval "$(/opt/homebrew/bin/brew shellenv)"
    fi
}

# ---------------------------------------------------------------------------
# vcpkg (C++ package manager)
# ---------------------------------------------------------------------------
VCPKG_DEFAULT_DIR="$HOME/.local/share/vcpkg"

find_or_install_vcpkg() {
    # 1. Already in PATH?
    if command -v vcpkg &>/dev/null; then
        VCPKG_ROOT="$(dirname "$(command -v vcpkg)")"
        export VCPKG_ROOT
        return 0
    fi

    # 2. VCPKG_ROOT env var set and valid?
    if [[ -n "${VCPKG_ROOT:-}" && -x "$VCPKG_ROOT/vcpkg" ]]; then
        export PATH="$VCPKG_ROOT:$PATH"
        return 0
    fi

    # 3. Common install locations
    for candidate in \
            "$HOME/.local/share/vcpkg" \
            "$HOME/vcpkg" \
            "/opt/vcpkg" \
            "/usr/local/vcpkg"; do
        if [[ -x "$candidate/vcpkg" ]]; then
            export VCPKG_ROOT="$candidate"
            export PATH="$VCPKG_ROOT:$PATH"
            return 0
        fi
    done

    # 4. Not found — bootstrap it
    warn "vcpkg not found. Cloning and bootstrapping into $VCPKG_DEFAULT_DIR …"
    echo -e "${YELLOW}This is a one-time operation that may take a few minutes.${NC}"
    echo ""
    read -rp "Press Enter to continue, or Ctrl-C to abort: "

    ensure_cmd git git
    mkdir -p "$(dirname "$VCPKG_DEFAULT_DIR")"
    git clone --depth=1 https://github.com/Microsoft/vcpkg.git "$VCPKG_DEFAULT_DIR"
    bash "$VCPKG_DEFAULT_DIR/bootstrap-vcpkg.sh" -disableMetrics

    export VCPKG_ROOT="$VCPKG_DEFAULT_DIR"
    export PATH="$VCPKG_ROOT:$PATH"

    success "vcpkg installed at $VCPKG_ROOT"
    echo -e "${YELLOW}Tip: add the following to your shell profile to persist this:${NC}"
    echo -e "  export VCPKG_ROOT=\"$VCPKG_ROOT\""
    echo -e "  export PATH=\"\$VCPKG_ROOT:\$PATH\""
}

# ---------------------------------------------------------------------------
# Build helpers for CMake + vcpkg projects
# ---------------------------------------------------------------------------
cmake_build() {
    local src_dir="$1"
    local build_dir="$2"
    local extra_cmake_args="${3:-}"

    info "Configuring with CMake…"
    # shellcheck disable=SC2086
    cmake -S "$src_dir" -B "$build_dir" \
          -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
          $extra_cmake_args

    local jobs; jobs=$(nproc 2>/dev/null || sysctl -n hw.logicalcpu 2>/dev/null || echo 4)
    info "Building (using $jobs parallel jobs)…"
    cmake --build "$build_dir" --parallel "$jobs"
}

# ---------------------------------------------------------------------------
# Component: C++ Server
# ---------------------------------------------------------------------------
setup_server() {
    header "Setting up C++ Server"

    # Prerequisites
    ensure_cmd git
    ensure_cmd cmake cmake
    ensure_cmd make make
    if [[ "$OS" == "macos" ]]; then
        ensure_cmd gcc gcc   # or xcode-select --install
    else
        ensure_cmd g++ g++
    fi

    find_or_install_vcpkg

    local src="$REPO_ROOT/server"
    local build="$src/build"

    cmake_build "$src" "$build"

    success "Server built successfully."
    echo ""
    info "Starting the server (listening on port 8099)…"
    echo -e "${YELLOW}Press Ctrl-C to stop.${NC}\n"

    # Try both possible output locations
    if   [[ -f "$build/ultimata-server" ]];         then exec "$build/ultimata-server"
    elif [[ -f "$build/Release/ultimata-server" ]]; then exec "$build/Release/ultimata-server"
    elif [[ -f "$build/Debug/ultimata-server" ]];   then exec "$build/Debug/ultimata-server"
    else error "Build succeeded but executable not found in $build"; fi
}

# ---------------------------------------------------------------------------
# Component: C++ SDL2 Client
# ---------------------------------------------------------------------------
setup_client() {
    header "Setting up C++ Game Client"

    # SDL2 requires a display
    if [[ "$OS" == "linux" && -z "${DISPLAY:-}${WAYLAND_DISPLAY:-}" ]]; then
        warn "No display detected (DISPLAY / WAYLAND_DISPLAY not set)."
        warn "The SDL2 client requires a graphical environment."
        warn "If you are connecting via SSH, use 'ssh -X' for X11 forwarding."
    fi

    # Prerequisites
    ensure_cmd git
    ensure_cmd cmake cmake
    ensure_cmd make make

    find_or_install_vcpkg

    local src="$REPO_ROOT/client"
    local build="$src/build"

    cmake_build "$src" "$build"

    success "Client built successfully."
    echo ""
    info "Starting the game client…"
    echo -e "${YELLOW}Make sure the server is running on localhost:8099 first.${NC}\n"

    local exe
    if   [[ -f "$build/bin/Ultimata" ]];   then exe="$build/bin/Ultimata"
    elif [[ -f "$build/bin/Release/Ultimata" ]]; then exe="$build/bin/Release/Ultimata"
    elif [[ -f "$build/bin/Debug/Ultimata" ]]; then exe="$build/bin/Debug/Ultimata"
    else error "Build succeeded but Ultimata executable not found in $build/bin"; fi

    cd "$REPO_ROOT/client"
    exec "$exe"
}

# ---------------------------------------------------------------------------
# Component: Python Server
# ---------------------------------------------------------------------------
setup_python_server() {
    header "Setting up Python Server"

    ensure_cmd python3 python3

    # Ensure pip
    if ! python3 -m pip --version &>/dev/null; then
        info "Installing pip…"
        if [[ "$OS" == "macos" ]]; then
            ensure_homebrew
            brew install python3   # installs pip as well
        else
            local pm; pm=$(detect_pkg_manager)
            case "$pm" in
                apt) need_sudo apt-get install -y python3-pip ;;
                dnf) need_sudo dnf install -y python3-pip ;;
                *) warn "pip not found. Please install it manually." ;;
            esac
        fi
    fi

    local req="$REPO_ROOT/python_server/requirements-dev.txt"
    if [[ -f "$req" ]]; then
        info "Installing Python dependencies…"
        python3 -m pip install -r "$req" --quiet
    fi

    success "Python server ready."
    echo ""
    info "Starting the Python server (listening on port 8099)…"
    echo -e "${YELLOW}Press Ctrl-C to stop.${NC}\n"

    cd "$REPO_ROOT/python_server"
    exec python3 simple_server.py
}

# ---------------------------------------------------------------------------
# Component: Node.js Client
# ---------------------------------------------------------------------------
setup_nodejs_client() {
    header "Setting up Node.js Client"

    # Node.js
    if ! command -v node &>/dev/null; then
        info "Installing Node.js…"
        if [[ "$OS" == "macos" ]]; then
            ensure_homebrew
            brew install node
        else
            local pm; pm=$(detect_pkg_manager)
            case "$pm" in
                apt)
                    # Use NodeSource for a recent LTS
                    curl -fsSL https://deb.nodesource.com/setup_lts.x | need_sudo bash -
                    need_sudo apt-get install -y nodejs
                    ;;
                dnf)
                    need_sudo dnf install -y nodejs
                    ;;
                *)
                    error "Please install Node.js 18+ from https://nodejs.org and re-run."
                    ;;
            esac
        fi
    fi

    local dir="$REPO_ROOT/nodejs-client"
    info "Installing npm dependencies…"
    npm --prefix "$dir" install --silent

    success "Node.js client ready."
    echo ""
    warn "The Node.js client requires the server to be running on localhost:8099."
    warn "Start the server first (./setup.sh server) in another terminal."
    echo ""
    info "Connecting to server…"
    echo -e "${YELLOW}Press Ctrl-C to disconnect.${NC}\n"

    cd "$dir"
    exec node --import tsx index.ts
}

# ---------------------------------------------------------------------------
# Interactive menu
# ---------------------------------------------------------------------------
pick_component() {
    echo ""
    echo -e "${BOLD}Which component would you like to set up and run?${NC}"
    echo ""
    echo "  1) server         — C++ multiplayer game server (port 8099)"
    echo "  2) client         — C++ SDL2 game client (needs a display)"
    echo "  3) python-server  — Python stub server (port 8099)"
    echo "  4) nodejs-client  — TypeScript/Node.js client (needs server)"
    echo ""
    read -rp "Enter choice [1-4]: " choice
    case "$choice" in
        1) COMPONENT="server" ;;
        2) COMPONENT="client" ;;
        3) COMPONENT="python-server" ;;
        4) COMPONENT="nodejs-client" ;;
        *) error "Invalid choice: $choice" ;;
    esac
}

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
header "Ultimata — Setup & Run"
echo -e "Repository: ${CYAN}$REPO_ROOT${NC}"
echo -e "Platform  : ${CYAN}$OS${NC}"

[[ -z "$COMPONENT" ]] && pick_component

case "$COMPONENT" in
    server)         setup_server ;;
    client)         setup_client ;;
    python-server)  setup_python_server ;;
    nodejs-client)  setup_nodejs_client ;;
    *)
        error "Unknown component: '$COMPONENT'\nValid choices: server, client, python-server, nodejs-client"
        ;;
esac
