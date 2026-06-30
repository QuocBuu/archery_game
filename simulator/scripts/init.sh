#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SIMULATOR_DIR="$(dirname "$SCRIPT_DIR")"
REPO_DIR="$(dirname "$SIMULATOR_DIR")"
EMSDK_DIR="$SIMULATOR_DIR/emsdk"

echo "~ Checking native build dependencies..."

command -v gcc        >/dev/null 2>&1 || { echo "ERROR: gcc not found — run: sudo apt install build-essential";      exit 1; }
command -v g++        >/dev/null 2>&1 || { echo "ERROR: g++ not found — run: sudo apt install build-essential";      exit 1; }
command -v pkg-config >/dev/null 2>&1 || { echo "ERROR: pkg-config not found — run: sudo apt install pkg-config";   exit 1; }
pkg-config --exists sdl2              || { echo "ERROR: libsdl2-dev not found — run: sudo apt install libsdl2-dev"; exit 1; }

echo "~ Native dependencies OK."

echo "~ Initializing emsdk submodule..."
git -C "$REPO_DIR" submodule update --init -- simulator/emsdk

echo "~ Installing Emscripten toolchain — first run downloads ~1-2 GB..."
cd "$EMSDK_DIR"
./emsdk install latest
./emsdk activate latest

echo "~ All dependencies installed."
