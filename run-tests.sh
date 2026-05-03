#!/usr/bin/env bash
# run-tests.sh  – Run all automated tests for the Ultimata project.
#
# Usage:
#   ./run-tests.sh           Run all test suites
#   ./run-tests.sh python    Run only Python tests
#   ./run-tests.sh ts        Run only TypeScript tests
#   ./run-tests.sh cpp       Run only C++ tests (requires vcpkg + CMake)
#
# Requirements (installed automatically when missing):
#   - Python 3 + pip
#   - Node.js + npm
#   - CMake ≥ 3.21, vcpkg, and a C++23-capable compiler (for C++ tests)

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
FILTER="${1:-all}"

# Colours
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

pass() { echo -e "${GREEN}✓ $1${NC}"; }
fail() { echo -e "${RED}✗ $1${NC}"; }
info() { echo -e "${YELLOW}▶ $1${NC}"; }

OVERALL_PASS=true

# ---------------------------------------------------------------------------
# Python tests (python_server / pytest)
# ---------------------------------------------------------------------------
run_python_tests() {
    info "Running Python tests (pytest)…"
    pushd "$REPO_ROOT/python_server" > /dev/null

    # Install dev dependencies if not already present
    if ! python3 -m pytest --version &>/dev/null; then
        pip install -r requirements-dev.txt --quiet
    fi

    if python3 -m pytest -v; then
        pass "Python tests passed"
    else
        fail "Python tests failed"
        OVERALL_PASS=false
    fi

    popd > /dev/null
}

# ---------------------------------------------------------------------------
# TypeScript tests (nodejs-client / vitest)
# ---------------------------------------------------------------------------
run_ts_tests() {
    info "Running TypeScript tests (vitest)…"
    pushd "$REPO_ROOT/nodejs-client" > /dev/null

    # Install node dependencies if node_modules is missing
    if [ ! -d "node_modules" ]; then
        npm install --silent
    fi

    if npm test; then
        pass "TypeScript tests passed"
    else
        fail "TypeScript tests failed"
        OVERALL_PASS=false
    fi

    popd > /dev/null
}

# ---------------------------------------------------------------------------
# C++ tests (server / Google Test via CMake + vcpkg)
# ---------------------------------------------------------------------------
run_cpp_tests() {
    info "Running C++ server tests (Google Test)…"

    if [ -z "${VCPKG_ROOT:-}" ]; then
        echo "  ⚠  VCPKG_ROOT is not set – skipping C++ tests."
        echo "     Set VCPKG_ROOT to your vcpkg installation and re-run."
        return
    fi

    pushd "$REPO_ROOT/server" > /dev/null

    cmake --preset test -B build-tests -S . -DBUILD_TESTING=ON -Wno-dev
    cmake --build build-tests --target ultimata-server-tests -j"$(nproc 2>/dev/null || sysctl -n hw.logicalcpu 2>/dev/null || echo 4)"

    if ctest --test-dir build-tests --output-on-failure; then
        pass "C++ tests passed"
    else
        fail "C++ tests failed"
        OVERALL_PASS=false
    fi

    popd > /dev/null
}

# ---------------------------------------------------------------------------
# Dispatch
# ---------------------------------------------------------------------------
echo ""
echo "══════════════════════════════════════════"
echo "   Ultimata – Test Runner"
echo "══════════════════════════════════════════"
echo ""

case "$FILTER" in
    python) run_python_tests ;;
    ts|typescript) run_ts_tests ;;
    cpp|c++) run_cpp_tests ;;
    all|*)
        run_python_tests
        run_ts_tests
        run_cpp_tests
        ;;
esac

echo ""
if $OVERALL_PASS; then
    pass "All test suites passed! 🎉"
    exit 0
else
    fail "One or more test suites failed."
    exit 1
fi
